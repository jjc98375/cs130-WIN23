# Assignment 1: Command-line Memory

## Overview

This assignment involves creating a `memory` program that enables get/set operations on files within a Linux directory, demonstrating proficiency in handling file I/O, memory management, and string parsing in C.

## Objective

The goal is to develop a command-line application that interacts with the file system to retrieve (`get`) or modify (`set`) the contents of files based on user input. This assignment serves as a refresher on basic system programming concepts including Linux system calls and buffering techniques.

## Key Features

- **Get Operation:** Retrieves and displays the content of a specified file to the terminal.
- **Set Operation:** Creates or updates a specified file with given content.
- **Error Handling:** Provides feedback for invalid commands or file operations.

## Implementation Details

- **Language:** Implemented in C, leveraging low-level system calls for file manipulation.
- **Concurrency:** Designed to handle one command at a time, ensuring straightforward usage and testing.
- **Memory Management:** Careful allocation and deallocation of memory to prevent leaks.
- **Security Measures:** Basic checks to prevent unauthorized file access or modification.

## Usage

To use the `memory` program, compile it using the provided Makefile and then run it from the terminal. Here are some examples:


