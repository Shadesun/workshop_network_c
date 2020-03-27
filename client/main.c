#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <zconf.h>
#include <netinet/in.h>
#include <libnet.h>

#define MAX_READ 2000
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 4242

void retrieveFile(int sockFd) {
    char buff[MAX_READ] = {0};
    FILE *pFile;

    pFile = fopen("got.txt", "w");
    if (!pFile) {
        fprintf(stderr,"File creation failed.\n");
        return;
    }
    //Read until the socket is empty
    while(read(sockFd, buff, MAX_READ) > 0)
        fprintf(pFile, "%s", buff);
}

int main(int ac, char **av) {
    int distantFd;
    struct sockaddr_in distantSock;

    //Create a new socket
    distantFd = socket(AF_INET, SOCK_STREAM, 0);
    if (distantFd < 0) {
        fprintf(stderr, "Error creating the TCP socket, exiting.\n");
        return 84;
    }
    bzero(&distantSock, sizeof(distantSock));
    //Set the ip and the port of the server's socket
    distantSock.sin_family = AF_INET;
    if (inet_aton(SERVER_IP, (struct in_addr *)&distantSock.sin_addr.s_addr) == 0) {
        fprintf(stderr, "Invalid IP, exiting.\n");
        return 84;
    }
    distantSock.sin_port = htons(SERVER_PORT);
    //Connect to the server
    if (connect(distantFd, (struct sockaddr *)&distantSock, sizeof(distantSock)) != 0) {
        fprintf(stderr,"Connection to the peer failed, exiting.");
        return 84;
    }
    //Retrieve the file
    retrieveFile(distantFd);
    //Close the socket
    close(distantFd);
    return 0;
}