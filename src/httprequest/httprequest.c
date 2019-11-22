#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/ip.h>

// This program sends an HTTP request to 127.0.0.1:8000 and prints
// the response.

#define HOST 2130706433
#define PORT 8000
#define REQUEST "GET /httprequest.c HTTP/1.1\r\n\r\n"
#define BUFLEN 32

int main() {
    int sockfd;
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORT);
    sa.sin_addr.s_addr = htonl(HOST);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket errno %d\n", errno);
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *) &sa, (socklen_t) sizeof (struct sockaddr_in)) == -1) {
        printf("connect errno %d\n", errno);
        return 1;
    }

    char req[BUFLEN] = REQUEST;
    if (write(sockfd, req, strlen(req)) == -1) {
        printf("write errno %d\n", errno);
        return 1;
    }

    char n = 1, res[BUFLEN];
    while (n > 0) {
        n = read(sockfd, res, BUFLEN);
        if (n < 0) {
            printf("read errno %d\n", errno);
            return 1;
        } else if (n == 0) {
            break;
        }
        if (write(1, res, n) == -1) {
            printf("print errno %d\n", errno);
            return 1;
        }
    }

    if (close(sockfd) == -1) {
        printf("close errno %d\n", errno);
        return 1;
    }

    return 0;
}
