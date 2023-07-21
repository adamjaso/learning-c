#include<stdio.h>
#include<string.h>
#include<openssl/sha.h>
#include<openssl/hmac.h>

#define DIGESTSIZE 32

#define shastr(d) \
    "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x" \
    "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", \
    d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], \
    d[8], d[9], d[10], d[11], d[12], d[13], d[14], d[15], \
    d[16], d[17], d[18], d[19], d[20], d[21], d[22], d[23], \
    d[24], d[25], d[26], d[27], d[28], d[29], d[30], d[31]

void hmac(unsigned char *key, int keysize, unsigned char *data, unsigned char digest[DIGESTSIZE]) {
    unsigned int digest_len;
    digest_len = DIGESTSIZE;
    memset(digest, 0, 32);
    HMAC(EVP_sha256(), key, keysize, data, strlen((char *)data), digest, &digest_len);
};

int main() {
    unsigned char key[] = "abc", data1[] = "def", data2[] = "ghi", md1[DIGESTSIZE], md2[DIGESTSIZE];
    hmac(key, 3, data1, md1);
    hmac(md1, DIGESTSIZE, data2, md2);
    printf(shastr(md2));
    printf("\n");
    return 0;
};
