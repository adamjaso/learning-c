#include <unistd.h>
#include <stdio.h>

// This program prints all environment variables to stdout.

int main(void) {
    for (char **env = __environ; *env != NULL; *env++) {
        printf("%s\n", *env);
    }
}
