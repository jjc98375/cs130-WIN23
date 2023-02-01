#pragma once

#include "asgn2_helper_funcs.h"
#include <stdbool.h>

#define VALUE_SIZE 2048

void send_response(int socketfd, int code);
void response(int socketfd);

// typedef struct Request {

//     // int socket;

//     // int method; // Method
//     // char path[100]; // URI
//     // char version[100]; // Version

//     // char hostvalue[100];

//     // unsigned long int cnt_len; // Content-Length

//     // bool has_mtd;
//     // bool has_len;

//     // char msg_bdy[VALUE_SIZE]; // Message-Body

//     // int read_len;

// } Request;

// typedef struct {

//     int in_fd; //do we need this?

//     char* method;
//     char* URI;
//     bool is_valid_version;
//     int content_length;
//     char* message_body;

// } Request;

// typedef struct {

// } status_code;

// typedef struct {
//     bool is_valid_version;
//     status_code code;

// } Response;
