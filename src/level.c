#include "inc/level.h"
#include "inc/arena.h"
#include "inc/hashmap.h"
#include "inc/tilemap.h"
#include "inc/tiles.h"
#include "inc/world.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void handle_err(bool cond, char *loc, char *message) {
  if (!cond) {
    fprintf(stderr, "%s\n", message);
    if (loc != NULL)
      perror(loc);
    exit(EXIT_FAILURE);
  }
}

void write_level(char *name, World *world) {
  size_t name_len = strlen(name);
  size_t tile_count = hashmap_len(&world->tilemap.map);

  char *filename = malloc(name_len + 5);
  sprintf(filename, "%s.cdb", name);
  FILE *file = fopen(filename, "wb");
  handle_err(file != NULL, "write_level", "ERROR: Failed to write level file.");

  Headers headers = {
      .magic = MAGIC,
      .version = VERSION,
      .spawn = world->spawn,
      .end = world->end,
      .tile_length = tile_count,
  };

  Properties properties = {
      .level_name_len = name_len,
      .level_name = name,
  };

  size_t written = fwrite(&headers, sizeof(Headers), 1, file);
  handle_err(written == 1, NULL,
             "ERROR: Could not write 'Headers' properly to file.");

  Entry *tile_entry;
  HashMapIterator it;
  hashmap_init_iterator(&it, &world->tilemap.map);
  while ((tile_entry = hashmap_next_entry(&it))) {
    Tile *tile = tile_entry->value;
    written = fwrite(tile, sizeof(Tile), 1, file);
    handle_err(written == 1, NULL,
               "ERROR: Could not write 'Tile' properly to file.");
  }

  written = fwrite(&properties.level_name_len, sizeof(size_t), 1, file);
  handle_err(
      written == 1, NULL,
      "ERROR: Could not write 'Properties.level_name_len' properly to file.");
  written = fwrite(name, sizeof(char), name_len, file);
  handle_err(
      written == name_len, NULL,
      "ERROR: Could not write 'Properties.level_name' properly to file.");

  fclose(file);
  free(filename);
}

void read_level(char *filename, World *world, int version) {
  size_t read;
  char *name;
  Headers headers;
  Properties properties;

  FILE *file = fopen(filename, "rb");
  handle_err(file != NULL, "read_level", "ERROR: Failed to read level file.");

  read = fread(&headers, sizeof(Headers), 1, file);
  handle_err(read == 1, NULL,
             "ERROR: Could not read 'Headers' properly from file.");

  if (headers.magic != MAGIC || headers.version != version) {
    fprintf(stderr, "ERROR: Invalid level file\n");
    // TODO: maybe got back to main menu or something
    goto cleanup;
  }

  world->spawn = headers.spawn;
  world->end = headers.end;

  for (size_t i = 0; i < headers.tile_length; ++i) {
    Tile *tile = arena_alloc(sizeof(Tile));
    read = fread(tile, sizeof(Tile), 1, file);
    handle_err(read == 1, NULL,
               "ERROR: Could not read 'Tile' properly from file.");
    tilemap_add_tile(&world->tilemap, tile);
  }

  read = fread(&properties.level_name_len, sizeof(size_t), 1, file);
  handle_err(
      read == 1, NULL,
      "ERROR: Could not read 'Properties.level_name_len' properly from file.");

  name = arena_alloc(properties.level_name_len + 1);
  name[properties.level_name_len + 1] = '\0';

  read = fread(name, sizeof(char), properties.level_name_len, file);
  handle_err(
      read == properties.level_name_len, NULL,
      "ERROR: Could not read 'Properties.level_name' properly from file.");
  properties.level_name = name;
  printf("Finished reading level: '%s'\n", name);

cleanup:
  fclose(file);
}
