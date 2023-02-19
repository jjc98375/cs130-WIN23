#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct queue {
    void **array;
    int in;
    int out;
    int size;

    sem_t full, empty;
    pthread_mutex_t lock;

} queue_t;

//constructor
queue_t *queue_new(int size) {
    queue_t *q = malloc(sizeof(queue_t));
    q->array = (void **) malloc(sizeof(void *) * size);
    q->size = size;

    q->in = 0;
    q->out = 0;

    sem_init(&q->full, 0, q->size);
    sem_init(&q->empty, 0, 0);
    pthread_mutex_init(&q->lock, NULL);

    return q;
}

//destructor
void queue_delete(queue_t **q) {

    if (q == NULL || *q == NULL) {
        return;
    }

    free((*q)->array);

    pthread_mutex_destroy(&((*q)->lock));
    sem_destroy(&((*q)->full));
    sem_destroy(&((*q)->empty));

    free(*q);
    *q = NULL;
}

bool queue_push(queue_t *q, void *elem) {
    if (!q || !elem) {
        return false;
    }
    sem_wait(&q->full);
    pthread_mutex_lock(&q->lock);
    q->array[q->in] = elem;
    q->in = (q->in + 1) % q->size;
    pthread_mutex_unlock(&q->lock);
    sem_post(&q->empty);
    return true;
}

bool queue_pop(queue_t *q, void **elem) {
    if (!q) {
        return false;
    }
    sem_wait(&q->empty);
    pthread_mutex_lock(&q->lock);
    *elem = q->array[q->out];
    q->out = (q->out + 1) % q->size;
    pthread_mutex_unlock(&q->lock);
    sem_post(&q->full);
    return true;
}
