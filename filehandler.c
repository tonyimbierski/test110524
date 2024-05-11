#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "memoryhandler.h"
#include "errorhandler.h"

char * buffer;
unsigned int bufferSize = 0;

/* Assumptions: file size limited to 2Gb */
char * fhReadAsBlock(char * filename, unsigned int * dataSize)
{
    int filesize;
    char * data;

    ehCheckNullParameter(filename, "fhReadAsBlock");
    ehCheckNullParameter(dataSize,"fhReadAsBlock");

    FILE *f = fopen(filename, "rb");

    if (f == NULL) 
    {
        perror(filename);
        ehFatalNoReturn(errno, "fhReadAsBlock");
    }

    if (fseek(f, 0, SEEK_END) < 0)
    {
        perror(filename);
        ehFatalNoReturn(errno, "trying to seek to end of file");
    }

    filesize = ftell(f);
    if (filesize == 0)
    {
        printf("%s is empty\n", filename);
        ehFatalNoReturn(-1, "fhReadAsBlock");
    }

    data = mhAllocOrAbort(filesize);
    rewind(f);
    fread(data, filesize, 1, f);
    fclose(f);   
    *dataSize = filesize; 
    return data;
}

FILE * fhOpen(char * name)
{
    FILE * f = fopen(name, "r");
    if (f == NULL)
    {
        perror(name);
        ehFatalNoReturn(errno, "fhOpen");
    }
    return f;
}

char * fhReadNextOrNull(FILE * f, unsigned int longestExpected)
{
    char * status;

    if (longestExpected > bufferSize)
    {
        buffer = mhAllocOrAbort(longestExpected+1);
        bufferSize = longestExpected;
    }
    
    fgets(buffer, bufferSize, f);
}

void fhCleanup()
{
    if (buffer)
    {
        free(buffer);
        buffer = NULL;
    }
}