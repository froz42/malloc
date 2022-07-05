# Malloc @ 42

This project is about implementing a dynamic memory allocation  
mechanism

![Screenshot from 2022-07-05 16-07-32](https://user-images.githubusercontent.com/54767855/177347408-631330cd-0222-4245-ab1f-083a8eb5d0b3.png)

## Memory page

First, we  have  to  define  what  is  a  memory  page: In  computer  science, a  memory  page  is  a  contiguous  block  of  memory  managed  by  the  virtual  memory. When  the  kernel  give  memory  to  a  program  It  will  give  a  whole  page  of  memory  at  least.

## functions
### GETPAGESIZE(2)                                                                                                            
```c
#include <unistd.h>

int getpagesize(void);
```
This function return the number of bytes in a memory pages.

### MMAP(3P)                                                                
```c
 #include <sys/mman.h>

void *mmap(void *addr, size_t len, int prot, int flags,
           int fildes, off_t off);
 ```

This function will apply an object to an address space:
* addr: A hint where the memory should be placed. (Can be NULL)
* len: number of bytes to map.
* prot: define is address is writable, readable, executable.
* flags: can tell if memory is shared between process or not
and tell if map is anonymous: mapped to a zeroed virtual file
* fildes, the file to be mapped
* off_t: offset of the file

### MUNMAP(3P)

```c
#include <sys/mman.h>

int munmap(void *addr, size_t len);
```

This function remove mapping did with mmap

## Basic structure

### allocation bit
to know if a block is allocated with use an allocation bit:
Our size is always aligned to 16: so we will never get a odd number.

```
┌──────────────┐
│ Size (even)  │
├──────────────┼─┐
│ 0 1 0 1 1 1 0│1│
└──────────────┴─┘
                ▲
                │
                │
             allocation bit
```
### block
The block has the following structure:
```
---------------------------------
| size | prev | data + padding |
---------------------------------
```
* size: the size of the block in bytes

* prev: the previous block in the area

* data + padding: the data of the block

* The data of the block is aligned to 16 bytes

## Best fit

To find where to put my block, I use a red-black tree sorted by size.
It reduce the complexity of O(n) to O(log(n)) 
![Screenshot from 2022-07-05 15-59-46](https://user-images.githubusercontent.com/54767855/177347527-3e4832ec-40aa-4ac6-a8a0-578d62a77f1d.png)

## Defragmentation

When I free a block, I check if the previous and next block are not allocated and merge them
