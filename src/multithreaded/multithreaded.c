#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

// This program runs 8 threads in parallel.

#define NUM_THREADS 8
#define handle_error_n(msg, errno) \
    do { dprintf(2, msg, errno); exit(EXIT_FAILURE); } while (0);
#define handle_error(msg) handle_error_n(msg, errno)


void *thread_start(void *arg) {
    while (1) {
        printf("thread %d is running\n", *(int *) arg);
        sleep(1);
    }
    return NULL;
}

int main() {
    int e = 0;
    int *thread_data;
    pthread_t *threads;
    threads = calloc(NUM_THREADS, sizeof (pthread_t));
    if (threads == NULL) {
        handle_error("calloc == NULL %d\n");
    }
    thread_data = calloc(NUM_THREADS, sizeof (int));
    if (thread_data == NULL) {
        handle_error("thread_data == NULL %d\n");
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i] = i;
        if ((e = pthread_create(&threads[i],
                                NULL,
                                &thread_start,
                                &thread_data[i])) != 0) {
            handle_error_n("pthread_create %d\n", e);
        }
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        if ((e = pthread_join(threads[i], NULL)) != 0) {
            handle_error_n("pthread_join %d\n", e)
        }
    }
    free(threads);
    return 0;
}
