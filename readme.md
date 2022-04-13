# Malloc

## Authorised functions

- mmap
- munmap
- getpagesize

# ROADMAP

- [ ] comprendre comment marche mmap munmap getpagesize
- [ ] implementer un malloc basique en utilisant uniquement les hors zone
- [ ] reflechir au zone memoires (regarder l'implementation standard)
- [ ] implementer les zone memoire et hors zone et free
- [ ] trouver un moyen de defragmenter la memoire



# implementation small , tiny zones

https://moss.cs.iit.edu/cs351/slides/slides-malloc.pdf

header de taille n contien la taille du block (libre ou pas) 000000001 <- 1 si occuper 0 si free donc un mask 0x1

si on alloue un block avec une size < au block_size / 2 on divide le block:
on modifie le header et on ajoute un header au suivant
