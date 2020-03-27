#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <zconf.h>
#include <netinet/in.h>
#include <libnet.h>

#define MAX_SIZE 2000
#define IP "127.0.0.1"
#define PORT 4242

void sendFile(int sockFd, char *filename) {
    char buff[MAX_SIZE] = {0};
    FILE *pFile;

    pFile = fopen(filename, "r");
    if (!pFile) {
        fprintf(stderr, "Error trying to open file : %s\n", filename);
        return;
    }
    //Writes the files content on the socket
    while (fgets(buff, MAX_SIZE, pFile))
        write(sockFd, buff, sizeof(buff));
    fclose(pFile);
}

int main(int ac, char **av) {
    int localFd, distantFd, len;
    struct sockaddr_in localSock, distantSock;
    char *filename;

    if (ac != 2 || !av[1]) {
        fprintf(stderr, "Invalid arguments, exiting.");
        printf("usage : ./server filename");
        return (84);
    }
    //Get the name of the file that has to be transferred
    filename = av[1];
    //Create the server socket
    localFd = socket(AF_INET, SOCK_STREAM, 0);
    if (localFd < 0) {
        fprintf(stderr, "Error creating the TCP socket, exiting.\n");
        return 84;
    }
    bzero(&localSock, sizeof(localSock));
    //Setting the ip, the family and the port of the socket
    localSock.sin_family = AF_INET;
    if (inet_aton(IP, (struct in_addr *)&localSock.sin_addr.s_addr) == 0) {
        fprintf(stderr, "Invalid IP, exiting.\n");
        return 84;
    }
    localSock.sin_port = htons(PORT);
    //Bind the socket to the selected port
    if (bind(localFd, (struct sockaddr *)&localSock, sizeof(localSock)) != 0) {
        fprintf(stderr, "Bind failed, exiting.\n");
        return 84;
    }
    //Start listening on the selected port
    if (listen(localFd, 10) != 0) {
        fprintf(stderr,"Listen failed, exiting\n");
        return 84;
    }
    len = sizeof(distantSock);
    //Accept connections towards the server
    distantFd = accept(localFd, (struct sockaddr *)&distantSock, (socklen_t *)&len);
    if (distantFd < 0) {
        fprintf(stderr, "Local accept failed, exiting.");
        return 84;
    }
    //Send the file
    sendFile(distantFd, filename);
    //Close the socket
    close(localFd);
    return 0;
}