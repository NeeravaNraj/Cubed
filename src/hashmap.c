#include "inc/hashmap.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define HASHMAP_DIRECTORY_LEN(map) ((1 << (map)->directory.global_depth))

size_t hash(const char* str, size_t depth) {
    uint64_t hash = 7919;

    uint32_t c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return (hash & ((1 << depth) - 1));
}


void init_bucket(Bucket** bucket, int depth) {
    Bucket* new = malloc(sizeof(Bucket));
    new->len = 0;
    new->local_depth = depth;

    *bucket = new;
}


void init_directory(Directory* directory) {
    directory->global_depth = INITIAL_GLOBAL_DEPTH;
    int len = 1 << directory->global_depth;
    directory->buckets = MALLOC(sizeof(Bucket*) * len);

    for (int i = 0; i < len; ++i) {
        init_bucket(&directory->buckets[i], INITIAL_GLOBAL_DEPTH);
        directory->buckets[i]->prefix = i;
    }
}


void hashmap_init(HashMap* map) {
    init_directory(&map->directory);
}


void split_bucket(HashMap* map, size_t hash_index) {
    Bucket* old_bucket = map->directory.buckets[hash_index];
    int local_depth = old_bucket->local_depth;
    int new_local_depth = local_depth + 1;

    if (new_local_depth > map->directory.global_depth) {
        int old_size = 1 << map->directory.global_depth;
        int new_size = old_size * 2;
        map->directory.buckets = REALLOC(map->directory.buckets, sizeof(Bucket*) * new_size);
        for (int i = old_size; i < new_size; ++i) {
            map->directory.buckets[i] = map->directory.buckets[i - old_size];
        }
        map->directory.global_depth++;
    }

    Bucket* new_bucket;
    init_bucket(&new_bucket, new_local_depth);
    old_bucket->local_depth = new_local_depth;

    bool set = false;
    size_t dir_size = 1 << map->directory.global_depth;
    for (int i = old_bucket->prefix + 1; i < dir_size; ++i) {
        Bucket* bucket = map->directory.buckets[i];

        if (bucket->prefix == old_bucket->prefix && i & (1 << local_depth)) {
            map->directory.buckets[i] = new_bucket;
            if (!set) {
                new_bucket->prefix = i;
                set = true;
            }
        }
    }

    // Redistribution
    for (int i = 0; i < old_bucket->len; ++i) {
        Entry entry = old_bucket->entries[i];
        size_t hash_value = hash(entry.key, new_local_depth);

        if (hash_value & (1 << local_depth)) {
            new_bucket->entries[new_bucket->len++] = entry;

            for (int j = i; j < old_bucket->len; ++j) {
                if (j + 1 < old_bucket->len) {
                    old_bucket->entries[j] = old_bucket->entries[j + 1];
                }
            }
            --old_bucket->len;
            --i;
        }
    }
}

void hashmap_insert(HashMap* map, const char* key, void* value) {
    size_t hash_index = hash(key, map->directory.global_depth);
    Bucket* bucket = map->directory.buckets[hash_index];
    /* bool same = (hash_index & ((1 << bucket->local_depth) - 1)) == bucket->prefix; */

    if (hashmap_get(map, key)) return;

    if (bucket->len < BUCKET_SIZE) {
        bucket->entries[bucket->len].key = key;
        bucket->entries[bucket->len].value = value;
        bucket->entries[bucket->len].key_len = strlen(key);
        bucket->len++;
    } else {
        split_bucket(map, hash_index);
        hashmap_insert(map, key, value);
    }
}

const Entry* hashmap_get(HashMap* map, const char* key) {
    size_t hash_index = hash(key, map->directory.global_depth);
    Bucket* bucket = map->directory.buckets[hash_index];
    size_t key_len = strlen(key);
    for (int i = 0; i < bucket->len; ++i) {
        Entry* entry = &bucket->entries[i];
        if (
            key_len == entry->key_len &&
            strncmp(key, entry->key, entry->key_len) == 0
        ) {
            return entry;
        }
    }

    return NULL;
}

void hashmap_update(HashMap* map, const char* key, void* value) {
    size_t hash_index = hash(key, map->directory.global_depth);
    Bucket* bucket = map->directory.buckets[hash_index];
    size_t key_len = strlen(key);
    for (int i = 0; i < bucket->len; ++i) {
        Entry* entry = &bucket->entries[i];
        if (
            key_len == entry->key_len &&
            strncmp(key, entry->key, entry->key_len) == 0
        ) {
            bucket->entries[i].value = value;
        }
    }
}

void hashmap_delete(HashMap* map, const char* key) {
    size_t hash_index = hash(key, map->directory.global_depth);
    Bucket* bucket = map->directory.buckets[hash_index];
    size_t key_len = strlen(key);

    for (int i = 0; i < bucket->len; ++i) {
        Entry* entry = &bucket->entries[i];
        if (
            key_len == entry->key_len &&
            strncmp(key, entry->key, entry->key_len) == 0
        ) {
            bucket->entries[i] = bucket->entries[--bucket->len];
        }
    }
}

void hashmap_deinit(HashMap* map) {
    size_t dir_size = 1 << map->directory.global_depth;
    size_t freed_count = 0;
    Bucket** freed = MALLOC(sizeof(Bucket*) * dir_size);
    for (int i = 0; i < dir_size; ++i) {
        Bucket* bucket = map->directory.buckets[i];
        for (int j = 0; j < freed_count; ++j) {
            if (bucket == freed[j]) {
                goto next;
            }
        }

        FREE(bucket);
        freed[freed_count++] = bucket;
next: continue;
    }

    FREE(freed);
    FREE(map->directory.buckets);
    map->directory.buckets = NULL;
    map->directory.global_depth = 0;
}

void print_binary(size_t num, size_t depth) {
    for (int i = depth- 1; i >= 0; i--) {
        printf("%zu", (num >> i) & 1);
    }
}

void hashmap_print(HashMap* map) {
    size_t dir_len = (1 << map->directory.global_depth);
    Bucket** buckets = alloca(sizeof(Bucket*) * dir_len);
    size_t printed_buckets = 0;

    printf("{\n    gd = %zu\n", map->directory.global_depth);
    for (int i = 0; i < dir_len; ++i) {
        Bucket* bucket = map->directory.buckets[i];
        bool printed = false;
        size_t parent;
        for (int j = 0; j < printed_buckets; ++j) {
            if (buckets[j] == bucket) {
                printed = true;
                parent = j;
            }
        }

        if (!printed) {
            buckets[printed_buckets++] = bucket;
            printf("    Bucket %d with ld = %zu at [%p]\n", i, bucket->local_depth, bucket);
            for (int j = 0; j < bucket->len; ++j) {
                Entry* entry = &bucket->entries[j];
                size_t h = hash(entry->key, map->directory.global_depth);
                printf("      %s = %p, hash = ", entry->key, entry->value);
                print_binary(h, map->directory.global_depth);
                printf("\n");
            }
        } else {
            printf("    Bucket %d -> %zu with ld = %zu at [%p]\n", i, parent, bucket->local_depth, bucket);
        }
    }
    printf("}\n");
}

void hashmap_init_iterator(HashMapIterator* it, HashMap* map) {
    it->page = 0;
    it->entry = 0;

    it->map = map;
    it->bucket_size = 0;
    it->map_size = HASHMAP_DIRECTORY_LEN(map);
}


Entry* hashmap_next_entry(HashMapIterator* it) {
    if (it->page >= it->map_size) return NULL;
    assert(it->map_size == HASHMAP_DIRECTORY_LEN(it->map));

    while (!it->map->directory.buckets[it->page]->len) {
        it->page++;
    }

    if (it->page >= it->map_size) return NULL;

    Bucket* bucket = it->map->directory.buckets[it->page];
    if (it->bucket_size == 0) {
        it->bucket_size = bucket->len;
    }

    /* printf("test %zu == %zu\n", it->bucket_size, bucket->len); */
    assert(it->bucket_size == bucket->len);
    Entry* entry = &bucket->entries[it->entry++];

    if (it->entry >= bucket->len) {
        it->page++;
        it->entry = 0;
        it->bucket_size = 0;
    }

    return entry;
}

size_t hashmap_len(HashMap* map) {
    size_t total = 0;

    for (size_t i = 0; i < (1 << map->directory.global_depth); ++i) {
        total += map->directory.buckets[i]->len;
    }

    return total;
}
