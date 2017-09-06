#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 5


void logexit(const char *str)
{
        if(errno) perror(str);
        else puts(str);
        exit(EXIT_FAILURE);
}


int main(int argc, char **argv)
{
        int s = socket(AF_INET, SOCK_STREAM, 0);
        if(s == -1)
            logexit("socket");

        struct in_addr addr = { .s_addr = htonl(INADDR_ANY) };
        struct sockaddr_in dst;
        dst.sin_family = AF_INET,
        dst.sin_port = htons(51515);

        if(inet_pton(AF_INET, "192.168.1.162", &dst.sin_addr) <= 0)
            logexit("inet_pton error occured");

        struct sockaddr *sa_dst = (struct sockaddr *)&dst;

        if(connect(s, sa_dst, sizeof(dst)))
            logexit("connect");

        send(s, "hello", 5, 0);

        char buffer[BUFFER_SIZE];
        if(0 >= recv(s, buffer, BUFFER_SIZE, MSG_WAITALL))
            logexit("recv");
        
        fprintf(stdout, "Received %s\n", buffer);

        close(s);
        exit(EXIT_SUCCESS);
}