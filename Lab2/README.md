# You Spin Me Round Robin

The project involved implementing round-robin scheduling for a given workload and quantum length in C. The skeleton code provided a basic framework for parsing input files and command-line arguments. The main task was to add functionality within the designated section of the code. I implemented the round-robin scheduling algorithm within this section, ensuring calculations for total waiting time and total response time. The program outputs the average waiting time and response time.

## Building

```shell
Assuming you have the Makefile and rr.c file, you can simply run the make command to build the program:

make
```

## Running

cmd for running TODO
```shell
./rr "file_name" quantum_size

Using the example file given:
./rr processes .txt 3
```

results TODO
```shell

There were 2 separate ways to test our code - the given processes.txt file and test_lab2.py file.

Results from running python -m unittest:

Ran 2 tests in 0.329s

OK


Results from running ./rr processes.txt 3:

Average waiting time: 7.00
Average response time: 2.75

```

## Cleaning up

```shell
Because the Makefile is already made, you can simply run the make clean command to clean up the binary files:

make clean
```
