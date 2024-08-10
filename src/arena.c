#include "inc/arena.h"
#include <stdlib.h>
#include <string.h>


typedef struct Node  {
    void* pool;
    size_t used;
    size_t capacity;
    struct Node* next;
} Node;

typedef struct Arena {
    Node* nodes;
    size_t total_nodes;
} Arena;

Arena arena;


Node* node_new(size_t size) {
    Node* node = ARENA_MALLOC(sizeof(Node));

    node->used = 0;
    node->capacity = 0;
    node->next = NULL;
    node->pool = ARENA_MALLOC(size);

    return node;
}

int arena_add_node(size_t size) {
    Node* node = node_new(
        size > ARENA_SIZE ? size * 2 : ARENA_SIZE
    );

    if (!(node && node->pool)) return -1;

    node->next = arena.nodes;
    arena.nodes = node;
    arena.total_nodes++;

    return 0;
}

/*
 * Initializes the arena with an initital memory pool.
 * */
void arena_init() {
    arena.nodes = NULL;
    arena.total_nodes = 0;
}

/*
 * Allocate SIZE number of bytes.
 * */
void* arena_alloc(size_t size) {
    if (!arena.total_nodes && arena_add_node(size)) {
         goto failure;
    }

    Node* node = arena.nodes;
    
    while (node->used + size > node->capacity) {
        if (!node->next) {
            if (arena_add_node(size))
                goto failure;
            node = arena.nodes;
            break;
        }
        node = node->next;
    }

    void* mem = (node->pool + node->used);
    node->used += size;

    return mem;

failure:
    return NULL;
}

/*
 * Deallocates and cleans up arena.
 * */
void arena_deinit() {
    Node* node = arena.nodes;
    while (node != NULL) {
        Node* ptr = node;
        node = node->next;
        ARENA_FREE(ptr->pool);
    }

    arena.nodes = NULL;
    arena.total_nodes = 0;
}
