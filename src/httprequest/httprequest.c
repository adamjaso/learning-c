#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#define perr(msg) do { printf(msg" %d\n", errno) ; exit(1) ; } while (0);
#define BUFSIZE 80
#define GET "GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n"
#define HOSTNAME "www.google.com"
int main() {
    int sock;
    ssize_t ret;
    struct sockaddr_in src;
    struct addrinfo hints, *dst;
    char buf[BUFSIZE];

    memset(&src, 0, sizeof(src));
    src.sin_family = AF_INET;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;

    if (-1 == getaddrinfo(HOSTNAME, "80", &hints, &dst)) perr("getaddrinfo");
    if (!dst) perr("unable to resolve "HOSTNAME);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sock) perr("socket");
    if (-1 == bind(sock, (struct sockaddr *) &src, sizeof (struct sockaddr_in))) perr("bind");
    if (-1 == connect(sock, (struct sockaddr *) dst->ai_addr, sizeof (struct sockaddr_in))) perr("connect");
    if (-1 == send(sock, GET, strlen(GET)+1, 0)) perr("send");

    while (1) {
        ret = recv(sock, buf, sizeof (buf), 0);
        if (-1 == ret) perr("recv");
        if (0 == ret) break;
        ret = fwrite(buf, ret, 1, stdout);
        if (-1 == ret) perr("fwrite");
    }
    printf("\n");
    freeaddrinfo(dst);

    return 0;
};
