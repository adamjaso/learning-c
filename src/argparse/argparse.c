#include <stdio.h>

// This program reads and prints all command line arguments.

int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++)
        printf("arg[%d]=%s\n", i, argv[i]);
    return 0;
}
