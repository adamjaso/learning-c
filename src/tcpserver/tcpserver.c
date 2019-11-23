#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#define HOST 2130706433
#define PORT 8000
#define BUFLEN 1024

int main(void) {
    int sockfd, rsockfd, sockopt;
    char buf[BUFLEN];
    struct sockaddr_in sa;
    socklen_t socklen;

    socklen = (socklen_t) sizeof (struct sockaddr_in);
    memset(&buf, 0, BUFLEN);
    memset(&sa, 0, socklen);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORT);
    sa.sin_addr.s_addr = htonl(HOST);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket %d\n", errno);
        return 1;
    }

    sockopt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(int)) == -1) {
        printf("setsockopt %d\n", errno);
        return 1;
    }

    if (bind(sockfd, (struct sockaddr *) &sa, socklen) == -1) {
        printf("bind %d\n", errno);
        return 1;
    }

    if (listen(sockfd, 1) == -1) {
        printf("listen %d\n", errno);
        return 1;
    }

    rsockfd = accept(sockfd, (struct sockaddr *) &sa, &socklen);
    if (rsockfd == -1) {
        printf("accept %d\n", errno);
        return 1;
    }

    if (read(rsockfd, buf, sizeof (char) * 1024) == -1) {
        printf("read %d\n", errno);
        return 1;
    }
    printf("data : %s\n", buf);

    if (write(rsockfd, buf, sizeof (char) * 1024) == -1) {
        printf("write %d\n", errno);
        return 1;
    }

    if (close(rsockfd) == -1) {
        printf("close rsockfd %d\n", errno);
    }

    if (close(sockfd) == -1) {
        printf("close sockfd %d\n", errno);
    }

    return 0;
}
