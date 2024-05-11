#include "quikdiff.h"

#include <stdio.h>
#include <stdlib.h>

void usageNoReturn()
{
    printf("usage: quikdiff <file 1> <file 2>\n");
    printf("\tshows lines missing or added from file 1 to file 2\n");
    exit(-1);
}

int main(int argc, char const *argv[])
{
    if (argc != 3) usageNoReturn();

    quikdiff(argv[1], argv[2]);

    return 0;
}
