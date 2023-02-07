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

    char* method = (char*)malloc(10);
    retrieveMethod(socketfd, method, &statusCode);
    if(statusCode != 1000) {
        send_response(socketfd, statusCode);
    }

    char* URL = (char*)malloc(70);
    retrieveURL(socketfd, URL, &statusCode);
    if(statusCode != 1000) {
        send_response(socketfd, statusCode);
    }

    retrieveHTTP(socketfd, &statusCode);  //we read until the first \r\n
    if(statusCode != 1000) {
        send_response(socketfd, statusCode);
    }

    int contentLengthValue = parseHeader(socketfd, &statusCode);
    //if it was PUT but no contentLegnth? invalid command
    if (strncmp(method, "GET", 3) != 0 && contentLengthValue == -1) {
        statusCode = 400;
    }
    if(statusCode != 1000) {
        send_response(socketfd, statusCode);
    }


    char messageBody[contentLengthValue+1];
    read(socketfd, messageBody, contentLengthValue);
    messageBody[contentLengthValue] = 0;

    if(strncmp(method, "GET", 3) == 0) {
        int fd = open(URL, O_RDONLY, 0777);

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
        } else {
            struct stat st;
            if (stat(URL, &st) == -1) {
                perror("stat");
            }

            // printf("File size: %ld\n", st.st_size);
            pass_bytes(fd, socketfd, st.st_size);
            statusCode = 200;
        }
        close(fd);

    } 
    else {

        int fdTruncate = open(URL, O_TRUNC | O_WRONLY, 0777);
        //file try to truncate it 
        if(fdTruncate == -1) {
            //if file open failed because the file was not exist at all, then attempt to create it
            if(errno == ENOENT) {
                int fdCreate = open(URL, O_CREAT | O_WRONLY, 0777);
                if(fdCreate == -1) {
                    statusCode = 500;
                } else {
                    write_all(fdCreate, messageBody, contentLengthValue);
                    statusCode = 201;
                }
                close(fdCreate);
            } 
            else {
                statusCode = 500;
            }
        } 
        else {
            write_all(fdTruncate, messageBody, contentLengthValue);
            statusCode = 200;
        }
        close(fdTruncate);
    }

    send_response(socketfd, statusCode);


    free(method);
    free(URL);

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
            doWork(socketfd);
        }

        close(socketfd);
    }

    return EXIT_SUCCESS;
}
