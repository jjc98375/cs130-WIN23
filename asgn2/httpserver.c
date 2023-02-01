#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <err.h>

#include "asgn2_helper_funcs.h"
#include "module.h"

void HandleConnection(int socketfd) {

    // echo steps
    char buf[4096];
    int readd = 0;
    readd = read_until(socketfd, buf, 4096, "\r\n");
    write_all(1, buf, readd);
    write_all(socketfd, buf, readd);

    // response steps
    response(socketfd);
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        errx(EXIT_FAILURE, "wrong arguments: %s port_num", argv[0]);
    }

    int port = atoi(argv[1]);
    if (port < 1 || port > 65535) {
        // write(2, "Invalid Port\n", sizeof("Invalid Port\n"));
        // return 1;

        fprintf(stderr, "Invalid Port\n");
        exit(EXIT_FAILURE);
    }

    Listener_Socket socket;

    //initiate the socket
    int listen = listener_init(&socket, port);
    if (listen != 0) {
        write(2, "Invalid listener init value, couldn't make a valid request\n",
            sizeof("Invalid listener init value, couldn't make a valid request\n"));
        return 1;
    }

    while (1) {
        int socketfd = listener_accept(&socket);

        // if(socketfd == -1) {
        //   printf("Invalid accept, couldn't create a socket\n");
        //   return 1;
        // }

        if (socketfd > 0) {
            HandleConnection(socketfd);
        }

        close(socketfd);
    }

    return EXIT_SUCCESS;
}
