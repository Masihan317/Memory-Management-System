# Memory-Management-System

## Description

The heap/free list is a linked list with available memory that the user can request. The blocks on the free list sorted in order of ascending memory address. The very beginning of the list consists of a node of 16 bytes. The first 8 bytes of the node stores the size of memory available to the user, while the second 8 bytes stores a pointer to the next node. The rest of the memory are free memory available to the user. The user can use getmem to request memory where a block of usable memory will be returned to the user. getmem will search through the free list and find a block of memory big enough and either return that block of memory to the user directly or split that block then return the memory. freemem returns a block of memory after the user finished using it where it will be put back on the free list for reuse.

## Program Notes

The current program is able to detect the size of memory requested by the user to decide between either splitting an available block or returning the entire block to the user to prevent fragmentation and/or waste of memory.

## Sample Results

I did 10 runs each of ./bench using the default parameters (1000 trials, 50% getmem calls, 10% of large block requests, largest size of small blocks being 200 bytes, largest size of large blocks being 20000 bytes, random seed value being the current time). The program appears quite efficient. (The results of 2 runs of the bench test on the memory system is listed below.)

### Default Parameters Run 1

```
Running bench for 10000 trials, 50% getmem calls.
Elapsed time: 1 msec, Total storage = 630528 bytesFree blocks = 42, Average block = 12542.000000 bytes
Elapsed time: 2 msec, Total storage = 690000 bytesFree blocks = 39, Average block = 15545.000000 bytes
Elapsed time: 3 msec, Total storage = 844336 bytesFree blocks = 57, Average block = 2746.000000 bytes
Elapsed time: 5 msec, Total storage = 1254016 bytesFree blocks = 84, Average block = 8339.000000 bytes
Elapsed time: 8 msec, Total storage = 1254016 bytesFree blocks = 85, Average block = 8953.000000 bytes
Elapsed time: 11 msec, Total storage = 1274016 bytesFree blocks = 86, Average block = 8901.000000 bytes
Elapsed time: 14 msec, Total storage = 1294032 bytesFree blocks = 83, Average block = 8745.000000 bytes
Elapsed time: 17 msec, Total storage = 1294032 bytesFree blocks = 88, Average block = 6655.000000 bytes
Elapsed time: 21 msec, Total storage = 1314048 bytesFree blocks = 84, Average block = 5659.000000 bytes
Elapsed time: 21 msec, Total storage = 1314048 bytesFree blocks = 84, Average block = 5659.000000 bytes
```

### Default Parameters Run 2

```
Running bench for 10000 trials, 50% getmem calls.
Elapsed time: 0 msec, Total storage = 430288 bytesFree blocks = 31, Average block = 7876.000000 bytes
Elapsed time: 1 msec, Total storage = 470304 bytesFree blocks = 27, Average block = 6199.000000 bytes
Elapsed time: 2 msec, Total storage = 678096 bytesFree blocks = 47, Average block = 6995.000000 bytes
Elapsed time: 4 msec, Total storage = 1154208 bytesFree blocks = 83, Average block = 6956.000000 bytes
Elapsed time: 7 msec, Total storage = 1174176 bytesFree blocks = 84, Average block = 5674.000000 bytes
Elapsed time: 9 msec, Total storage = 1174176 bytesFree blocks = 71, Average block = 13395.000000 bytes
Elapsed time: 12 msec, Total storage = 1174176 bytesFree blocks = 69, Average block = 16294.000000 bytes
Elapsed time: 15 msec, Total storage = 1174176 bytesFree blocks = 68, Average block = 16791.000000 bytes
Elapsed time: 18 msec, Total storage = 1174176 bytesFree blocks = 69, Average block = 14169.000000 bytes
Elapsed time: 20 msec, Total storage = 1214208 bytesFree blocks = 79, Average block = 11739.000000 bytes
Elapsed time: 20 msec, Total storage = 1214208 bytesFree blocks = 79, Average block = 11739.000000 bytes
```