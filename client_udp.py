import sys
import time
from socket import *
from threading import Thread


class Client:
    def __init__(self, client_name, server_host, server_port):
        self.server_addr = (server_host, int(server_port))
        self.sk = socket(AF_INET, SOCK_DGRAM)

        while True:
            self.sk.sendto(client_name.encode(), self.server_addr)
            mesg = self.sk.recv(1024).decode()
            if client_name in mesg:
                self.client_name = client_name
                print("+-----------------------------------------------------+")
                print("|  WELCOME TO THE CHAT ROME, ENTER \"exit\" TO LEAVE !  |")
                print("+-----------------------------------------------------+")
                break
            else:
                client_name = input("Please choose other names: ").strip()

        self.thread_recv = Thread(target=self.recv_mesg, daemon=True)
        self.thread_send = Thread(target=self.send_mesg, daemon=True)
        self.thread_recv.start()
        self.thread_send.start()
        self.thread_send.join()

    def send_mesg(self):
        while True:
            client_mesg = input().strip()
            client_mesg = self.client_name + ": " + client_mesg
            self.sk.sendto(client_mesg.encode(), self.server_addr)
            if "exit" in client_mesg:
                print("---- CONNECTION CLOSED ! ----")
                self.sk.close()
                sys.exit(0)

    def recv_mesg(self):
        while True:
            try:
                server_mesg = self.sk.recv(1034)
                if not server_mesg:
                    print("SERVER INTERRUPT !")
                    sys.exit(0)
                server_mesg = server_mesg.decode()
                if self.client_name not in server_mesg:
                    print(server_mesg)
            except Exception as e:
                print("ERROR !")


if __name__ == "__main__":
    host = input("Enter the chat room host: ").strip()
    port = input("Enter the chat room port: ").strip()
    name = input("Enter the only user name: ").strip()
    client = Client(name, host, port)
