#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define SIZE_INIT 30                            //velkost inicializovanej pamate
#define MALLOC_SIZE 3                          //velkost pamate, ktoru chce pouzivatel alokovat
#define INT_OFFSET sizeof(int)                  //velkost int typu
#define CHAR_OFFSET sizeof(char)

/*struktura ("hlavicka") pre linked list blokov volnej pamate
    momentalna velkost: 4 */
typedef struct BLOCK_HEAD {
    int mchunk_size;
    int next_block_offset;
} BLOCK_HEAD;

typedef struct ALLOC_HEAD {
    int alloc_size;
} ALLOC_HEAD;

typedef struct MEM_HEAD {
    int first_free_offset;
} MEM_HEAD;

static MEM_HEAD *memory_head = NULL;      //pointer na zaciatok volnych blokov
//ukazuje na hlavicku pamate, nie na volnu pouzitelnu pamat

//---------------------------------------------------------------------//
//                                                                     //
//       SECONDARY FUNCTIONS                                           //
//                                                                     //
//---------------------------------------------------------------------//

//funkcia vrati ukazovatel typu int na hlavicku
int *get_on_memory_address(char *memory){
    return (int*)memory;
}

//funkcia vráti hodnotu napísanu v hlavičke
int get_int_value_on(char *memory){
    return *get_on_memory_address(memory);
}

//funkcia vrati ukazovatel typu int na hlavicku
char *get_on_memory_address_char(char *memory){
    return (char*)memory;
}

//funkcia vráti hodnotu napísanu v hlavičke
char get_char_on(char *memory){
    return *get_on_memory_address_char(memory);
}

void * allocated_memory_is_NULL (void *alloc_mem) {
    alloc_mem = NULL;
    printf("Error:\tnot enough memory\n");
    return alloc_mem;
}



//---------------------------------------------------------------------//
//                                                                     //
//       MAIN 4 FUNCTIONS                                              //
//                                                                     //
//---------------------------------------------------------------------//

/*funckia na alokaciu pamate
atributy: velkost pamate, ktoru chce pouzivatel vyuzivat*/
void *memory_alloc(unsigned int size) {
    printf("\nMemory of %dB starting allocating--------------\n", size);
    BLOCK_HEAD *previous = NULL, *current = NULL;                    //docasne ukazovatele na volnu pamat
    ALLOC_HEAD *alloc_head = NULL;
    void *allocated_memory = NULL;                                           //ukazovatel na alokovanu pamat

    int curr_mchunk_size = get_int_value_on((char *) ((char *) memory_head + memory_head->first_free_offset));
    int next_block_offset_val = get_int_value_on(
            (char *) ((char *) memory_head + memory_head->first_free_offset + INT_OFFSET));

    current = (BLOCK_HEAD*)((char*)memory_head + memory_head->first_free_offset);

    if ((char) get_int_value_on((char *) current) != 0){ //mozno ((current->mchunk_size) > (size))
        current->mchunk_size = curr_mchunk_size;
        current->next_block_offset = next_block_offset_val;
        //first fit
        while(((current->mchunk_size) < (size)) &&
        ((char) get_int_value_on((char *) ((char *) current + current->next_block_offset)) != 0)) { //najde prvy hodiaci sa volny blok
            previous = current;
            next_block_offset_val = get_int_value_on((char*) ((char*) current + current->next_block_offset + INT_OFFSET));
            curr_mchunk_size = get_int_value_on((char *) ((char *) current + current->next_block_offset));
            current = (BLOCK_HEAD*)((char*)current + current->next_block_offset);
            current->mchunk_size = curr_mchunk_size;
            current->next_block_offset = next_block_offset_val;
        }

        if (((current->mchunk_size) < (size)) &&
            ((char) get_int_value_on((char *) ((char *) current + current->next_block_offset)) == 0)) {
            return allocated_memory_is_NULL(allocated_memory);
        }
    } else {
        return allocated_memory_is_NULL(allocated_memory);
    }

    alloc_head = (ALLOC_HEAD*)current;
    allocated_memory = (char*)((char*)alloc_head + (int)INT_OFFSET);
    int alloc_size = (int) size;                                       //pretypovanie zadanej velkosti
    int remain_size = current->mchunk_size - alloc_size;//kolko zostane v bloku po alokacii
    printf("current: %d %d %p\n",current->mchunk_size, current->next_block_offset, current);
    printf("remain: %d\n", remain_size);
    printf("next val offset : %d\n=======================\n",
           (char) get_int_value_on(((char *) ((char *) current + current->next_block_offset))));

    if (remain_size >= 2*(int)INT_OFFSET) { //OK
        current = (BLOCK_HEAD*)((char*)current + alloc_size + (int)INT_OFFSET);
        current->mchunk_size = remain_size - (int)INT_OFFSET;
        current->next_block_offset = remain_size;
        printf("after malloc: %d %d %p\n",current->mchunk_size, current->next_block_offset, current);

        alloc_head->alloc_size = (-1)*alloc_size;

        if (previous == NULL) {
            memory_head->first_free_offset = (char*)current - (char*)memory_head;
        } //else
        printf("After allocation, there remains extra space in chunk. (%d)\n", current->mchunk_size);
        printf("memory_head->first_free_offset: %d\n", memory_head->first_free_offset);
        return allocated_memory;

    } else if ((remain_size < 2*(int)INT_OFFSET) && (remain_size >= 0)){
        printf("--------------current: %p %d\n", current, current->next_block_offset);
        next_block_offset_val = get_int_value_on((char *) ((char *) current + current->next_block_offset));
        printf("%d\n", (char)next_block_offset_val);

        alloc_head->alloc_size = (-1)*current->mchunk_size;

        current = (BLOCK_HEAD*)((char*)current + current->next_block_offset);

        if ((char)next_block_offset_val == 0) {
            printf("current is at the end.\n");
        } else {
            current->mchunk_size = (char)next_block_offset_val;
            current->next_block_offset = get_int_value_on(
                    (char *) ((char *) current + current->next_block_offset + (int) INT_OFFSET));
        }

        if (previous == NULL) {
            memory_head->first_free_offset = (char *) current - (char *) memory_head;
        } //else

        printf("memory_head: %d %p\n", memory_head->first_free_offset, memory_head);
        printf("%d\n", (char)next_block_offset_val);
        printf("After allocation, there not remains extra space in chunk.\n"
               "Allocated memory consumed whole block. (%d)\n\n", (-1)*alloc_head->alloc_size);
        return allocated_memory;

    } else {
        return allocated_memory_is_NULL(allocated_memory);
    }
}

//memory free function
int memory_free(void *valid_ptr) {

}

//check
int memory_check(void *ptr){

}

void memory_init(void *ptr, unsigned int size) {
    printf("Memory starting initializing-------------------\n");
    memory_head = (MEM_HEAD*)ptr;                       //ukazovatel na volny blok pamate
    BLOCK_HEAD *free_block = (BLOCK_HEAD*)((char*)ptr + (int)INT_OFFSET);

    memory_head->first_free_offset = (char*)free_block - (char*)memory_head;
    free_block->mchunk_size = (int)size - 2*(int)INT_OFFSET - (int)CHAR_OFFSET;
    free_block->next_block_offset = (char)(size - CHAR_OFFSET - INT_OFFSET);
    memset((char*)free_block + free_block->next_block_offset, 0, 1);

    printf("memory_h ptr: %p\n", memory_head);
    printf("last address: %p\n", (char*)free_block + free_block->next_block_offset);
    printf("last address val: %d\n", (char) get_int_value_on((char *) free_block + free_block->next_block_offset));
    printf("free_block ptr: %p\n", free_block);
    printf("size free_block for user: %d\n", free_block->mchunk_size);
    printf("offset free_block to end: %d\n", free_block->next_block_offset);
    printf("Memory initialized-----------------------------\n");
}

//---------------------------------------------------------------------//
//                                                                     //
//       MAIN                                                          //
//                                                                     //
//---------------------------------------------------------------------//

int main() {
    char region[SIZE_INIT];
    memory_init(region, SIZE_INIT);

//    for (int i = 0; i < SIZE_INIT; ++i) {
//        printf("%d %p\n", region[i], &region[i]);
//    }

    char * pointer = (char*) memory_alloc(MALLOC_SIZE);
    if (pointer)
        memset(pointer, 0, MALLOC_SIZE);

    char * pointer1 = (char*) memory_alloc(2);
    if (pointer1)
        memset(pointer1, 0, 2);

    char * pointer2 = (char*) memory_alloc(2);
    if (pointer2)
        memset(pointer2, 0, 2);

    char * pointer3 = (char*) memory_alloc(7);
    if (pointer3)
        memset(pointer3, 0, 7);

    for (int i = 0; i < SIZE_INIT; i++) {
        printf("%d %p\n", region[i], &region[i]);
    }

//    if (pointer)
//        memory_free(pointer);
    return 0;
}