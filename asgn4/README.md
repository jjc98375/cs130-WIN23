# Assignment 4 - Httpserver with multi-threads

This repository is a development of httpserver that has capacity to accepts multiple threads.
All the code is written by Joshua Cho. 

## Design decision

The prototyping of this code required some understanding with the starter code from assignment 2 and 3. Assignment 2 is a code structure that only accepts the single thread, and listen through socket. Assignment 3 is a code implementation of Queue by using semaphore, mutex, and conditional variable. 

Main function covers the overall implementation where it creates multiple thread using pthread, and send each thread to worker thread using dispatcher thread. Dispatcher thread using infinite while loop and listen to the new incoming connection through socket. After receiving the socket, it push to the global queue. Worker thread listens to the queue and soon as it available to pop, it pops the element. Each element contains the connection, the socket fd, and send it to handlConnection().

handleConnection() is basically a function that handles handleGet, handlePut, and handleUnsupported functions. Those functions are handled by handleConnection and handleConnection using conn_parse() and evaluate the returned request type. By this requestType, it calls the handle function accordingly. All function send response accordingly. Furthermore, there is another functioanlity that keeps the log of request called 'audit log'. 

Overall function is designed to not terminated unless termination command receives.
