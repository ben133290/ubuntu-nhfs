#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include "../include/util.h"

#ifndef PATH_MAX
#define PATH_MAX 4096  // Replace with an appropriate value if needed
#endif
#ifndef DT_DIR
#define DT_DIR 4  // Replace with the correct value if needed
#endif


char* readNthLine(const char* filename, int n) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return NULL;
    }

    char* line = NULL;
    size_t bufferSize = 0;
    size_t bytesRead;
    int currentLine = 1;

    while ((bytesRead = getline(&line, &bufferSize, file)) != -1) {
        if (currentLine == n) {
            // Remove newline character from the end of the line, if present
            while (bytesRead > 0 && (line[bytesRead - 1] == '\n' || line[bytesRead - 1] == '\r')) {
                line[bytesRead - 1] = '\0';
                bytesRead--;
            }

            break;
        }
        currentLine++;
    }

    fclose(file);

    if (currentLine < n) {
        printf("Requested line does not exist in the file.\n");
        free(line);
        return NULL;
    }

    return line;
}

void replaceNthLine(const char* filename, int n, const char* newString) {
    FILE* file = fopen(filename, "r+");
    if (file == NULL) {
        printf("Error: Failed to open the file.\n");
        return;
    }

    char buffer[1024];
    int lineCount = 0;
    int foundLine = 0;

    // Read and write line by line
    while (fgets(buffer, sizeof(buffer), file)) {
        lineCount++;

        if (lineCount == n) {
            foundLine = 1;
            // Rewind the file pointer to overwrite the line
            fseek(file, -strlen(buffer), SEEK_CUR);
            fputs(newString, file);
            // Pad the remaining characters with spaces to ensure old content is overwritten
            int padding = strlen(buffer) - strlen(newString);
            for (int i = 0; i < padding; i++) {
                fputc(' ', file);
            }
            break;
        }
    }

    if (!foundLine) {
        printf("Error: Line %d not found in the file.\n", n);
    }

    fclose(file);
}

void removeDirectory(const char* path) {
    DIR* dir = opendir(path);
    struct dirent* entry;

    // Iterate over directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Ignore current and parent directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Create the full path for the entry
        char entryPath[PATH_MAX];
        snprintf(entryPath, sizeof(entryPath), "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            // Recursive call for subdirectories
            removeDirectory(entryPath);
        } else {
            // Remove regular files
            if (remove(entryPath) != 0) {
                printf("Failed to remove file: %s\n", entryPath);
            }
        }
    }

    closedir(dir);

    // Remove the directory itself
    if (rmdir(path) != 0) {
        printf("Failed to remove directory: %s\n", path);
    }
}