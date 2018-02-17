#include "types.h"
#include "stat.h"
#include "user.h"

/*
    Author: David Zheng
    Date: Jan 31, 2018

    Code to simulate a vector from C++
    *** Working ATM ***
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
int main()
{
    myVector test;
    myVectorConstructor(&test,2);
    printf(1,"vec: %d,%d\n", test.size, test.capacity);

    char* string = "wow lols";
    printf(1,"String: %s, Length: %d\n", string,strlen(string));
    int i = 0;
    for(; i < strlen(string); i++)
    {
        myVecPush(&test,string[i]);
    }
    printVec(&test);
    printf(1,"vec: %d,%d\n", test.size, test.capacity);
    printf(1,"%c\n",myVecPop(&test));
    printVec(&test);
    printf(1,"vec: %d,%d\n", test.size, test.capacity);

    myVecClear(&test);
    printVec(&test);
    printf(1,"vec: %d,%d\n", test.size, test.capacity);

    exit();
}