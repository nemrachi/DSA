#include <stdio.h>
//
//typedef struct MEMORY_CHUNK{
//    int prev_mchunk_size; //size of previous free chunk of memory
//    int mchunk_size;      //size of actual chunk of memory with size of header
//    void *data;           //pointer to data saved in memory block (chunk)
//    struct MEMORY_CHUNK *next;
//} MEMORY_CHUNK;
//
//typedef struct {
//    unsigned int size;
//    MEMORY_CHUNK *free_memory_chunks;
//}  INIT_MEMORY;
//
//static INIT_MEMORY * memory_beginning = NULL;
//
//void foo(unsigned int size) {
//    size = (int)size - (int)sizeof(INIT_MEMORY);
//    memory_beginning->size = size;
//    memory_beginning->free_memory_chunks->mchunk_size = (int)size;
//    memory_beginning->free_memory_chunks->next = NULL;
//    printf("%d\n", memory_beginning->free_memory_chunks->mchunk_size);
//}
//
int main() {
    int x = 33;
    int z = 44;
    int *p;

    p = &x;

    printf("%d ", *p);
    printf("%p ", p);
    printf("%p\n", &p);

    p = &z;

    printf("%d ", *p);
    printf("%p ", p);
    printf("%p ", &p);

    return 0;
}
//
