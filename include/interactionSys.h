//
// Created by seraina on 08.06.23.
//

#ifndef INTERACTIONSYS_HEADER
#define INTERACTIONSYS_HEADER

void openFile(const char * path);
void closeFile(const char * path);
void callEditor(const char * path);
void deleteFile(const char * path);
const char * createFile(const char * path, const char * filename);

#endif //INTERACTIONSYS_HEADER
