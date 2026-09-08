# CS 452 Homework 1

This program implements a double-ended, doubly linked queue that stores pointers to arbitrary data.

## Project Structure

* `GNUmakefile` - Provided parent build file
* `hw1/deq.c` - Queue implementation
* `hw1/deq.h` - Provided queue interface
* `hw1/main.c` - Test suite
* `hw1/error.h` - Provided error macros
* `hw1/GNUmakefile` - Provided homework build file
* `hw1/libdeq.so` - Provided reference implementation
* `hw1/results.txt` - Saved test and Valgrind results

## Compile

From the `hw1` directory:

```bash
cd hw1
```

Compile my implementation:

```bash
make
```

## Run My Implementation

```bash
./deq
```

Expected result:

```text
All queue tests passed.
```

## Run Tests With the Provided Library

```bash
make try
./try
```

Expected result:

```text
All queue tests passed.
```

## Check for Memory Errors

```bash
make valgrind
```

Valgrind result:

```text
31 allocs, 31 frees
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts
```

Complete output is saved in `hw1/results.txt`.