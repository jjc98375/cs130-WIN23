# Hello World Program for CSE 130: Principles of Computer Systems Design

## Introduction
This repository contains a simple "Hello World" program written in C, as part of the assignment for CSE 130: Principles of Computer Systems Design. The purpose of this assignment is to familiarize students with the development environment, submission practices, and basic programming in C.

## Getting Started
To get started with this project, you will need to have an Ubuntu 22.04 virtual machine set up on your local machine. This will allow you to test your code in an environment that matches our grading setup.

### Installation
1. **Set up Ubuntu 22.04 VM:** Follow the instructions provided in the "Setting up Ubuntu" document on Canvas to create and configure your Ubuntu 22.04 virtual machine.
2. **Clone the repository:** Once your VM is set up, clone this repository to your local environment within the VM.
    ```bash
    git clone <repository-url>
    cd <repository-name>
    ```

### Building the Project
This project uses a Makefile for easy building of the hello binary.

- To build the hello binary, run:
    ```bash
    make all
    ```

- To clean the project (remove object files and binaries), run:
    ```bash
    make clean
    ```

### Running the Program
After building the project, you can run the program by executing the `hello` binary:

```bash
./hello
