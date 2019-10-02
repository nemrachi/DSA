#include <stdio.h>
#include <string.h>

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

//pointer to the beginning of the initialized memory
static INIT_MEMORY_BEGIN * memory_beginning = NULL;

//---------------------------------------------------------------------//
//                                                                     //
//       MAIN 4 FUNCTIONS                                              //
//                                                                     //
//---------------------------------------------------------------------//

//memory allocation function
//function input: size of wanted memory, which will be used b
void *memory_alloc(unsigned int size) {

}

//memory free function
int memory_free(void *valid_ptr) {

}

//check
int memory_check(void *ptr){

}

void memory_init(void *ptr, unsigned int size) {
    unsigned char *ptr_to_init_m = NULL;

    memory_beginning = ptr;
    memory_beginning->ending = 0;
    memory_beginning->mchunk_size = size;

    while (size > 0) {
        *ptr_to_data = 0;
        size--;
        *ptr_to_data++;
    }
}

//---------------------------------------------------------------------//
//                                                                     //
//       SECONDARY FUNCTIONS                                           //
//                                                                     //
//---------------------------------------------------------------------//

int main() {
    char region[SIZE];
    memory_init(region, SIZE);
    char* pointer = (char*) memory_alloc(10);
    if (pointer)
        memset(pointer, 0, 10);
    if (pointer)
        memory_free(pointer);
    return 0;
}