#include <stdio.h>
#include <stdlib.h>

#include "indexer.h"
#include "qdglobal.h"
#include "filehandler.h"

void reportNewLines(struct lineIndex * line)
{
    if (line->lineSeen == FALSE)
    {
        printf("+%.*s\n", line->length, line->data);
    }
}
void quikdiff(const char * filename1, const char * filename2)
{
    unsigned int file2DataSize;
    char * file2Data = fhReadAsBlock(filename2, &file2DataSize);
    FILE * file1;
    unsigned int longestExpected;
    char * testString;
    struct lineIndex * lineInFile2;

    indexBuild(file2Data, file2DataSize);
    longestExpected = indexLongestString()+1;

    file1 = fhOpen(filename1);
    while((testString = fhReadNextOrNull(file1, longestExpected)) != NULL)
    {
        lineInFile2 = indexFind(testString);
        if (lineInFile2)
        {
            lineInFile2->lineSeen = TRUE;
        }
        else
        {
            printf("-%s\n", testString);
        }
    }
    fclose(file1);

    /* now print added lines */

    indexProcessAll(reportNewLines);

    indexCleanup();
    fhCleanup();
    free(file2Data);
}

