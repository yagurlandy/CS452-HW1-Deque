# CS 452 Homework 1

This program implements a double-ended, doubly linked queue that stores pointers to arbitrary data.

## Compile

From the `hw1` directory:

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
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts
```

## Files

* `deq.c` - Queue implementation
* `deq.h` - Provided queue interface
* `main.c` - Test suite
* `error.h` - Provided error macros
* `GNUmakefile` - Provided build instructions
* `libdeq.so` - Provided reference implementation