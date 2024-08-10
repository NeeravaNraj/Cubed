#ifndef ARENA_H
#define ARENA_H

#include <stdlib.h>
#include <stdbool.h>

#define ARENA_MALLOC malloc
#define ARENA_CALLOC calloc
#define ARENA_FREE free

#ifndef ARENA_SIZE
#define ARENA_SIZE 10000 // 10kb nodes
#endif // !ARENA_SIZE

void arena_init();
void* arena_alloc(size_t size);
void arena_deinit();
#endif // !ARENA_H
