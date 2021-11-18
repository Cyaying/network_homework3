from socket import *
from threading import Thread


class Server:
    user_names_addrs = {}
    addr = ("127.0.0.1", 9999)

    # init to create and bind socket & create thread
    def __init__(self):
        self.sk = socket(AF_INET, SOCK_DGRAM)
        self.sk.bind(self.addr)
        self.thread_recv = Thread(target=self.mesg_recv)
        print("+-----------------------------------------------------+")
        print("|  WELCOME TO THE CHAT ROME, ENTER \"exit\" TO LEAVE !  |")
        print("+-----------------------------------------------------+")
        self.thread_recv.start()
        self.thread_recv.join()

    # send message to all users
    def mesg_to_all(self, client_mesg):
        for item in self.user_names_addrs:
            self.sk.sendto(client_mesg, self.user_names_addrs[item])

    # send message to users except user called "name"
    def mesg_to_others(self, name, client_mesg):
        for item in self.user_names_addrs.keys():
            if item == name:
                continue
            self.sk.sendto(client_mesg, self.user_names_addrs[item])

    # receive message from client user
    def mesg_recv(self):
        while True:
            try:
                client_mesg, client_addr = self.sk.recvfrom(1024)
                client_mesg = client_mesg.decode()

                # check whether the username exists
                if ":" not in client_mesg:
                    if client_mesg in self.user_names_addrs:
                        mesg = "Please choose other names."
                        self.sk.sendto(mesg.encode(), client_addr)
                    else:
                        mesg = "---- Welcome %s to the chat room. ----" % client_mesg
                        print(mesg)
                        self.user_names_addrs[client_mesg] = client_addr
                        self.mesg_to_all(mesg.encode())
                else:
                    client_name = client_mesg.split(":")[0]
                    # user input "exit" to leave
                    if "exit" in client_mesg:
                        mesg = "----  %s has left the chat room.  ----" % client_name
                        print(mesg)
                        self.user_names_addrs.pop(client_name)
                        self.mesg_to_others(client_name, mesg.encode())
                    # user send normal message, broadcast to others
                    else:
                        print(client_mesg)
                        self.mesg_to_all(client_mesg.encode())

            except Exception as e:
                print("Unexpected error appeared !")
                print(e)


if __name__ == "__main__":
    server = Server()
