#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<openssl/sha.h>

#define perr(msg) do { (errno ? printf(msg"%d \n", errno) : printf(msg"\n")); exit(1); } while(0);
#define BUFSIZE 16384
#define DIGESTSIZE 32

int main() {
    SHA256_CTX ctx;
    char buf[BUFSIZE];
    unsigned char digest[DIGESTSIZE];
    ssize_t ret;

    ret = BUFSIZE;
    if (!SHA256_Init(&ctx)) perr("init failed");
    while (ret == BUFSIZE) {
        ret = read(STDIN_FILENO, buf, sizeof(buf));
        if (!SHA256_Update(&ctx, &buf, ret)) perr("update failed");
    }
    if (-1 == ret) perr("read");
    if (!SHA256_Final(&digest[0], &ctx)) perr("final failed");
    ret = write(STDOUT_FILENO, &digest, DIGESTSIZE);
    if (-1 == ret) perr("write");
    return 0;
};
