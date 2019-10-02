//
// Created by emari on 2. 10. 2019.
//
#include <stdio.h>
#define SIZE 50


//structure for memory chunks
typedef struct {
    int prev_mchunk_size; //size of previous free chunk of memory
    int mchunk_size;      //size of actual chunk of memory with size of header
    void *data;           //pointer to data saved in memory block (chunk)
    struct MEMORY_CHUNK *next;
} MEMORY_CHUNK;


//struct for pointer to beginning of the initialized memory
typedef struct {
    void * beginnig;
    void * ending;
    int size;
    MEMORY_CHUNK memoryChunks;
}  INIT_MEMORY_BEGIN;

static INIT_MEMORY_BEGIN * memory_beginning = NULL;

void memory_init(void *ptr, unsigned int size) {

    memory_beginning->beginnig = ptr;
    memory_beginning = ptr;
    memory_beginning->()ending = ptr + size;

    printf("%p ", memory_beginning->beginnig);
    printf("%p ", memory_beginning);
    printf("%p ", memory_beginning->beginnig);

}

int main() {
    char region[SIZE];
    memory_init(region, SIZE);
    return 0;
}

