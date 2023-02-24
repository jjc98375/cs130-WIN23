# Assignment 3 - Queue Implementation in C with Semaphores and Mutexes


This repository contains a C implementation of a queue data structure that uses semaphores and mutexes to enable thread-safe operations. The code consists of three functions: queue_new(), queue_push(), and queue_pop(), as well as a queue_delete() function for cleanup.

## Design decision

To initialize the queue, I created one mutex and two semaphore to implement the this thread-safe queue data structure. A single mutex allows us to keep the push and pop method to safely guard the thread access. The two semaphore were builted because a single semaphore only can detect when the semaphore is empty. By implementing two semaphore for one indicating the empty status and one with full status, I was able to restrict both empty and full condition of queue.

Other design decisions were to make the queue a generic data structure by using a void ** array. This allows the user to store any type of data in the queue, as long as it is passed as a pointer. Another decision was to use dynamic memory allocation to allow for variable queue sizes.


  

## High-Level Functions


### Design of the queue data structure

```

typedef struct queue {

	void **array;
	int in;
	int out;
	int size;

	sem_t full, empty;
	pthread_mutex_t lock;

  
} queue_t;

```

The queue had 6 different attributes:
- array has a type of void** where it contains the actual queue that have elements with type of void*
- in - back of the queue, out - front of the queue
- size - size of the array
- full is a semaphore indicates full. It sem_wait() when queue_push() and sem_post() when queue_pop()
- empty is a semaphore indicates empty. It sem_wait() when queue_pop() and sem_post() when queue_push().
- lock is a mutex that uses for every pop or push. 


### Methods


queue_new(int size)

Creates a new queue with the given size. Returns a pointer to the newly created queue_t structure.

queue_push(queue_t _q, void_ elem)

Adds an element to the back of the queue. Blocks if the queue is full. Returns true if successful, false otherwise.

queue_pop(queue_t *q, void **elem)

Removes an element from the front of the queue. Blocks if the queue is empty. Returns true if successful, false otherwise.

queue_delete(queue_t **q)

Deletes the given queue and frees all associated memory.