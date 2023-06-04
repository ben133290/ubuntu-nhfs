#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

// TODO: when filesystem is set up, should alter ~/.bashrc and add path of tfs

FILE *fptr;

int add_bash_function()
{
    int status1 = system("chmod u+x bash_function.sh");
    int status2 = system("./bash_function.sh");
    return 1;
}
