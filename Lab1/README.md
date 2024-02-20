## UID: 105753560

## Pipe Up

In this lab, I wrote low-level C code to emulate the functionality of the pipe (|) operator in shells, allowing users to pass executable names as command-line arguments and executing each one in a new process, simulating behavior similar to a shell.

## Building

Assuming you have the Makefile and pipe.c file, you can simply run the make command to build the program.

## Running

Here is an example of me running my program:

./pipe ls cat du sort wc

This outputs:

3 6 33

This is what the commands do:

ls: Lists the files and directories in the current directory.

cat: Reads the output of ls and outputs it unchanged. In this context, cat is redundant because ls already outputs to stdout.

du: Estimates file space usage. When receiving input from a pipe, it doesn't behave as expected since it expects a file or directory name as an argument, not a stream of data. It will run on the current directory by default, not on the piped input.

sort: Sorts lines of text. In this case, it will sort lines lexicographically.

wc: Counts lines, words, and bytes. It will process the sorted output from sort and give a count of the number of lines, words, and bytes.

## Cleaning up

Because the Makefile is already made, you can simply run the make clean command to clean up the binary files.