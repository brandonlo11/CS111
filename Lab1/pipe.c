#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    // Check if there are any shell commands provided.
    if (argc <= 0) {
        printf("Error: No shell commands provided.\n");
        exit(22);
    }

    // If only the program name is provided, no piping is needed.
    if (argc == 1) {
        fprintf(stderr, "No shell commands provided.\n");
        exit(22);
    }

    // Iterate through each provided shell command for execution and piping.
    for (int commandIndex = 1; commandIndex < argc; commandIndex++) {
        int pipeFileDescriptors[2]; // Array to hold file descriptors for the pipe

        // Create a pipe for all commands except the last one.
        if (commandIndex < argc - 1) {
            if (pipe(pipeFileDescriptors) == -1) {
                perror("Failed to create a pipe");
                exit(errno);
            }
        }

        int childProcessID = fork(); // Create a child process
        if (childProcessID == -1) {
            perror("Fork failed");
            exit(errno);
        }

        // Child process execution.
        if (childProcessID == 0) {
            if (commandIndex < argc - 1) {
                // Redirect standard output to the write-end of the pipe for all but the last command.
                dup2(pipeFileDescriptors[1], STDOUT_FILENO);
                close(pipeFileDescriptors[0]); // Close the read-end of the pipe in the child process.
                close(pipeFileDescriptors[1]); // Close the write-end of the pipe in the child process.
            }

            // Execute the command.
            if (execlp(argv[commandIndex], argv[commandIndex], (char *)NULL) == -1) {
                perror("Failed to execute command");
                exit(errno);
            }
        }
        // Parent process execution.
        else {
            int childStatus;
            close(pipeFileDescriptors[1]); // Close the write-end of the pipe in the parent.

            waitpid(childProcessID, &childStatus, 0);

            if (!WIFEXITED(childStatus) || WEXITSTATUS(childStatus) != 0) {
                fprintf(stderr, "Error: Child process terminated abnormally or exited with an error.\n");
                exit(WEXITSTATUS(childStatus));
            }

            // Redirect standard input to the read-end of the pipe.
            dup2(pipeFileDescriptors[0], STDIN_FILENO);
            close(pipeFileDescriptors[0]); // Close the read-end of the pipe in the parent.
        }
    }
    return 0;
}
