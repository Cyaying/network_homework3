#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <error.h>


#define BUF_SIZE 255


int main(int argc, char *argv[])
{
    /* create socket */
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);

    /* bind socket with ip and port */
    struct sockaddr_in server_addr;
    int port = atoi(argv[1]);

    memset(&server_addr, 0, sizeof(server_addr));     // fill with 0
    server_addr.sin_family = AF_INET;                 // use IPv4
    server_addr.sin_port = htons(port);               // port 9999
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // local ip
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        exit(1);
    }

    /* listen, wait for the client */
    printf("server is listening ... \n");
    if (listen(server_sock, 20))
    {
        perror("listen failed");
        exit(1);
    }

    /* receive post from client */
    struct sockaddr client_addr;
    int client_addr_size = sizeof(client_addr);
    char recv_msg[BUF_SIZE] = {0};

    while (1)
    {
        int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
        if (client_sock < 0)
        {
            perror("accept socket failed");
            exit(1);
        }
        printf("server is accepting ... \n");

        while (1)
        {
            /* receive and show client message */
            if (read(client_sock, recv_msg, BUF_SIZE) < 0)
            {
                perror("receive from client failed");
                exit(1);
            }

            if (strcmp(recv_msg, "bye") == 0) break;

            printf("Recv: %s\n", recv_msg);

            /* send message to client */
            int len = strlen(recv_msg);
            for (int i = 0; i < len / 2; i++)
            {
                char tmp = recv_msg[i];
                recv_msg[i] = recv_msg[len - 1 - i];
                recv_msg[len - 1 - i] = tmp;
            }
            printf("Send: %s\n", recv_msg);
            if (write(client_sock, recv_msg, strlen(recv_msg)) < 0)
            {
                perror("send to client failed");
                exit(1);
            }

            /* reset recv_mesg */
            memset(recv_msg, 0, BUF_SIZE);
        }
        
        /* close data socket */
        if (close(client_sock) < 0)
        {
            perror("close data socket failed");
            exit(1);
        }
    }

    /* close listen socket */
    if (close(server_sock))
    {
        perror("close listen socket failed");
        exit(1);
    }

    return 0;
}
