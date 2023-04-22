#!/bin/bash

# Compile test.c with thread.c
gcc -o test test.c thread.c

# Compile test.c with mythread.c
gcc -o spin thread.c test_spin.c lock.c

# Execute the two versions of test.c
echo " "
echo "-----Testing-------"
./test
echo " "
echo "-----Testing Spinlock------"
./spin
