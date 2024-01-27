# A Kernel Seedling
In project 0, the task was to set up a virtual machine and create a kernel module that adds a file to "/proc/" to access internal kernel information. The primary objective was to edit the proc_count.c file so that it displays the current number of running processes in the kernel.

## Building
```shell
make
```
After editing the proc_count.c file, I ran the make command to build out some necessary files including proc_count.ko, which was needed to load into the kernel.

## Running
```shell
cat /proc/count
```
results: 137

After building and loading the kernel module into the linux kernel, I ran cat /proc/count, which reads the content of the /proc/count file. This correctly displayed the number of running processes.

## Cleaning Up
```shell
make clean
```
After printing the number of running processes and removing the kernel module from the linux kernel, I ran make clean to remove all files added by running make.

## Testing
```python
python -m unittest
```
The following was printed after I ran the testing command:

Ran 3 tests in 13.537s

OK

This demonstrated my code passed the given test cases.

Report which kernel release version you tested your module on
(hint: use `uname`, check for options with `man uname`).
It should match release numbers as seen on https://www.kernel.org/.

```shell
uname -r -s -v
```
Running this command, I found the kernal release version.

kernel ver: Linux 5.14.8
