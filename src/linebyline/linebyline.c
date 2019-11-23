#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void) {
    char *lineptr;
    size_t lineptrsize;
    ssize_t nread;

    lineptr = NULL;
    lineptrsize = 0;
    nread = 0;

    nread = getline(&lineptr, &lineptrsize, stdin);
    for (int i = 0; nread != -1; i++) {
        printf("line %d : %s", i+1, lineptr);
        nread = getline(&lineptr, &lineptrsize, stdin);
    }

    if (errno != 0) {
        printf("getline %d", errno);
    }
    free(lineptr);

    return 0;
}
