# Hash Hash Hash
In this assignment, I edited the code for a concurrent hash table to make it thread-safe. I was given a serial hash table implementation, then modified two additional versions by implementing two different locking strategies to ensure thread safety. My task involved adding mutex locks to the "add entry" functions without altering the core algorithms. I compared the performance of my implementations with the base version, considering both correctness and speed, and documented my findings and the rationale behind my chosen locking mechanisms.

## Building
```shell
Assuming you have the Makefile and rr.c file, you can simply run the make command to build the program:

make
```

## Running
```shell
./hash-table-tester -t number_of_threads -s number_of_hash_table_entries_to_add_per_thread
(number_of_hash_table_entries_to_add_per_thread is default 25,000)

Using the example given in the spec:
./hash-table-tester -t 8 -s 50000

Test 1 results:

Generation: 74,463 usec
Hash table base: 1,319,317 usec
    - 0 missing
Hash table v1: 1,693,884 usec
    - 0 missing
Hash table v2: 553,996 usec
    - 0 missing

Test 2 results:

Generation: 72,262 usec
Hash table base: 1,042,856 usec
    - 0 missing
Hash table v1: 1,539,152 usec
    - 0 missing
Hash table v2: 363,127 usec
    - 0 missing

Test 2 results:

Generation: 72,269 usec
Hash table base: 1,104,862 usec
    - 0 missing
Hash table v1: 1,439,240 usec
    - 0 missing
Hash table v2: 363,086 usec
    - 0 missing

Using python3 -m unittest:

.Running tester code 1...
.Running tester code 2...
.Running tester code 3...
.

Ran 3 tests in 13.364s

OK
```

## First Implementation
In the `hash_table_v1_add_entry` function, I added code which locks a mutex before attempting to retrieve or change a value in the hash table and unlocks it afterwards to ensure that only one thread can modify the table at a time. This prevents race conditions during concurrent access. The "pthread_mutex_t" mutex is included in the hash table structure for synchronization. Additionally, the mutex is initialized in the "hash_table_v1_create" function and destroyed in "hash_table_v1_destroy".

### Performance
```shell
./hash-table-tester -t number_of_threads -s number_of_hash_table_entries_to_add_per_thread
(number_of_hash_table_entries_to_add_per_thread is default 25,000)

In this instance, I used:
./hash-table-tester -t 4 -s 100000

Results:

Generation: 72,627 usec
Hash table base: 1,147,044 usec
    - 0 missing
Hash table v1: 2,064,180 usec
    - 0 missing
Hash table v2: 340,128 usec
    - 0 missing
```
Version 1 is a little slower/faster than the base version. As I locked the entire hash table, the implementaion removes much of the concurrency of the base version and adds additional overhead by having to lock and unlock the mutex.

## Second Implementation
In the `hash_table_v2_add_entry` function, I I added code which locks a mutex before attempting to retrieve or change a value in each bucket and unlocks it afterwards to ensure that only one thread can modify the table at a time. This also prevents race conditions during concurrent access. The "pthread_mutex_t" mutex is included in the hash table structure for synchronization. Additionally, the mutex is initialized in the "hash_table_v2_create" function and destroyed in "hash_table_v2_destroy".

### Performance
```shell
./hash-table-tester -t number_of_threads -s number_of_hash_table_entries_to_add_per_thread
(number_of_hash_table_entries_to_add_per_thread is default 25,000)

In this instance, I used:
./hash-table-tester -t 4 -s 75000

Results:

Generation: 55,533 usec
Hash table base: 562,285 usec
    - 0 missing
Hash table v1: 1,160,254 usec
    - 0 missing
Hash table v2: 164,331 usec
    - 0 missing
```

TODO more results, speedup measurement, and analysis on v2

Running two more times, the average base case time is 535,171.333 usec and the average v2 time was 181,387.667 usec. This is a 2.95x speedup when using 4 cores.

Version 2 is faster than version 1 and the base implementation primarily due to the granularity of its locking mechanism. This fine-grained locking reduces contention compared to v1, where a single mutex for the entire hash table serializes access, leading to potential bottlenecks when multiple threads attempt to access the table. By locking the buckets instead of the entire hash table, v2 allows more parallelism and enhances performance, especially in scenarios with high concurrency.

## Cleaning up
```shell
Because the Makefile is already made, you can simply run the make clean command to clean up the binary files:

make clean
```