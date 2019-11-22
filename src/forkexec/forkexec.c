#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

// This program should execute a test script passing in a custom environment
// variable.

#define TESTSCRIPT "./test.sh"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("arg 1 is missing\n");
        exit(EXIT_FAILURE);
    }
    pid_t child, wstatus;
    child = fork();
    if (child == -1) {
        printf("fork");
        exit(EXIT_FAILURE);
    }
    if (child == 0) {
        // child process
        char *script[] = {TESTSCRIPT, NULL};
        char *env[] = {argv[1], NULL};
        if (execve(script[0], script, env) == -1) {
            printf("execve %d\n", errno);
            exit(EXIT_FAILURE);
        }
    } else {
        // parent process
        child = wait(&wstatus);
        if (WIFEXITED(wstatus)) {
            printf("child %d exited with status %d\n", child, WEXITSTATUS(wstatus));
            exit(EXIT_SUCCESS);
        } else if (WIFSIGNALED(wstatus)) {
            printf("child %d killed by signal %d%s\n", child, WTERMSIG(wstatus), WCOREDUMP(wstatus) ? " (dumped core)" : "");
            exit(EXIT_FAILURE);
        } else if (WIFSTOPPED(wstatus)) {
            printf("child %d stopped by signal %d\n", child, WSTOPSIG(wstatus));
        } else if (WIFCONTINUED(wstatus)) {
            printf("child %d continued\n", child);
        } else {
            printf("child %d exited with unknown status %d\n", child, wstatus);
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}
