#include <stdio.h>
#include <stdlib.h>

/* fatal error reported, does not return */
void ehFatalNoReturn(int error, char * message)
{
    printf("fatal error %d in %s\n", error, message ? message : "<unknown>");
    exit(error);
}

void ehCheckNullParameter(void * p, char * source)
{
    if (p == NULL)
    {
        printf("null parameter in %s!\n ", source ? source : "<unknown>");
        ehFatalNoReturn(-1, source);
    }
}