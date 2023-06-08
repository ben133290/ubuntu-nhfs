#ifndef UTIL_H
#define UTIL_H

char* readNthLine(const char* filename, int n);
void replaceNthLine(const char* filename, int n, const char* newContent);
void removeDirectory(const char* directoryPath);

#endif
