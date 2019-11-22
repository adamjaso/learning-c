#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>

#define OCTETSIZE sizeof (char) * 3

void ip_octets_to_uint(unsigned int *octets, unsigned int *addr) {
    *addr = 0;
    for (int i = 0; i < 4; i++) *addr += octets[i] << ((3 - i) * 8);
}

int str_to_uint(char *octstr, unsigned int *octint) {
    *octint = 0;
    for (char *ch = octstr; *ch != '\0'; ch ++) {
        if (!isdigit(*ch)) return -1;
        *octint = *octint * 10 + (*ch - '0');
    }
    if (*octint >= 256) return -1;
    return 0;
}

int ip_str_to_octets(char *addrstr, unsigned int *addr) {
    char oct[3];
    char *pos;
    int i;
    size_t n;
    for (pos = addrstr, i = 0; i < 4; i ++) {
        memset(oct, 0, OCTETSIZE);
        n = strchr(pos, i < 3 ? '.' : '\0') - pos;
        if (n > OCTETSIZE) return -1;
        memcpy(oct, pos, n);
        if (str_to_uint(oct, &addr[i]) != 0) return -1;
        pos = strchr(pos, '.') + sizeof (char);
    }
    return 0;
}

int ip_str_to_uint(char *addrstr, unsigned int *addr) {
    unsigned int octets[4] = {0, 0, 0, 0};
    if (ip_str_to_octets(addrstr, octets) != 0) return -1;
    ip_octets_to_uint(octets, addr);
    return 0;
}

int ip_str_to_sockaddr_in(char *addrstr, int port, struct sockaddr_in *sa) {
    unsigned int addr;
    if (ip_str_to_uint(addrstr, &addr) != 0) return -1;
    memset(sa, 0, sizeof (struct sockaddr_in));
    sa->sin_family = AF_INET;
    sa->sin_port = htons(port);
    sa->sin_addr.s_addr = htonl(addr);
    return 0;
}

int main(int argc, char **argv) {
    char *addrstr;
    int i, res;
    unsigned int addr;
    for (i = 1, addrstr = argv[i]; i < argc; addrstr = argv[++i]) {
        addr = 0;
        res = ip_str_to_uint(addrstr, &addr);
        printf("%s\t%s\t%u\n", !res ? "ok " : "bad", addrstr, addr);
    }
    return 0;
}
