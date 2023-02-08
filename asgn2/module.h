#pragma once

#include "asgn2_helper_funcs.h"
#include <stdbool.h>

#define BUF_SIZE      4096
#define IDEAL_REQUEST 2048

const char *Phrase(int code);
//return 400 if theres issue else return 0
int check_validity_of_character(const char *s);
void send_response(int socketfd, int code);

void readAndParse(int socketfd);
void retrieveMethod(int socketfd, char *method, int *statusCode);
void retrieveURL(int socketfd, char *URL, int *statusCode);
void retrieveHTTP(int socketfd, int *statusCode);
int parseHeader(int socketfd, int *statusCode);
void readTheRest(int socketfd);
