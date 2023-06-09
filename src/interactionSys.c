//
// Created by seraina on 08.06.23.
// This c file handles all interaction with the hierarchical file system. It includes functionalities such as:
// Open (! how to open and mostly return to path), Close, Call editor, Delete file (in hfs), Create (mind .txt file)
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/interactionSys.h"

/*
 * Opens file in standard program, also includes editors for i.e. txt files
 */
void openFile(const char * path){
    const char * helper = "xdg-open ";
    char buffer[2000]; //TODO: is 500 enough for long paths?
    strncpy(buffer, helper, sizeof(buffer));
    strncat(buffer, path, sizeof(buffer) - strlen(buffer) - 1); //adds helper and path together in buffer
    const char * command = (const char*) buffer;
    printf("%s\n",command);
    if(system(NULL) != 0) { //Error handling for the rare case that no shell is available
        system(command);
    } else {
        printf("Error: Shell unavailable for command: %s\n", command);
    }
}
void closeFile(const char * path){ //might be dangerous, maybe just use editor/application quit

}


void deleteFile(const char * path){ //TODO: Do we need to check for permissions?
    int ret = remove(path);
    if(ret != 0) {
        printf("Error: Failed to remove file %s\n", path);
    }
}

int createFile(const char * path) {

    // Open the file in write mode
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("Error: Failed to create file %s\n", path);
        return -1;
    }

    // Close the file
    fclose(file);
    return 0;
}

/*int main() { //for testing purpose
    openFile("/home/seraina/ubuntu-nhfs/data/graph.txt");
    openFile("/home/seraina/OS/ex/os-exercise-3/os-exercise-3.pdf"); //TODO why do other types of files not work?
    openFile("/home/seraina/OS/ex/os-exercise-4/text.txt");
    deleteFile("/home/seraina/ubuntu-nhfs/data/test1.txt");
}*/
