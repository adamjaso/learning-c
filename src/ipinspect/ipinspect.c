#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define COLNAMES "RANGE", "ADDR", "NET", "MASK", "BCAST", "BITS", "SIZE"

int main(int argc, char **argv) {
    unsigned short bits = 32;
    unsigned int size = 1, net, bcast, mask, addr;
    char *line = NULL, *bitstr = NULL;
    size_t linesz;
    ssize_t ret, lret;
    struct in_addr iaddr;
    int showhex = 0;

    if (argc > 1) {
        if (!strcmp(argv[1], "-x")) {
            showhex = 1;
        } else if (!strcmp(argv[1], "-h")) {
            printf("usage: %s [-x] [-h]\n", argv[0]);
            return 1;
        }
    }

    if (showhex) {
        fprintf(stderr, "%20s  %8s  %8s  %8s  %8s  %4s  %10s\n", COLNAMES);
    } else {
        fprintf(stderr, "%20s  %15s  %15s  %15s  %15s  %4s  %10s\n", COLNAMES);
    }
    lret = getline(&line, &linesz, stdin);
    while (lret != -1) {
        bitstr = strchr(line, '/'); // find bits
        if (bitstr != NULL) {
            *bitstr++ = 0;
            bits = (unsigned short) atoi(bitstr);
        }
        if (bits < 1 || bits > 32) {
            fprintf(stderr, "invalid bits %s\n", line);
            goto next;
        }
        ret = inet_aton(line, &iaddr);
        if (ret == -1) {
            fprintf(stderr, "inet_aton %d\n", errno);
            goto next;
        }
        addr = ntohl(iaddr.s_addr);
        size = 1<<(32-bits);
        mask = (~(0<<bits))<<(32-bits);
        net = addr & mask;
        bcast = net | (size-1);

        (bitstr != NULL) ? *(--bitstr) = '/' : 0;
        line[lret-1] = 0; // drop newline
        if (showhex) {
            fprintf(stdout, "%20s  %08x  %08x  %08x  %08x  % 4u  % 10u\n",
                    line, addr, net, mask, bcast, bits, size);
        } else {
            fprintf(stdout, "%20s  ", line);
            iaddr.s_addr = htonl(addr); fprintf(stdout, "%15s  ", inet_ntoa(iaddr));
            iaddr.s_addr = htonl(net); fprintf(stdout, "%15s  ", inet_ntoa(iaddr));
            iaddr.s_addr = htonl(mask); fprintf(stdout, "%15s  ", inet_ntoa(iaddr));
            iaddr.s_addr = htonl(bcast); fprintf(stdout, "%15s  ", inet_ntoa(iaddr));
            fprintf(stdout, "% 4u  ", bits);
            fprintf(stdout, "% 10u\n", size);
        }
next:
        lret = getline(&line, &linesz, stdin);
    }
    if (errno != 0) {
        fprintf(stderr, "getline %d\n", errno);
        return 1;
    }
    return 0;
};
