#include "module.h"
#include "asgn2_helper_funcs.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <err.h>
#include <string.h>

//helps to get the right phrase given the status code
const char *Phrase(int code) {
    switch (code) {

    case 200: return "OK";
    case 201: return "Created";
    case 400: return "Bad Request";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 500: return "Internal Server Error";
    case 501: return "Not Implemented";
    case 505: return "Version Not Supported";
    }
    return NULL;
}

void send_response(int socketfd, int code) {

    char buf[VALUE_SIZE];
    const char *phrase = Phrase(code);

    sprintf(buf, "HTTP/1.1 %d %s\r\nContent-Length: %d\r\n\r\n%s\n", code, phrase,
        (int) strlen(phrase), phrase);
    write_all(socketfd, buf, strlen(buf));
}

void response(int socketfd) {

    // if(get && code == 200) {
    //   char buf[2048];

    // } else {

    //   send_response(socketfd, 200);
    //   send_response(socketfd, 201);
    //   send_response(socketfd, 400);
    //   send_response(socketfd, 403);
    //   send_response(socketfd, 404);
    //   send_response(socketfd, 500);
    //   send_response(socketfd, 501);
    //   send_response(socketfd, 505);

    // }

    //
    send_response(socketfd, 200);
    send_response(socketfd, 201);
    send_response(socketfd, 400);
    send_response(socketfd, 403);
    send_response(socketfd, 404);
    send_response(socketfd, 500);
    send_response(socketfd, 501);
    send_response(socketfd, 505);
}
