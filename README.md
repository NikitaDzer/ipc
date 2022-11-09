# IPC performance
## Description
IPC -  inter-process communication refers specifically to the mechanisms an operating system provides to allow the processes to manage shared data (from [wikipedia](https://en.wikipedia.org/wiki/Inter-process_communication). Our homework assignement was to measure performance of **shared memory**, **messages** and **pipe** ipc with different shared data size and different chunks size.

## Approach to measure
Util time can measure real execution time. <br>
We have written **run.sh** script to run this utility 5 times for each combination. <br>

In our approach chunks size is a multiple of the page size. So, we use 3 different chunks: 1 page, 2 pages and 4 pages.

## Reproduction

* Clone the repository:
`git clone https://github.com/NikitaDzer/ipc.git`

* Compile ipc tester
```shell
cd ipc
make
```

* Generate tests
`make test`

* Run tests
`./run.sh`


## Working stand
OS: Arch Linux 6.0.6 <br>
CPU: AMD Ryzen 5 4600H

## Results
Shared data size = **256** mb.

| IPC           | 1 page, secs | 2 pages, secs | 4 pages, secs |
|---------------|--------------|---------------|---------------|
| Messages      | 1.4          | 0.8           | -             | 
| Shared memory | 1.0          | 0.6           | 0.3           |
| Pipe          | 0.10         | 0.09          | 0.08          |
<br>

Shared data size = **1024** mb.

| IPC           | 1 page, secs | 2 pages, secs | 4 pages, secs |
|---------------|--------------|---------------|---------------|
| Messages      | 5.0          | 3.0           | -             | 
| Shared memory | 4.0          | 2.1           | 1.2           |
| Pipe          | 0.30         | 0.28          | 0.27          |
<br>

Shared data size = **4096** mb.

| IPC           | 1 page, secs | 2 pages, secs | 4 pages, secs |
|---------------|--------------|---------------|---------------|
| Messages      | 20           | 12            | -             | 
| Shared memory | 14           | 8             | 4             |
| Pipe          | 1.2          | 1.1           | 1.0           |
<br>

So, the **slowest** IPC is messages and the **fastest** is pipe. Also we see that **messages** and **shared memory** performance increases almost in proportion to the increase in the chunk size.
