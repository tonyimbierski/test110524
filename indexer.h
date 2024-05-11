#define MAXINDEXENTRIES 1024
#define OVERSIZELINES (MAXINDEXENTRIES + 1)

struct lineIndex
{
    struct lineIndex * next;
    char * data;
    unsigned int length;
    char lineSeen;
};

void indexAdd(struct lineIndex * );
unsigned int indexLongestString();
struct lineIndex * indexFind(char * );
int checkExactMatch(struct lineIndex * test, char * );
void indexProcessAll(void (*)(struct lineIndex *) );
struct lineIndex * indexBuild(char * , unsigned int );
void indexCleanup();