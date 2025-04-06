# ThreadSort

ThreadSort is a program that generates random numbers and sorts them into even and odd lists using multiple threads.

## Compilation

To compile the program, run:

```bash
make
```

## Usage
Run the program with a configuration file:

```bash
./threadsort --file config.txt
```

Create a file named config.txt (or any name you prefer) with the following format:
```bash
numbers_per_thread=10
thread_num=2
```
numbers_per_thread: How many random numbers each thread will generate.
thread_num: How many threads to use.

## Basic Makefile Commands
```bash
make: Compiles the program.
make run: Compiles and runs the program with a default configuration.
make clean: Removes compiled files.
```

## Testing
To run basic tests:

```bash
make unit_test: unit test
make test: functional test
```