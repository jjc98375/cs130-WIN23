#include "module.h"
#include "asgn2_helper_funcs.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>

//maybe use 0666

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

int check_validity_of_character(const char *s) {
    for (unsigned long i = 1; i < strlen(s); i++) {
        if (s[i] != '.' && s[i] != '-' && isalnum(s[i]) == 0) {
            //400
            return 400;
            // fprintf(stderr, "invalid characters on it\n");
            // exit(EXIT_FAILURE);
        }
    }
    //valid
    return 0;
}

void send_response(int socketfd, int code) {
    char buf[100];
    memset(buf, 0, 100);

    const char *phrase = Phrase(code);
    sprintf(buf, "HTTP/1.1 %d %s\r\nContent-Length: %d\r\n\r\n%s\n", code, phrase,
        (int) strlen(phrase) + 1, phrase);
    write_all(socketfd, buf, strlen(buf));
}

void readTheRest(int socketfd) {
    int read_more;
    char inner_buf[BUF_SIZE];
    do {
        read_more = read(socketfd, inner_buf, BUF_SIZE);
    } while (read_more == BUF_SIZE);
}

void retrieveMethod(int socketfd, char *method, int *statusCode) {

    char buf[1];
    int index = 0;
    int bytes_read;
    do {
        if (index == 9) {
            // fprintf(stderr, "character out of range from method from request line\n");
            method[index] = buf[0];
            if (400 < *statusCode) {
                *statusCode = 400;
            }
            readTheRest(socketfd);
            return;
        }
        bytes_read = read(socketfd, buf, 1);
        if (bytes_read == -1) {
            //read failed
            *statusCode = 500;
            return;
        } else {
            if (bytes_read == 1) {
                if (buf[0] == ' ') {
                    method[index] = 0;
                    break;
                } else {
                    method[index] = buf[0];
                }
                index++;
            }
        }
    } while (bytes_read > 0);

    //check the validity of character
    for (unsigned long i = 0; i < strlen(method); i++) {
        if (((method[i] >= 'a') && (method[i] <= 'z'))
            || ((method[i] >= 'A') && (method[i] <= 'Z'))) {
            continue;
        } else {
            //400
            // fprintf(stderr, "invalid character from method from request line\n");
            if (400 < *statusCode) {
                *statusCode = 400;
            }
            readTheRest(socketfd);
            return;
        }
    }

    //see if it is actual method
    if (strcmp(method, "GET") != 0 && strcmp(method, "PUT") != 0) {
        //501
        // fprintf(stderr, "method was not GET or PUT\n");
        *statusCode = 501;
        readTheRest(socketfd);
        return;
    }
}

void retrieveURL(int socketfd, char *URL, int *statusCode) {

    char buf[1];
    int index = 0;
    int bytes_read;

    bytes_read = read(socketfd, buf, 1);
    if(bytes_read == 1) {
        if(buf[0] != '/') {
            *statusCode = 400;
            readTheRest(socketfd);
            return;
        }
    }

    do {
        if (index == 64) {
            //400
            // fprintf(stderr, "character out of range from URL from request line\n");
            URL[index] = 0;
            if (400 < *statusCode) {
                *statusCode = 400;
                readTheRest(socketfd);
            }
            return;
        }
        bytes_read = read(socketfd, buf, 1);
        if (bytes_read == 1) {
            if (buf[0] == ' ') {
                URL[index] = 0;
                break;
            } else {
                URL[index] = buf[0];
            }
            index++;
        }
    } while (bytes_read > 0);

    if (index < 1) {
        //400
        // fprintf(stderr, "illformated URL\n");
        if (400 < *statusCode) {
            *statusCode = 400;
            readTheRest(socketfd);
        }
        return;
    }

    //check the validity of character
    if (check_validity_of_character(URL) == 400) {
        if (400 < *statusCode) {
            *statusCode = 400;
            readTheRest(socketfd);
        }
        return;
    }
}

//from here you should and read all
//this part is where I'm fucked I think

void retrieveHTTP(int socketfd, int *statusCode) {

    char httpContent[6];
    int bytes_read = read(socketfd, httpContent, 5);
    if (bytes_read < 5 || strncmp(httpContent, "HTTP/", 5) != 0) {
        //400
        // fprintf(stderr, "illformated http itself\n");
        if (400 < *statusCode) {
            *statusCode = 400;
            readTheRest(socketfd);
        }
        return;
    }

    // memmove(http, content, 5);

    char versionContent[10]; //just assumed it has 10 contents
    char buf[1];
    int index = 0;
    do {
        bytes_read = read(socketfd, buf, 1);
        if (bytes_read == 1) {

            if (buf[0] == '\r') {
                read(socketfd, buf, 1);
                versionContent[index] = 0;
            } else {
                versionContent[index] = buf[0];
            }
            index++;
        }
    } while (bytes_read > 0);

    if (strlen(versionContent) != 3) {
        //1.10 is ill formatted
        if (400 < *statusCode) {
            *statusCode = 400;
            readTheRest(socketfd);
        }
        return;
    } else {
        if (strncmp(versionContent, "1.1", 3) != 0) {
            //505
            *statusCode = 505;
            readTheRest(socketfd);
            // fprintf(stderr, "illformated http version\n");
            return;
        }
    }
}

//I really do hope '\n' come after '\r'
int parseHeader(int socketfd, int *statusCode) {

    int nbytes = 128;
    int contentLengthValue = -1;
    // bool endofthepoint = false;

    char key[nbytes];
    char value[nbytes];

    memset(key, 0, nbytes);
    memset(value, 0, nbytes);
    bool validity = false;
    bool contentLength = false;
    int countKey = 0;
    int countValue = 0;

    // bool firstTimeHeader = true;

    char buf[1];
    while (1) {

        //check the first read of key
        int firstread = read(socketfd, buf, 1);
        if (firstread == 1) {
            //it reached to the end of the header line
            if (buf[0] == '\r') {
                read(socketfd, buf, 1);

                //if it is not get and stopped
                // if (firstTimeHeader && (strncmp(method, "GET", 3) != 0)) {
                //     fprintf(stderr, "Non-GET method have no header, which is false\n");
                //     exit(EXIT_FAILURE);
                // }

                return contentLengthValue;
            } else {
                key[0] = buf[0];
                countKey++;
            }
        } else {
            //400
            if (400 < *statusCode) {
                *statusCode = 400;
                readTheRest(socketfd);
            }
            return contentLengthValue;
            // fprintf(stderr, "less than 1 character for key so ill formated\n");
            // exit(EXIT_FAILURE);
        }

        //read the key parts
        while (countKey < nbytes) {
            int bytes_read = read(socketfd, buf, 1);
            if (bytes_read == 1) {

                if (buf[0] == '\r') {
                    //400
                    if (400 < *statusCode) {
                        *statusCode = 400;
                        readTheRest(socketfd);
                    }
                    return contentLengthValue;

                    // fprintf(stderr, "can't have \r\n before : in key; ill formated\n");
                    // exit(EXIT_FAILURE);
                }

                if (buf[0] == ':') {
                    bytes_read = read(socketfd, buf, 1); // read the empty space
                    if (bytes_read == 1 && buf[0] == ' ') {
                        validity = true;
                        break;
                    }
                    //": " not match
                    else {
                        //400
                        if (400 < *statusCode) {
                            *statusCode = 400;
                            readTheRest(socketfd);
                        }
                        return contentLengthValue;

                        // fprintf(stderr, "the colon and space not came together\n");
                        // exit(EXIT_FAILURE);
                    }
                } else {
                    key[countKey] = buf[0];
                }
            }
            countKey++;
        }
        //out of range
        if (validity == false) {
            //400
            if (400 < *statusCode) {
                *statusCode = 400;
                readTheRest(socketfd);
            }
            return contentLengthValue;

            // fprintf(stderr, "format of key is out of range\n");
            // exit(EXIT_FAILURE);
        }

        //check if there are right characters
        if (check_validity_of_character(key) == 400) {
            if (400 < *statusCode) {
                *statusCode = 400;
                readTheRest(socketfd);
            }
            return contentLengthValue;
        };

        //check if the key is content-length
        if (strncmp(key, "Content-Length", 14) == 0) {
            contentLength = true;
        }

        validity = false;

        //check the first read of value
        firstread = read(socketfd, buf, 1);
        if (firstread == 1) {
            if (buf[0] == '\r') {
                //400
                if (400 < *statusCode) {
                    *statusCode = 400;
                    readTheRest(socketfd);
                }
                return contentLengthValue;

                // fprintf(stderr, "less than 1 character for value so ill formated\n");
                // exit(EXIT_FAILURE);
            } else {
                value[0] = buf[0];
                countValue++;
            }
        } else {
            //400
            if (400 < *statusCode) {
                *statusCode = 400;
                readTheRest(socketfd);
            }
            return contentLengthValue;

            // fprintf(stderr, "less than 1 character for value so ill formated\n");
            // exit(EXIT_FAILURE);
        }

        //read the value parts
        while (countValue < nbytes) {
            int bytes_read = read(socketfd, buf, 1);
            if (bytes_read == 1) {
                if (buf[0] == '\r') {
                    read(socketfd, buf, 1);
                    validity = true;
                    break;
                } else {
                    value[countValue] = buf[0];
                }
            }
            countValue++;
        }

        //out of range
        if (validity == false) {
            //400
            if (400 < *statusCode) {
                *statusCode = 400;
                readTheRest(socketfd);
            }
            return contentLengthValue;

            // fprintf(stderr, "value characters are out of range");
            // exit(EXIT_FAILURE);
        }

        //right characters
        for (unsigned long i = 0; i < strlen(value); i++) {
            //400
            if (value[i] < 33 || value[i] > 126) {
                if (400 < *statusCode) {
                    *statusCode = 400;
                    readTheRest(socketfd);
                }
                return contentLengthValue;

                // fprintf(stderr, "value have a invalid character\n");
                // exit(EXIT_FAILURE);
            }
        }

        //if content-length is exist, store the value of it
        if (contentLength == true) {
            contentLengthValue = atoi(value);
        }

        memset(key, 0, nbytes);
        memset(value, 0, nbytes);

        validity = false;
        contentLength = false;

        countKey = 0;
        countValue = 0;
    }

    fprintf(stderr, "Why the fuck you are outside of while loop from parseHeader\n");
    exit(EXIT_FAILURE);
}
