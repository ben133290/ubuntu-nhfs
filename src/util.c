#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/util.h"

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