#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define SIZE_INIT 50                            //size of init memory, delete it later

/*structure for linked list of free memory chunks
    size of this struct: 8 */
typedef struct FREE_MCHUNK {
    int mchunk_size;                            //size of actual chunk of memory without size of header
    //void *ptr_data;                           //pointer to data saved in memory block (chunk)
    struct FREE_MCHUNK *next;                   //next free chunk of memory
} FREE_MCHUNK;

#define HEAD_OFFSET sizeof(FREE_MCHUNK)          //size of struct


static FREE_MCHUNK *h_free_mchunks = NULL;      //pointer to the beginning of the initialized memory

//---------------------------------------------------------------------//
//                                                                     //
//       MAIN 4 FUNCTIONS                                              //
//                                                                     //
//---------------------------------------------------------------------//

/*memory allocation function
function input: size of wanted allocated memory*/
void *memory_alloc(unsigned int size) {
    printf("\nMemory of %dB starting allocating--------------\n", size);
    FREE_MCHUNK *previous = NULL, *current = NULL; //temporary pointers to blocks of memory
    void *allocated_memory;
    current = h_free_mchunks;
    printf("size alloc:%d\n", size);

    while((current->mchunk_size < size) && (current->next != NULL)) { //find free block for wanted size
        previous = current;
        current = current->next;
    }

    if(current->mchunk_size == size) {
        current->mchunk_size = ~current->mchunk_size + 1;
        printf("original ptr alloc:%p\n", current);
        allocated_memory = (void*) ++current;    //jump to address for user data (after block header)
        printf("++ptr alloc:%p\n", allocated_memory);
        printf("Allocated memory is same size as initialized memory.");
        return allocated_memory;

    } else if (current->mchunk_size > (size + HEAD_OFFSET)) {
        printf("Allocated block fit in init memory with a split\n");
        //return allocated_memory;

    } else {
        allocated_memory = NULL;
        printf("Error:\tnot enough memory\n");
        return allocated_memory;
    }
    return NULL;
}

//memory free function
int memory_free(void *valid_ptr) {

}

//check
int memory_check(void *ptr){

}

void memory_init(void *ptr, unsigned int size) {
    printf("Memory starting initializing-------------------\n");
    h_free_mchunks = ptr;                       //pointer to first memory address
    printf("start init: %p %p\n", ptr, h_free_mchunks);

    size = size - HEAD_OFFSET;                  //real size for user data
    h_free_mchunks->mchunk_size = (int)size;
    h_free_mchunks->next = NULL;
    printf("size init:%d\n", h_free_mchunks->mchunk_size);
    printf("Memory initialized-----------------------------\n");
}

//---------------------------------------------------------------------//
//                                                                     //
//       SECONDARY FUNCTIONS                                           //
//                                                                     //
//---------------------------------------------------------------------//

void divide_and_conquer(FREE_MCHUNK * free_mchunks, unsigned int size){
    printf("Memory spliting--------------------------------\n");
    FREE_MCHUNK *new_list = (void*)((void*)free_mchunks + size + HEAD_OFFSET);
    new_list->mchunk_size = (int)(free_mchunks->mchunk_size - size - HEAD_OFFSET);
    printf("%d\n", new_list->mchunk_size);
    new_list->next=free_mchunks->next;
    free_mchunks->mchunk_size=size;
    //free_mchunks->free=0;
    free_mchunks->next=new_list;
    //vyhodit z litu
}

//---------------------------------------------------------------------//
//                                                                     //
//       MAIN                                                          //
//                                                                     //
//---------------------------------------------------------------------//

int main() {
    char region[SIZE_INIT];
    memory_init(region, SIZE_INIT);
    char * pointer = (char*) memory_alloc(42);
//    if (pointer)
//        memset(pointer, 0, 10);
//    if (pointer)
//        memory_free(pointer);
    return 0;
}
