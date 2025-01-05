#include <stdio.h>
#include <stdlib.h>
#include "inc/system.h"

#ifdef _WIN32
#include <windows.h>

void list_files_in_directory(const char *path, ListListFilesCb cb) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;

    char search_path[MAX_PATH];
    snprintf(search_path, MAX_PATH, "%s\\*", path);

    hFind = FindFirstFile(search_path, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error: Cannot open directory %s\n", path);
        return;
    }

    do {
        cb(findFileData.cFileName);
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

#else
#include <dirent.h>

void list_files_in_directory(const char *path, ListFilesCb cb) {
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Skip the special entries "." and ".."
        cb(entry->d_name);
    }

    closedir(dir);
}
#endif
