#include <string.h>
#include <stdlib.h>

#include "qdglobal.h"
#include "indexer.h"
#include "errorhandler.h"
#include "memoryhandler.h"

static struct lineIndex* indexBySize[OVERSIZELINES];
static unsigned int longestStringObserved = 0;
static const char LF = '\n';
static const char CR = '\r';

/* internal prototypes */
void indexAdd(struct lineIndex *);
void checkIndexOrAbort(unsigned int );
struct lineIndex * indexFetch(unsigned int );
void indexStore(struct lineIndex * , unsigned int );

/* we don't store zero length lines, so save an array entry! */
void checkIndexOrAbort(unsigned int index)
{
    if ((index > 0) && (index < OVERSIZELINES))
    {
        return;
    }
    else
    {
        ehFatalNoReturn(-1, "index check");
    }
}

void indexStore(struct lineIndex * item, unsigned int index)
{
    checkIndexOrAbort(index);
    indexBySize[index-1] = item;
}

struct lineIndex * indexFetch(unsigned int index)
{
    checkIndexOrAbort(index);
    return indexBySize[index-1];
}


/* assumption: most lines will be less than 1024 characters */
struct lineIndex * indexBuild(char * data, unsigned int dataLength)
{
    char * currentLine = data;
    unsigned int currentLineLength;
    char * cursor = data;
    struct lineIndex * indexHead = NULL;
    struct lineIndex * newIndexEntry;

    ehCheckNullParameter(data, "indexBuild");

    while(cursor < data + dataLength)
    {
        if ((*cursor == CR) || (*cursor == LF))
        {
            /* end of line */
            currentLineLength = cursor - currentLine + 1;
 
            /* ignore empty lines - this also handles windows CRLF line terminators */
            if (currentLineLength > 0)
            {
                if (currentLineLength > longestStringObserved)
                {
                    longestStringObserved = currentLineLength;
                }
                newIndexEntry = mhAllocOrAbort(sizeof(struct lineIndex));
                newIndexEntry->data = currentLine;
                newIndexEntry->length = currentLineLength;
                newIndexEntry->lineSeen = FALSE;
                indexAdd(newIndexEntry);
                currentLine = cursor+1;
            }
        }

        cursor++;
    }
}

void indexAdd(struct lineIndex * newLine)
{
    ehCheckNullParameter(newLine, "indexAdd");
    unsigned int sizeForIndex = newLine->length;
    if (sizeForIndex > OVERSIZELINES)
    {
        sizeForIndex = OVERSIZELINES;
    }

    struct lineIndex * temp = indexFetch(sizeForIndex);
    indexStore(newLine, sizeForIndex);
    newLine->next = temp;
}

unsigned int indexLongestString()
{
    return longestStringObserved;
}

struct lineIndex * indexFind(char * string)
{
    int sizeToFind;
    struct lineIndex * test;
    ehCheckNullParameter(string, "indexFind");
    sizeToFind = strnlen(string, OVERSIZELINES);
    test = indexFetch(sizeToFind);
    while(test)
    {
        if (checkExactMatch(test, string))
        {
            return test;
        }
        test = test->next;
    }
    return NULL;
}

int checkExactMatch(struct lineIndex * test, char * string)
{
    char * p1 = test->data;
    char * p2 = string;

    /* better than memcmp or strcmp as we bail out quickly if no match */
    while(*p1 == *p2)
    {
        if ((*p1 == LF) || (*p1 == CR) || (*p1 == '\0'))
        {
            return TRUE;
        }
        p1++;
        p2++;
    }

    return FALSE;
}

void indexProcessAll(void (*indexCallback)(struct lineIndex *))
{
    unsigned int i;
    struct lineIndex *item;
    for (i=1; i<OVERSIZELINES; i++)
    {
        item = indexFetch(i);
        while(item)
        {
            indexCallback(item);
            item = item->next;
        }
    }
}

void indexCleanup()
{
    int i;
    struct lineIndex * item;
    struct lineIndex * temp;

    for (i=0; i<OVERSIZELINES; i++)
    {
        item = indexBySize[i];
        while (item)
        {
            temp = item->next;
            free(item);
            item = temp;
        }
    }
}