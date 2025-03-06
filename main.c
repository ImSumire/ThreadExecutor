#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


typedef struct ThreadExecutor {
    int count;
    pthread_t* threads;
} ThreadExecutor;

ThreadExecutor* te_run(void* (*funcs[])(void*), int count) {
    pthread_t* threads = malloc(count * sizeof(pthread_t));

    int* thread_id;
    for (int i = 0; i < count; i += 1) {
        thread_id = malloc(sizeof(int));
        *thread_id = i;

        int rc = pthread_create(&threads[i], NULL, funcs[i], thread_id);
        if (rc) {
            printf("Error creating thread %d: %d\n", i, rc);
            return NULL;
        }
    }

    ThreadExecutor* te = malloc(sizeof(ThreadExecutor));
    te->count = count;
    te->threads = threads;

    return te;
}

void te_join(ThreadExecutor* te) {
    for (int i = 0; i < te->count; i += 1) {
        pthread_join(te->threads[i], NULL);
    }
}

void te_drop(ThreadExecutor* te) {
    free(te->threads);
    free(te);
}


/* Usage example */
float randfloat(float _min, float _max) {
    return (float)rand() / RAND_MAX * (_max - _min) + _min;
}

void* foo(void* arg) {
    int id = *(int*)arg;

    float delay = randfloat(0.0, 5.0);

    usleep(delay * 1000000);
    printf("%d finished after %f seconds\n", id, delay);
}

int main() {
    srand(time(NULL));

    ThreadExecutor* te = te_run(
        (void* (*[])(void*)) {
            foo, foo, foo, foo, foo, foo, foo, foo, foo, foo, foo, foo, foo,
            foo, foo, foo, foo, foo, foo, foo, foo, foo, foo, foo, foo, foo
        },
        26
    );
    te_join(te);
    te_drop(te);

    return 0;
}
