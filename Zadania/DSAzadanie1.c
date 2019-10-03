#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h> //offsetof

#define SIZE_INIT 50 //size of init memory, delete it later

//structure for linked list of free memory chunks
//size of this struct: 16
typedef struct MEMORY_CHUNK{
    //int prev_mchunk_size; //size of previous free chunk of memory
    int mchunk_size;      //size of actual chunk of memory with size of header
    void *data;           //pointer to data saved in memory block (chunk)
    struct MEMORY_CHUNK *next; //next chunk of memory
} MEMORY_CHUNK;


//struct for pointer to beginning of the initialized memory
//size of this struct: 12
typedef struct {
    void * beginning;
    void * ending;
    unsigned int size; //size of initialized memory
    MEMORY_CHUNK *free_memory_chunks; //header to memory chunks
}  INIT_MEMORY_HEADER;

//pointer to the beginning of the initialized memory
static INIT_MEMORY_HEADER * memory_beginning = NULL;

//---------------------------------------------------------------------//
//                                                                     //
//       MAIN 4 FUNCTIONS                                              //
//                                                                     //
//---------------------------------------------------------------------//

//memory allocation function
//function input: size of wanted memory, which will be used b
void *memory_alloc(unsigned int size) {
    MEMORY_CHUNK *previous = NULL, *current = NULL;
    void * allocated_memory;

    current = memory_beginning->free_memory_chunks;

    while(((current->mchunk_size<size) || (current->mchunk_size<0))&&(current->next!=NULL)){
        previous = current;
        current = current->next;
        printf("One block checked\n");
    }

    if(current->mchunk_size == size) {
        current->mchunk_size *= -1;
        allocated_memory = (void*)++current;
        printf("Allocated memory is same size as initialized memory.");
        return allocated_memory;
    } else if (current->mchunk_size>size+ sizeof(MEMORY_CHUNK)) {
        printf("Allocated block fit in init memory with a split\n");
        //return allocated_memory;
    } else {
        allocated_memory = NULL;
        printf("Error: not enough memory\n");
        return allocated_memory;
    }



}

//memory free function
int memory_free(void *valid_ptr) {

}

//check
int memory_check(void *ptr){

}

void memory_init(void *ptr, unsigned int size) {
    memory_beginning = ptr; //pointer to first memory address
    memory_beginning->beginning = ptr;
    memory_beginning->free_memory_chunks = ptr; //pointer to first free memory address

    printf("start init: %p %p %p\n", ptr, memory_beginning->beginning,memory_beginning->free_memory_chunks);

    memory_beginning->ending = (memory_beginning->beginning + size) - 1;
    printf("end init:%p\n", memory_beginning->ending);

    size = size - sizeof(INIT_MEMORY_HEADER); //real size for data

    memory_beginning->free_memory_chunks->next = NULL;
    memory_beginning->size = size; //size of memory
    memory_beginning->free_memory_chunks->mchunk_size = (int)size; //size of free memory
    printf("size init:%d %d\n", memory_beginning->free_memory_chunks->mchunk_size, memory_beginning->size);
    printf("Memory initialized\n");
}

//---------------------------------------------------------------------//
//                                                                     //
//       SECONDARY FUNCTIONS                                           //
//                                                                     //
//---------------------------------------------------------------------//



//---------------------------------------------------------------------//
//                                                                     //
//       MAIN                                                          //
//                                                                     //
//---------------------------------------------------------------------//
int main() {
    char region[SIZE_INIT];
    memory_init(region, SIZE_INIT);
    //char* pointer = (char*) memory_alloc(39);
//    if (pointer)
//        memset(pointer, 0, 10);
//    if (pointer)
//        memory_free(pointer);
    return 0;
}