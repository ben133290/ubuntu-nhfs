//
// Created by seraina on 08.06.23.
// This c file handles all interaction with the hierarchical file system. It includes functionalities such as:
// Open (! how to open and mostly return to path), Close, Call editor, Delete file (in hfs), Create (mind .txt file)
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/interactionSys.h"

void openFile(const char * path){
    const char * helper = "xdg-open ";
    char buffer[2000]; //TODO: is 500 enough for long paths?
    strncpy(buffer, helper, sizeof(buffer));
    strncat(buffer, path, sizeof(buffer) - strlen(buffer) - 1);
    const char * command = (const char*) buffer;
    printf("%s\n",command);
    if(system(NULL) != 0) {
        system(command);
        printf("Im hereee\n");
    } else {
        printf("Error: Shell unavailable for command: %s\n", command);
    }
}
void closeFile(const char * path){

}

void callEditor(const char * path){

}

void deleteFile(const char * path){

}

const char * createFile(const char * path, const char * filename){

}

/*int main() { //for testing purpose
    openFile("/home/seraina/ubuntu-nhfs/data/graph.txt");
    openFile("/home/seraina/OS/ex/os-exercise-3/os-exercise-3.pdf"); //TODO why do other types of files not work?
    openFile("/home/seraina/OS/ex/os-exercise-4/text.txt");

}*/
