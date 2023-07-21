#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>

#define perr(msg) do { printf(msg" %d\n", errno) ; exit(1) ; } while (0);
#define USAGE "USAGE: %s BINDADDR BINDPORT\n"
#define OKRESP "HTTP/1.1 200\r\nContent-Type: text/plain\r\nContent-Length: %d\r\nConection: close\r\n\r\n"
#define BADRESP "HTTP/1.1 400\r\nContent-Type: text/plain\r\nContent-Length: %d\r\nConnection: close\r\n\r\n"
#define REQ_SIZE 8191
#define HEADER_SIZE 1023

int main(int argc, char **argv) {
    int sock, rsock, sockopt, hasmore;
    ssize_t reqsize, headersize;
    struct sockaddr_in raddr, *laddr;
    socklen_t raddrlen;
    struct addrinfo hints, *addr;
    char *bindaddr, *bindport, *pos, header[HEADER_SIZE+1], req[REQ_SIZE+1];

    raddrlen = sizeof(struct sockaddr_in);

    if (argc < 3) { printf(USAGE, argv[0]); exit(1); }
    bindaddr = argv[1];
    bindport = argv[2];

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;

    if (-1 == getaddrinfo(bindaddr, bindport, &hints, &addr)) perr("getaddrinfo");
    laddr = (struct sockaddr_in *) addr->ai_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    sockopt = 1;
    if (-1 == sock) perr("socket");
    if (-1 == setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(int))) perr("setsockopt");
    if (-1 == bind(sock, (struct sockaddr *) laddr, sizeof(struct sockaddr_in))) perr("bind");
    freeaddrinfo(addr);
    if (-1 == listen(sock, 1)) perr("listen");
    while (1) {
        // open sock
        rsock = accept(sock, (struct sockaddr *) &raddr, &raddrlen);
        if (-1 == rsock) perr("accept");
        // read request
        reqsize = recv(rsock, req, REQ_SIZE, 0);
        if (-1 == reqsize) perr("recv");
        if (-1 == ioctl(rsock, FIONREAD, &hasmore)) perr("ioctl");
        if (hasmore) {
            // log request
            printf("time=%ld addr=%s bytes=%ld header=TOO_LARGE\n", time(NULL), inet_ntoa(raddr.sin_addr), reqsize + hasmore);
            strncpy(req, "request too large", REQ_SIZE);
            reqsize = strlen(req);
            headersize = snprintf(header, HEADER_SIZE, BADRESP, (int) reqsize);
            if (-1 == send(rsock, header, headersize, 0)) perr("send");
        } else {
            // log request
            pos = strchr(req, '\n');
            if (pos == NULL) pos = req;
            strncpy(header, req, pos - req);
            header[pos - req] = '\0';
            printf("time=%ld addr=%s bytes=%ld header=%s\n", time(NULL), inet_ntoa(raddr.sin_addr), reqsize, header);
            // send headers
            headersize = snprintf(header, HEADER_SIZE, OKRESP, (int) reqsize);
            if (-1 == send(rsock, header, headersize, 0)) perr("send");
        }
        // send response
        if (-1 == send(rsock, req, reqsize, 0)) perr("send");
        // close sock
        close(rsock);
    }
    return 0;
};
