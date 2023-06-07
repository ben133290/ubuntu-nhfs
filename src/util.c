#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/util.h"

char* readNthLine(const char* filename, int n) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Unable to open the file.\n");
        return NULL;
    }

    // Move the file pointer to the desired line
    int lineCount = 1;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (lineCount == n) {
            // Allocate memory to store the line
            char* line = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
            if (line == NULL) {
                printf("Memory allocation failed.\n");
                fclose(file);
                return NULL;
            }

            // Copy the line into the allocated memory
            strcpy(line, buffer);

            fclose(file);
            return line;
        }
        lineCount++;
    }

    printf("Specified line does not exist.\n");
    fclose(file);
    return NULL;
}