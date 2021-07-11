#include <stdio.h>// including things from stdio and stdlib
#include <stdlib.h>

#include<sys/types.h>//socket function definitions
#include <sys/socket.h>
#include <arpa/inet.h>

#include <netinet/in.h>//structure for storing address information
#include <string.h>
#include <unistd.h> //socket close function
#include <fcntl.h>


int main(int argc, char **argv) {
    char *addr;                  /* will be a pointer to the address */
    struct sockaddr_in address;  /* the libc network address data structure */
    fd_set fdset;
    struct timeval tv;

    if (argc != 4) {
        fprintf(stderr, "Usage %s <address> <port_num_start> <port_num_end>\n", argv[0]);
        return EXIT_FAILURE;
    }

    addr = argv[1];

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(addr); /* assign the address */
    for(int i = atoi(argv[2]); i <= atoi(argv[3]); i++)
    {   address.sin_port = htons(i);            /* translate int2port num */
    
        short int sock = socket(AF_INET, SOCK_STREAM, 0);// create a socket
        fcntl(sock, F_SETFL, O_NONBLOCK);// set socket to non blocking mode
    
        connect(sock, (struct sockaddr *)&address, sizeof(address));
    
        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);
        tv.tv_sec = 0;             /* 1 second timeout */
        tv.tv_usec = 100000;
    
        if (select(sock + 1, NULL, &fdset, NULL, &tv) == 1)
        {
            int so_error;
            socklen_t len = sizeof so_error;
    
            getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
    
            if (so_error == 0) {
                printf("port:%d is open\n", i);
            }
        }
        else
        {
            printf("port:%d is closed\n", i);
        }
    
        close(sock);
    }
    return 0;
}