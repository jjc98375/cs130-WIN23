// Asgn 2: A simple HTTP server.
// By: Eugene Chou
//     Andrew Quinn
//     Brian Zhao

#include "asgn2_helper_funcs.h"
#include "connection.h"
#include "debug.h"
#include "response.h"
#include "request.h"
#include "queue.h"

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>

#include <pthread.h>
#include <semaphore.h>
#include <sys/file.h>

//constants
#define NUM_THREAD 4 //default number of threads

//functions
void handle_connection(int);
void handle_get(conn_t *);
void handle_put(conn_t *);
void handle_unsupported(conn_t *);

//static declare
static queue_t *Q;
pthread_mutex_t mutex;

//audit function
void audit(const char *method, const char *uri, uint16_t statusCode, int thread_id) {
    fprintf(stderr, "%s,%s,%hu,%d\n", method, uri, statusCode, thread_id);
}

//WORKER THREAD
void *worker_thread(void *arg) {
    (void) arg;
    // reqThread* reqT = (reqThread *) arg;

    while (1) {

        long connfd = 0;
        if (queue_pop(Q, (void **) &connfd) == 0) {
            errx(EXIT_FAILURE, "queue failed to pop");
        }
        if (connfd != -1) {
            // fprintf(stderr, "you connected it successfully\n");

            handle_connection(connfd);
        }
    }

    return NULL;
}

int main(int argc, char **argv) {

    int num_of_thread = 4;
    int port;
    int opt;

    while ((opt = getopt(argc, argv, "t:")) != -1) {
        switch (opt) {
        case 't': num_of_thread = atoi(optarg); break;
        default: fprintf(stderr, "Usage: %s [-t threads] <port>\n", argv[0]); exit(EXIT_FAILURE);
        }
    }

    if (optind != argc - 1) {
        fprintf(stderr, "Usage: %s [-t threads] <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[optind]);

    printf("Threads: %d\n", num_of_thread);
    printf("Port: %d\n", port);

    Q = queue_new(num_of_thread); //initialize the Queue that contains each new connection

    pthread_t threads[num_of_thread];

    // reqThread **thread_pool = (reqThread **) malloc(sizeof(reqThread *) * num_of_thread);

    for (int i = 0; i < num_of_thread; i++) {

        // thread_pool[i] = (reqThread *) malloc(sizeof(reqThread));
        // thread_pool[i]->thread_id = 0;
        // thread_pool[i]->thread = (pthread_t *) malloc(sizeof(pthread_t));
        fprintf(stderr, "thread %d\n", i);

        if (pthread_create(&threads[i], NULL, worker_thread, NULL) != 0) {
            errx(EXIT_FAILURE, "pthread_create() failed");
        }
    }

    //DISPATCH
    signal(SIGPIPE, SIG_IGN);

    Listener_Socket sock;
    listener_init(&sock, port); //accept a connection

    while (1) {

        long connfd = listener_accept(&sock); //gives a fd for each 'valid' connection
        fprintf(stderr, "here\n");

        if (connfd == -1) {
            errx(EXIT_FAILURE, "wasn't able to connect it");
        }

        // queue_push(Q, &connfd);

        if (queue_push(Q, (void *) connfd)) {
            fprintf(stderr, "queue pushed\n");
            fprintf(stderr, "your before queue push connfd is %ld\n", connfd);
        };
    }

    //FINISH THE THREADS
    for (int i = 0; i < num_of_thread; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("All threads have finished!\n");

    return EXIT_SUCCESS;
}

void handle_connection(int connfd) {

    conn_t *conn = conn_new(connfd);
    const Response_t *res = conn_parse(conn);

    if (res != NULL) {
        conn_send_response(conn, res);
    } else {
        debug("%s", conn_str(conn));

        const Request_t *req = conn_get_request(conn);
        if (req == &REQUEST_GET) {
            handle_get(conn);
        } else if (req == &REQUEST_PUT) {
            handle_put(conn);
        } else {
            handle_unsupported(conn);
        }
    }
    conn_delete(&conn);
}

void handle_get(conn_t *conn) {

    char *chid = conn_get_header(conn, "Request-Id");
    int thread_id = atoi(chid);
    fprintf(stderr, "thread id is %d", thread_id);

    char *uri = conn_get_uri(conn);
    const Response_t *res = NULL; //here you initialized res
    debug("handling get request for %s", uri);

    uint16_t statusCode;

    // What are the steps in here?

    // 1. Open the file.
    // If  open it returns < 0, then use the result appropriately
    //   a. Cannot access -- use RESPONSE_FORBIDDEN
    //   b. Cannot find the file -- use RESPONSE_NOT_FOUND
    //   c. other error? -- use RESPONSE_INTERNAL_SERVER_ERROR
    // (hint: check errno for these cases)!
    int fd = open(uri, O_RDONLY, 0666);
    if (fd < 0) {
        if (errno == EACCES) {
            res = &RESPONSE_FORBIDDEN;
            conn_send_response(conn, res);
            goto out;
        } else if (errno == ENOENT) {
            res = &RESPONSE_NOT_FOUND;
            conn_send_response(conn, res);
            goto out;
        } else {
            res = &RESPONSE_INTERNAL_SERVER_ERROR;
            conn_send_response(conn, res);
            goto out;
        }
    }

    // 2. Get the size of the file.
    // (hint: checkout the function fstat)!
    struct stat s;
    fstat(fd, &s);
    off_t ssize = s.st_size;

    // if (fstat(fd, &file_stat) != 0) {
    //     conn_send_response(conn, &RESPONSE_INTERNAL_SERVER_ERROR);
    //     close(fd);
    //     return;
    // }

    // 3. Check if the file is a directory, because directories *will*
    // open, but are not valid.
    // (hint: checkout the macro "S_IFDIR", which you can use after you call fstat!)
    if (S_ISDIR(s.st_mode)) {
        res = &RESPONSE_FORBIDDEN;
        conn_send_response(conn, res);
        close(fd);
        goto out;
    }

    // 4. Send the file
    // (hint: checkout the conn_send_file function!)
    res = &RESPONSE_OK;
    conn_send_response(conn, res);
    conn_send_file(conn, fd, ssize);
    close(fd);

out:
    // 5. audit log
    statusCode = response_get_code(res);
    audit("GET", uri, statusCode, thread_id);
}

//FLOCK
void handle_put(conn_t *conn) {
    char *chid = conn_get_header(conn, "Request-Id");
    int thread_id = atoi(chid);
    fprintf(stderr, "thread id is %d\n", thread_id);

    uint16_t statusCode;

    char *uri = conn_get_uri(conn);
    const Response_t *res = NULL; //here you initialized res
    debug("handling put request for %s", uri);

    // Check if file already exists before opening it.
    bool existed = access(uri, F_OK) == 0;
    debug("%s existed? %d", uri, existed);

    // Acquire the file creation lock
    pthread_mutex_lock(&mutex);

    //main part
    int fd = -1;
    if (!existed) { //not exist
        fd = open(uri, O_CREAT | O_WRONLY | 0600);
        if (fd < 0) {
            debug("%s: %d", uri, errno);
            res = &RESPONSE_INTERNAL_SERVER_ERROR;
            goto out;
        }
        //initiate flock

        // struct flock lock;
        // lock.l_type = F_WRLCK;
        // lock.l_whence = SEEK_SET;
        // lock.l_start = 0;
        // lock.l_len = 0;

        flock(fd, LOCK_EX);
        //do the work
        res = conn_recv_file(conn, fd);
        if (res == NULL) {
            res = &RESPONSE_CREATED;
        }
    } else { //exist
        fd = open(uri, O_TRUNC | 0600);
        if (fd < 0) {
            debug("%s: %d", uri, errno);
            if (errno == EACCES) {
                res = &RESPONSE_FORBIDDEN;
                goto out;
            } else {
                res = &RESPONSE_INTERNAL_SERVER_ERROR;
                goto out;
            }
        }
        //do the work
        res = conn_recv_file(conn, fd);
        if (res == NULL) {
            res = &RESPONSE_OK;
        }
    }

    // Close the file and also unlock the flock if it was exist
    close(fd);

out:
    // Release the file creation lock
    pthread_mutex_unlock(&mutex);

    //send the response to client and also send the audit log to stderr
    conn_send_response(conn, res);
    statusCode = response_get_code(res);
    audit("PUT", uri, statusCode, thread_id);
}

void handle_unsupported(conn_t *conn) {
    debug("handling unsupported request");
    // send responses
    conn_send_response(conn, &RESPONSE_NOT_IMPLEMENTED);
}
