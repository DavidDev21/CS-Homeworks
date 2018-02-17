#include "types.h"
#include "stat.h"
#include "user.h"

/*
    Author: David Zheng
    Course: Intro to OS
    HW #1: SED.C
    Due Date: Feb 10, 2018
*/

// Would like to simulate a vector from C++
// Reasoning: dynamic buffer for reading from fd
// Since I don't know how long is a line (in bytes)

// vector struct and methods
// ============================
typedef struct
{
    char* data;
    uint capacity; // capacity
    uint size; // # of elements in vec
} myVector; 

// resizes vec the data in the vec
void myVecResize(myVector* buff)
{
    //printf(1,"resizing\n");
    buff->capacity = 2*buff->capacity;

    // temp is the new memory space for resizing
    char* temp;
    temp = (char*)malloc(buff->capacity * sizeof(char));
    
    uint i;
    // the length should be fine since its a char = 1 byte
    // copies data from old to new memory
    for(i = 0; i < buff->size; i++)
    {
        temp[i] = buff->data[i];
    }

    free(buff->data);

    // the fact that temp is local scope should be no issue
    // temp points to memory on the heap
    buff->data = temp;
}

// pushes data (a char) into the vec
void myVecPush(myVector* buff, char c)
{
    // Resize if vec is full
    if(buff->size == buff->capacity)
    {
        myVecResize(buff);
    }
    // adds to the end of the vec
    // this is assuming we only dealing with chars
    buff->data[buff->size] = c;
    buff->size++;
}

// just similar allow push() to rewrite this position
char myVecPop(myVector* buff)
{
    buff->size--;
    return buff->data[buff->size]; // should be the last element
}

// "constructor" for myVec
// give inital capacity
// error codes: -1 = fail, 0 = success
int myVectorConstructor(myVector* buff, uint cap)
{
    //printf(1,"In constructor()\n");
    if(cap <= 0)
    {
        cap = 10; // default
    }
    buff->size = 0;
    buff->capacity = cap;
    buff->data = (char*) malloc(cap * sizeof(char));
    return 0;
}

// prints content of the vector
void printVec(myVector* buff)
{
    //printf(1,"printing Vec:");
    uint i;
    for(i = 0; i < buff->size; i++)
    {
        printf(1,"%c",buff->data[i]);
    }
    printf(1,"\n");
}

// Clears out the content of the vectors
void myVecClear(myVector* buff)
{
    buff->size = 0;
    free(buff->data);
    buff->data = (char*) malloc(buff->capacity);
}

void myVecDie(myVector* buff)
{
    free(buff->data);
}
// ==========================================
// Helper functions
// ==========================================
// Counts # of times the key shows up in line
int countKeyword(myVector* line, char* key)
{
    uint i = 0,keyCursor = 0, count = 0, keyLen = strlen(key);
    // C doesn't have boolean type built in????
    uint isMatch = 1; // Assume we have a match
    for(;i < line->size; i++)
    {
        for(keyCursor = 0;keyCursor < keyLen; keyCursor++)
        {
            //printf(1,"%c : ", line->data[i + keyCursor]);
            //printf(1,"%c\n", key[keyCursor]);
            if(line->data[i + keyCursor] != key[keyCursor])
            {
                //printf(1,"no match\n");
                isMatch = 0;
            }

        }
        //printf(1,"=======\n");
        if(isMatch == 1)
        {
            count++; 
        }
        isMatch = 1;
    }
    //printf(1,"count: %d", count);
    return count;
}

// replaces characters of line with the given keyword
// key is the word to replace
/*
    Handle any length of strings to replace
    Algo
    1.) Find the spot of the match
    2.) store chars after the target word into temp vec
    3.) overrwrite match word with new word
    4.) recover old char from temp vec
*/
void replaceWord(myVector* line, char* key, char* newWord)
{
    //printf(1,"In replaceWord(); %d\n",strlen(key));
    uint i = 0,cursor = 0, keyLen = strlen(key);
    uint newLen = strlen(newWord);
    uint isMatch = 1; // Assume we have a match

    //uint c = 0;
    for(;i < line->size; i++)
    {
        for(cursor = 0;cursor < keyLen; cursor++)
        {
            if(line->data[i + cursor] != key[cursor])
            {
                //printf(1,"no match\n");
                isMatch = 0;
            }

        }
        //printf(1,"=======\n");
        //printf(1,"isMatch: %d, ",isMatch);
        if(isMatch == 1)
        {
            //c++;
            //printf(1,"==========%d=========\n",c);
            
            myVector temp;
            myVectorConstructor(&temp, 0);
            //printf(1,"LineSize B4: %d\n",line->size);
            // cache chars that need to be recovered
            while(line->size > i)
            {
                myVecPush(&temp,myVecPop(line));
            }
    /*         printf(1,"Line size: %d\n",line->size);
            printf(1,"Temp Size: %d\n",temp.size );
            printf(1,"Current Line\n");
            printVec(line);
            printVec(&temp);
            printf(1,"Writing\n"); */
            // overrwrite
            for(cursor = 0; cursor < newLen; cursor++)
            {
                myVecPush(line,newWord[cursor]);
            }
            /* printf(1,"Line size: %d\n",line->size);
            printf(1,"Temp Size: %d\n",temp.size );
            printf(1,"Current Line\n");
            printVec(line);
            printVec(&temp);
            printf(1,"recovering\n"); */
            // recover old chars
            // apparently counting backwards is buggy
            // with a for loop
            cursor = temp.size - keyLen;
            while(cursor > 0)
            {
                //printf(1,"%d ",cursor);
                cursor--;
                myVecPush(line,temp.data[cursor]);
            }

            // Free up mem
            myVecDie(&temp);
            
        }

        //printf(1,"line size outside: %d\n",line->size);
        isMatch = 1;
    }
}

// ===========================================
// searches for key and new word
int SED(int fd, char* key, char* newWord)
{
    int numOccur = 0;
    //lineBuffer holds the characters being read from fd
    myVector lineBuffer; 
    // sets up my lineBuffer struct
    myVectorConstructor(&lineBuffer, 512);

    // Question: What happens if you read again?
    // Ans: it picks up where it left off from previous read
    //printf(1,"in: SED()\nkey: %s\n",key);

    // read(fd , buff, n bytes)
    // read() reads from fd, puts into buff for n bytes
    char* readBuffer = malloc(sizeof(char));

    //read while there are things to read
    /*
        reading from STDIN:
            a end of transmission (EOT) character signals
            the end of STDIN. in UNIX, its defaulted to be
            "ctrl + d". read() can pick this up and stop the
            loop
        reading from file:
            the file would have a end of file (EOF) character
            that signals the end of the file
            read() also detects that

        read() will return > 0 as long there are bytes to read
        unless otherwise
    */
    while(read(fd,readBuffer, sizeof(char)) > 0)
    {
        if(*readBuffer == '\n')
        {
            printVec(&lineBuffer);
            numOccur += countKeyword(&lineBuffer,key);
            replaceWord(&lineBuffer,key,newWord);
            printVec(&lineBuffer);
            myVecClear(&lineBuffer);
        }
        myVecPush(&lineBuffer,*readBuffer);
        //printf(1,"%c",*readBuffer);
    }

    // Cleanup
    myVecDie(&lineBuffer);
    printf(1,"read success: total match = %d\n",numOccur);
    return 0;
}

// argc = argument count, argv[] = argument parameters
// Note: "sed" also counts as a arg
int main(int argc,char* argv[])
{
    
    //printf(1,"%d\n", argc);
    //printf(1,"%d\n", sizeof(argv));

    int fd;

   /*  printf(1,"Arguments: ");
    for(i = 0; i < argc;++i)
    {
        printf(1,"%s",argv[i]);
    }
    printf(1,"\n"); */

    // if there is zero arguments, "sed counts as 1"
    if(argc <= 1)
    {
        SED(0,"the","xyz");
        exit();
    }
    // Read from a single file
    if(argc == 2)
    {
        if((fd = open(argv[1],0)) < 0)
        {
            printf(1,"sed: can't open %s\n", argv[1]);
        }
        SED(fd, "the","xyz");
        exit();
    }

    // Bonus points / Part 4
    // Allows for input
    // parmeter order: 
    // sed -TOBEREPLACED -REPLACEDWITH Filename
    if(argc == 4)
    {
        if((fd = open(argv[3],0)) < 0)
        {
            printf(1,"sed: can't open %s\n", argv[1]);
        }
        SED(fd,argv[1]+1,argv[2]+1);

        exit();
    }
    // kills the current process (from xv6)
    exit();
}