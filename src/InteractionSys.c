//
// Created by seraina on 08.06.23.
// This c file handles all interaction with the hierarchical file system. It includes functionalities such as:
// Open (! how to open and mostly return to path), Close, Call editor, Delete file (in hfs), Create (mind .txt file)
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/interactionSys.h"

void openFile(const char * path);
void closeFile(const char * path);
void callEditor(const char * path);
void deleteFile(const char * path);
const char * createFile(const char * path, const char * filename);

/*
int main() { //for testing purpose

}*/

