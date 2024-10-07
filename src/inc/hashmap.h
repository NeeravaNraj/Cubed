#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>

#ifndef HASHMAP_CONFIG

#define INITIAL_GLOBAL_DEPTH 1
#define BUCKET_SIZE 32

#endif // !HASHMAP_CONFIG

#ifndef HASHMAP_ALLOC_CONFIG
#define MALLOC malloc
#define REALLOC realloc
#define FREE free
#endif // !HASHMAP_MALLOC



typedef struct {
    const char* key;
    size_t key_len;
    void* value;
} Entry;

typedef struct {
    Entry entries[BUCKET_SIZE];
    size_t len;
    size_t local_depth;
    int prefix;
} Bucket;

typedef struct {
    Bucket** buckets;
    size_t global_depth;
} Directory;

typedef struct {
    Directory directory;
} HashMap;

typedef struct {
    size_t page;
    size_t entry;
    size_t map_size;
    size_t bucket_size;
    HashMap* map;
} HashMapIterator;


void hashmap_init(HashMap* map);
void hashmap_insert(HashMap* map, const char* key, void* value);
const Entry* hashmap_get(HashMap* map, const char* key);
void hashmap_update(HashMap* map, const char* key, void* value);
bool hashmap_delete(HashMap* map, const char* key);
void hashmap_deinit(HashMap* map);
void hashmap_init_iterator(HashMapIterator* it, HashMap* map);
Entry* hashmap_next_entry(HashMapIterator* it);
void hashmap_print(HashMap* map);
size_t hashmap_len(HashMap* map);
#endif // !HASHMAP_H
