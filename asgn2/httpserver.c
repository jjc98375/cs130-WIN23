#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "asgn2_helper_funcs.h"
#include "module.h"

void doWork(int socketfd) {

    int statusCode = 1000;

    char method[10];
    retrieveMethod(socketfd, method, &statusCode);
    if (statusCode == 501) {
        send_response(socketfd, statusCode);
        return;
    }

    char realURL[70]; // foo.txt
    retrieveURL(socketfd, realURL, &statusCode);

    retrieveHTTP(socketfd, &statusCode); //we read until the first \r\n
    if (statusCode == 505) {
        send_response(socketfd, statusCode);
        return;
    }

    int contentLengthValue = parseHeader(socketfd, &statusCode);
    //if it was PUT but no contentLegnth? invalid command
    if (strncmp(method, "GET", 3) != 0 && contentLengthValue == -1) {
        statusCode = 400;
    }

    //if no 501 and 505 and then no 400, then
    if (statusCode != 1000) {
        send_response(socketfd, statusCode);
        return;
    }

    char messageBody[contentLengthValue + 1];
    read(socketfd, messageBody, contentLengthValue);
    messageBody[contentLengthValue] = 0;

    if (strncmp(method, "GET", 3) == 0) {
        int fd = open(realURL, O_RDONLY, 0666);

        struct stat ps;
        stat(realURL, &ps);
        if (S_ISDIR(ps.st_mode) == 1) {
            statusCode = 403;
            send_response(socketfd, statusCode);
            return;
        }

        if (fd == -1) {

            if (errno == ENOENT) {
                //404
                statusCode = 404;

                // fprintf(stderr, "File file.txt does not exist\n");
                // exit(EXIT_FAILURE);
            }

            else if (errno == EACCES) {
                //403
                statusCode = 403;
                // fprintf(stderr, "Permission denied when trying to open file.txt\n");
                // exit(EXIT_FAILURE);
            }
            send_response(socketfd, statusCode);
        } else {

            struct stat st;
            if (stat(realURL, &st) == -1) {
                perror("stat");
            }

            int file_length = st.st_size;

            char buf[100];
            memset(buf, 0, 100);
            sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", file_length);
            int lengthOfBuf = strlen(buf);

            write_all(socketfd, buf, lengthOfBuf);
            pass_bytes(fd, socketfd, file_length);

            // int read_more;
            // char inner_buf[BUF_SIZE];
            // do {
            //     read_more = read(fd, inner_buf, BUF_SIZE);
            //     if (read_more > 0) {
            //         write(socketfd, inner_buf, read_more);
            //     }
            // } while (read_more > 0);
        }
        close(fd);

    } else {

        int fdTruncate = open(realURL, O_TRUNC | O_WRONLY, 0777);
        //file try to truncate it
        if (fdTruncate == -1) {
            //if file open failed because the file was not exist at all, then attempt to create it
            if (errno == ENOENT) {
                int fdCreate = open(realURL, O_CREAT | O_WRONLY, 0777);
                if (fdCreate == -1) {
                    statusCode = 500;
                } else {
                    write_all(fdCreate, messageBody, contentLengthValue);
                    statusCode = 201;
                }
                close(fdCreate);
            } else {
                statusCode = 500;
            }
        } else {
            write_all(fdTruncate, messageBody, contentLengthValue);
            statusCode = 200;
        }

        send_response(socketfd, statusCode);

        close(fdTruncate);
    }
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

        if (socketfd < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                fprintf(stdout, "Timeout \n");
            }
        }

        doWork(socketfd);
        close(socketfd);
    }

    return EXIT_SUCCESS;
}
