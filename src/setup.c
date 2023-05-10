#include <stdio.h>
#include <stdlib.h>

// when filesystem is set up, should alter ~/.bashrc and add path of tfs

FILE *fptr;

int main()
{
    fptr = fopen("~/.bashrc", "a"); //a for append
    fputs("nhfs(){}", fptr); // still causes segfault, why?
    fclose(fptr);
    return 1;
}
