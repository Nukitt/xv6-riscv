#include "../kernel/types.h"
#include "../kernel/param.h"
#include "../kernel/stat.h"
#include "user.h"

int integer_check(char s[])
{
    // int flag = 1;
    // for(int i = 0; s[i] != '\0' ; i++)
    // {
    //     if(s[i] < '0' || s[i] > '9')
    //     {
    //         return 0;
    //     }
    // }

    int i=0;
    while(s[i] != '\0')
    {
        if(s[i] < '0' || s[i] > '9')
        {
            return 0;
        }
        i++;
    }

    return 1;
}


int main(int argc, char*argv[])
{
    char *nargv[MAXARG];

    int flag,fail;
    flag = integer_check(argv[1]);
    
    // checking for incorrect input
    if(argc < 3 || !flag)
    {
        fprintf(2, "Incorrect Syntax\n");
        fprintf(2, "Correct Syntax: strace <mask> <command>\n");
        exit(1);
    }

    int mask = atoi(argv[1]);
    fail = trace(mask);

    if(fail < 0)
    {
        fprintf(2, "strace: Trace Failed\n");
    }

    // copying the command
    for(int i=0;i<argc-2 && i< MAXARG-2;i++)
    {
        // printf(1,"%s ",nargv[i]);
        nargv[i] = argv[i+2];
    }

    exec(nargv[0], nargv);   // executing command

    return 0;
}