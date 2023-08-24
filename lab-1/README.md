# Operating System Lab 1

Welcome to the Operating System Lab 1 project! This repository contains the code and documentation for the lab exercise as part of the Operating System course.

## Table of Contents

- [Introduction](#introduction)
- [Technologies](#technologies)
- [Features](#features)
- [Getting Started](#getting-started)
- [Usage](#usage)

## Introduction

Operating System Lab 1 is a hands-on project that provides practical insights into fundamental concepts of operating systems. The project focuses on process management, memory allocation, and basic system interactions.

## Technologies

The project is implemented using:
- C programming language
- GNU Compiler Collection (GCC)
- POSIX Standard Libraries
## Features

- Internal shell commands
- Execution of commands with and without arguments
- Background execution using '&'

## Getting Started

Follow these steps to get a copy of the project on your local machine:

1. Clone the repository:
   ```sh
   git clone https://github.com/RewanAbdulkariem/Operating-System-labs.git

Sure, here's how you can expand on the "Usage" section of your README to explain how to compile and run the project using GCC, as well as provide examples of command usage:

## Usage

### Compilation

To compile the project using GCC, follow these steps:

1. Open a terminal window.
2. Navigate to the project directory:
   ```sh
   cd /path/to/Operating-System-labs/lab-1
3. Compile the source code using GCC:
   ```sh
   gcc -o my_shell main.c -Wall -Wextra

4. Running the Shell
Once the project is compiled, you can run the shell using the following command:
   ```sh
   ./my_shell

## Command Usage
The shell supports various commands similar to a standard Unix shell. Here are a few examples:

### Internal Shell Command:
1. This will display a list of supported commands and their usage.
   ```sh
   help


### Command with Arguments:
2. This will list files and directories in long format.
   ```sh
   ls -l

### Background Execution:
3. This will run the script my_script.sh in the background.
   ```sh
   ./my_script.sh &


