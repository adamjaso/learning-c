#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ifaddrs.h>

// This program will list all INET4 network interfaces and their addresses.

void ip_uint_to_str(char *addrstr, const unsigned int *addr) {
    unsigned int octs[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++) {
        octs[3 - i] = (*addr >> (8 * i)) % 256;
    }
    sprintf(addrstr, "%d.%d.%d.%d", octs[0], octs[1], octs[2], octs[3]);
}

void ip_sockaddrin_to_uint(unsigned int *addruint, const struct sockaddr_in *ifa) {
    *addruint = ntohl(((struct sockaddr_in *) ifa)->sin_addr.s_addr);
}

int main(void) {
    int ret;
    char addrstr[16], maskstr[16];
    unsigned int iaddr;
    struct ifaddrs *ifaddr;

    ifaddr = NULL;
    ret = getifaddrs(&ifaddr);

    if (ret == -1) {
        printf("getifaddrs %d\n", errno);
    } else {
        for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr != NULL && ifa->ifa_addr->sa_family == AF_INET) {
                ip_sockaddrin_to_uint(&iaddr, (struct sockaddr_in *) ifa->ifa_addr);
                ip_uint_to_str(addrstr, &iaddr);

                ip_sockaddrin_to_uint(&iaddr, (struct sockaddr_in *) ifa->ifa_netmask);
                ip_uint_to_str(maskstr, &iaddr);

                printf("%s\t%s\t%s\n", ifa->ifa_name, addrstr, maskstr);
            }
        }
    }
    freeifaddrs(ifaddr);
    return 0;
}
