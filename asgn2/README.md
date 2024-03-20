### README for Assignment 2: HTTP Server

```markdown
# Assignment 2: HTTP Server

## Overview

This assignment tasked me with creating a basic HTTP server in C. The server supports GET and PUT requests, demonstrating a deeper understanding of the HTTP protocol, client-server architecture, and advanced string and memory management techniques.

## Objective

The primary goal was to implement a server that could handle simple HTTP requests from clients, process those requests to either retrieve or store data, and respond according to the HTTP/1.1 protocol standards.

## Key Features

- **HTTP GET and PUT Support:** Ability to handle GET requests for retrieving file content and PUT requests for storing content in a file.
- **Connection Handling:** Manages TCP connections, accepting incoming requests, and processing each in sequence.
- **Robust Error Handling:** Implements comprehensive error checking and response codes to handle various failure scenarios gracefully.
- **Logging:** Basic logging functionality to track requests and server actions.

## Implementation Details

- **Language:** The server is implemented in C, utilizing socket programming for network communication.
- **Concurrency:** Processes requests sequentially, ensuring simplicity in handling HTTP transactions and state management.
- **Security Considerations:** Includes checks to mitigate potential risks such as directory traversal attacks.
- **Efficiency:** Utilizes efficient parsing and memory management techniques to handle HTTP requests and responses.

## Usage

Compile the server using the Makefile, then start it by specifying a port number. Here is an example command to run the server:
