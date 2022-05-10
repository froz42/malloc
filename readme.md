# Malloc

***project status***

[![tmatis's 42 malloc Score](https://badge42.vercel.app/api/v2/cl1mkhkns001109mixy0purl5/project/2559363)](https://github.com/JaeSeoKim/badge42)
## Authorised functions

- mmap
- munmap
- getpagesize

# Data structure

## alocated block

```
head (size n + is_allocated) + payload of n size + footer (where the previous block start)
```

## freed block

rb tree of freed nodes

```
head (size n + is_allocated) + left + right + parent + footer (where the previous block start)
```