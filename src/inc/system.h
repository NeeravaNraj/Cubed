#ifndef SYSTEM_H
#define SYSTEM_H

typedef void (*ListFilesCb)(const char* dirent);
void list_files_in_directory(const char *path, ListFilesCb cb);
#endif // !SYSTEM_H
