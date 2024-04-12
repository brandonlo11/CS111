# Hey! I'm Filing Here

In this lab, I successfully implemented the following. I created a 1 MiB ext2 filesystem consisting of two directories, a regular file, and a symbolic link, all within a file named cs111-base.img. I learned about the ext2 filesystem structures and implemented functions to construct the superblock, descriptor table, bitmaps, inode table, and specific content blocks according to given specifications. I also made sure to account for error checking, filesystem integrity, and adherence to ext2 standards.

## Building

Assuming you have the Makefile and ext2-create.c file, you can simply run the make command to build the program:

make

## Running

We can use these shell commands to run the code, as stated in the lab4 pdf:

make # Compiles the source code into an executable program
./ ext2 - create # Executes the program to generate cs111-base.img
dumpe2fs cs111 -base.img #  Provides detailed information about the created filesystem for debugging
fsck.ext2 cs111 -base.img # Verifies the integrity and consistency of the filesystem
mkdir mnt # Creates a new directory intended as the mount point for the filesystem
sudo mount -o loop cs111 -base.img mnt # Mounts the filesystem image to the newly created directory, enabling file system access
sudo umount mnt # Unmounts the filesystem when you 're done, disconnecting it from the directory
rmdir mnt # Remove the directory previously used as the mount point when you 're done

## Cleaning up

Because the Makefile is already made, you can simply run the make clean command to clean up the binary files:

make clean
