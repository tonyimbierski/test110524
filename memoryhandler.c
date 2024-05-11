#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <errno.h>

#include "qdglobal.h"
#include "errorhandler.h"

/* Assumptions: limited to 2Gb */
void * mhAllocOrAbort(unsigned int size)
{
    void * allocated = calloc(1, size);
    if (allocated == NULL)
    {
        printf("unable to allocate %dbytes\n", size);
        ehFatalNoReturn(ENOMEM, "mhAllocOrAbort");
    }

    return allocated;
}

