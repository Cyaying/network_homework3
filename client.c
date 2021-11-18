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
    /* server address init */
    struct sockaddr_in server_addr;
    char *host = argv[1];
    int port = atoi(argv[2]);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_port = htons(port);

    /* define send and receive buffer */
    char send_msg[BUF_SIZE] = {0};
    char recv_msg[BUF_SIZE] = {0};

    /* create socket */
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0)
    {
        perror("create socket failed");
        exit(1);
    }

    /* connect to server */
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("connect to server failed");
        exit(1);
    }

    while (1)
    {
        memset(recv_msg, 0, sizeof(recv_msg));
        memset(send_msg, 0, sizeof(send_msg));

        /* send message to server */
        printf("Send: ");
        scanf("%s", send_msg);
        getchar();

        // send(client_sock, send_msg, BUF_SIZE, 0);
        if (write(client_sock, send_msg, BUF_SIZE) < 0)
        {
            perror("send msg failed");
            exit(1);
        }

        if (strcmp(send_msg, "bye") == 0) break;

        /* receive message from server */
        if (read(client_sock, recv_msg, sizeof(recv_msg)) < 0)
        {
            perror("recv msg failed");
            exit(1);
        }
        printf("Recv: %s\n", recv_msg);
    }

    /* close socket */
    if (close(client_sock) < 0)
    {
        perror("close socket failed");
        exit(1);
    }

    return 0;
}
