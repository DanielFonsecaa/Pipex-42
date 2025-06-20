<div align="center">

![pipexm](https://github.com/user-attachments/assets/6ac48d02-1374-4a69-92c0-ada8e9cd82d4)

</div>

# Pipex-42

Pipex is a project that recreates the shell’s ability to execute commands connected by pipes (`|`), handling input/output redirection and process management. It demonstrates how Unix-like systems use processes and file descriptors to build command pipelines.

<div align="center">

| Grade | Evaluation Information |
| :---- | :--------------------- |
| <img src="https://img.shields.io/badge/125%20%2F%20100%20%E2%98%85-success"/> | `3 peers` `30 mins` |

</div>

## Key Concepts and System Calls

### What is a Process?

A process is an instance of a running program. Each process has its own memory space and file descriptors. In Unix, processes can create child processes, communicate via pipes, and execute new programs.

### `fork()`

`fork()` creates a new process (child) that is a copy of the calling process (parent). After `fork()`, both processes continue running independently. The child receives a return value of 0, while the parent receives the child’s PID.

### `execve()`

`execve()` replaces the current process image with a new program. It is used after `fork()` in the child process to run a new command. If `execve()` succeeds, the original program code is replaced; if it fails, the child can handle the error.

### `pipe()`

`pipe()` creates a unidirectional data channel (pipe) that can be used for inter-process communication. It returns two file descriptors: one for reading and one for writing. Data written to the write end can be read from the read end.

### `dup2()`

`dup2(oldfd, newfd)` duplicates a file descriptor, making `newfd` refer to the same resource as `oldfd`. This is used to redirect standard input/output, for example, to make a process read from a pipe instead of the keyboard, or write to a file instead of the terminal.

### `access()`

`access()` checks a file’s accessibility (existence, permissions) before trying to execute or open it. It helps ensure that a command or file can be used as expected.

### `wait()` and `waitpid()`

`wait()` and `waitpid()` are used by the parent process to wait for child processes to finish. `waitpid()` can wait for a specific child, and both return the exit status of the child, allowing the parent to know if the command succeeded.

## How Pipex Works

1. **Parsing Arguments:**  
   The program receives command-line arguments specifying input/output files and commands to execute.

2. **Setting Up Pipes:**  
   For N commands, N-1 pipes are created to connect the output of one command to the input of the next.

3. **Forking Processes:**  
   For each command, the program forks a child process. Each child sets up its input/output using `dup2()` to connect to the appropriate file or pipe.

4. **Executing Commands:**  
   In each child, `execve()` is called to run the command. If it fails, an error is reported.

5. **Closing File Descriptors:**  
   Unused file descriptors are closed in both parent and child processes to prevent leaks and ensure proper pipe behavior.

6. **Waiting for Children:**  
   The parent process waits for all child processes to finish using `waitpid()`, collects their exit statuses, and cleans up resources.

## Why These System Calls?

- **`fork()`**: To create separate processes for each command in the pipeline.
- **`execve()`**: To run the actual shell commands in the child processes.
- **`pipe()`**: To connect the output of one command to the input of the next.
- **`dup2()`**: To redirect standard input/output to files or pipes as needed.
- **`access()`**: To check if a command exists and is executable before trying to run it.
- **`wait()`/`waitpid()`**: To synchronize the parent with its children and retrieve their exit statuses.

## Visual

Here’s a simple visual representation of how the pipeline works with three commands:

![Pipeline Diagram](https://github.com/user-attachments/assets/7934e00e-13a6-48c8-97f3-04e3dd96d790)
