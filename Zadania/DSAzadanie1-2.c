#include <stdio.h>
#include <string.h>
#include <stdint.h>
//---------------------------------------------------------------------//
//Poznamka ku komentarom:                                              //
//                                                                     //
//Komentar ku kodu sa nachadza bud v tom istom riadku daneho           //
//kodu alebo nad kodom                                                 //
//                                                                     //
//---------------------------------------------------------------------//
#define SIZE_INIT 1000 //velkost inicializovanej pamate
#define MALLOC_SIZE 3 //velkost pamate, ktoru chce pouzivatel alokovat
#define INT_OFFSET sizeof(int) //velkost int typu (4)
#define CHAR_OFFSET sizeof(char) //velkost char typu (1)

/*struktura ("hlavicka") pre linked list blokov volnej pamate
    momentalna velkost struktury: 8 */
typedef struct BLOCK_HEAD {
    int mchunk_size;  //velkost volneho bloku, ktoru moze pouzivatel vyuzit pri alokacii
    int next_block_offset; //offset k dalsiemu volnemu bloku v pamati
                           //cize vlastne pocet bajtov od danej hlavicky k dalsej hlavicke volneho bloku
} BLOCK_HEAD;

/*struktura ("hlavicka") pre alokovanu pamat
    momentalna velkost struktury: 4 */
typedef struct ALLOC_HEAD {
    int alloc_size; //udavana v minusovej hodnote, kvoli oznaceniu, ze dany blok pamate je zabraty
                    //absolutna hodnota predstavuje velkost alokovanej pamate
} ALLOC_HEAD;

/*struktura ("hlavicka") pre celkovu inicializovanu pamat
    momentalna velkost struktury: 4 */
typedef struct MEM_HEAD {
    int first_free_offset; //offset k prvemu volnemu bloku v pamati
                           //cize vlastne pocet bajtov od danej hlavicky k dalsej hlavicke volneho bloku
} MEM_HEAD;

static MEM_HEAD *memory_head = NULL; //ukazovatel na prvy volny blok

//---------------------------------------------------------------------//
//                                                                     //
//       SECONDARY FUNCTIONS                                           //
//                                                                     //
//---------------------------------------------------------------------//

/*funkcia vrati NULL hodnotu pre alokovanu pamat
    atribut funkcie: ukazovatel na alokovany blok pamate*/
void *allocated_memory_as_NULL(void *alloc_mem) {
    alloc_mem = NULL;
    printf("Error:\tnot enough memory\n"); //info pre pouzivatela
    return alloc_mem;
}

/*funkcia vrati ukazovatel typu int
    atribut funkcie: ukazovatel typu char*/
int *get_on_memory_address(char *memory){
    return (int*)memory;
}

/*funkcia vrati int hodnotu zapisanu na adrese, kde odkazuje ukazovatel typu char
    atribut funkcie: ukazovatel typu char*/
int get_int_value_on(char *memory){
    return *get_on_memory_address(memory);
}

//---------------------------------------------------------------------//
//                                                                     //
//       MAIN 4 FUNCTIONS                                              //
//                                                                     //
//---------------------------------------------------------------------//

/*funckia na alokaciu pamate
    atribut funkcie: velkost bloku pamate, ktoru chce pouzivatel alokovat*/
void *memory_alloc(unsigned int size) {
    printf("\nMemory of %dB starting allocating---------------\n", size);
    //docasne ukazovatele na volne bloky pamate, s ktorymi sa bude vo funkcii pracovat
    BLOCK_HEAD *previous = NULL, *current = NULL;
    ALLOC_HEAD *alloc_head = NULL; //hlavicka pre alokovanu pamat
    void *allocated_memory = NULL; //ukazovatel na blok pamate, ktory bude alokovany pre pouzivatela
    //velkost prveho volneho bloku v pamati
    int curr_mchunk_size = get_int_value_on((char*)((char*)memory_head + memory_head->first_free_offset));
    //offset (na dalsi volny blok) ulozeny v prvom volnom bloku
    int next_block_offset_val = get_int_value_on(
            ((char*)memory_head + memory_head->first_free_offset + INT_OFFSET));

    printf("get_int_value_on:\n(int)%d (char)%d\n", next_block_offset_val, (char)next_block_offset_val);

    current = (BLOCK_HEAD*)((char*)memory_head + memory_head->first_free_offset); //ukazovatel na prvy volny blok

    if ((char)get_int_value_on((char*)current) != 0){//ak sa ukazovatel na prvy volny blok nenachadza na konci pamate...
        current->mchunk_size = curr_mchunk_size; //nastavi hlavicku prveho volneho bloku
        current->next_block_offset = next_block_offset_val; //nastavi hlavicku prveho volneho bloku

        //hlada vhodny volny blok pre alokaciu - FIRST FIT
        while(((current->mchunk_size) < (size)) &&
        ((char)get_int_value_on((char*)((char*)current + current->next_block_offset)) != 0)) {
            previous = current; //ulozi si terajsi volny blok ako predosli a bude sa posuvat na dalsi volny blok
            next_block_offset_val = get_int_value_on(
                    (char*)((char*)current + current->next_block_offset + INT_OFFSET));
            printf("vo while next_block_offset_val: (int)%d (char)%d\n",
                    next_block_offset_val, (char)next_block_offset_val);
            curr_mchunk_size = get_int_value_on((char*)((char*)current + current->next_block_offset));
            current = (BLOCK_HEAD*)((char*)previous + previous->next_block_offset);
            current->mchunk_size = curr_mchunk_size;
            current->next_block_offset = next_block_offset_val;
        }

        if (((current->mchunk_size) < (size)) &&
            ((char) get_int_value_on((char *) ((char *) current + current->next_block_offset)) == 0)) {
            return allocated_memory_as_NULL(allocated_memory);
        }

    } else { //ak sa ukazovatel na prvy volny blok nachadza na konci pamate...
        return allocated_memory_as_NULL(allocated_memory); //...vrati NULL
    }

    ///skontrolovat este toto ak je prev = NULL a ak nie je + spravit poriadne komentare v malloc funkcii
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
        next_block_offset_val = (char)get_int_value_on((char *) ((char *) current + current->next_block_offset));
        printf("%d\n", next_block_offset_val);

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
        return allocated_memory_as_NULL(allocated_memory);
    }
}

//memory free function
int memory_free(void *valid_ptr) {

}

//check
int memory_check(void *ptr){

}

/*funckia na inicializaciu pamate
    atribut funkcie: ukazovatel na volny blok pamate, velkost inicializovanej pamate*/
void memory_init(void *ptr, unsigned int size) {
    printf("Memory starting initializing-------------------\n");
    memory_head = (MEM_HEAD*)ptr; //ulozenie ukazovatela na volny blok pamate do globalneho ukazovatela
    BLOCK_HEAD *free_block = (BLOCK_HEAD*)((char*)ptr + (int)INT_OFFSET); //ukazovatel na hlavicku prveho volneho bloku
    //offset od hlavicky inicializovanej pamate ku hlavicke prveho volneho bloku pamate
    memory_head->first_free_offset = (char*)free_block - (char*)memory_head;
    //celkova velkost inicializovanej pamate - 2*velkost hlaviciek - velkost "paticky"(konca) pamate
    free_block->mchunk_size = (int)size - 2*(int)INT_OFFSET - (int)CHAR_OFFSET;
    free_block->next_block_offset = (int)(size - CHAR_OFFSET - INT_OFFSET); //offset ku koncu inicializovanej pamate
    //nastavenie "paticky" na hodnotu 0
    memset((char*)free_block + free_block->next_block_offset, 0, 1);

/*  printf("memory_h ptr: %p\n", memory_head);
      printf("last address: %p\n", (char*)free_block + free_block->next_block_offset);
      printf("last address val: %d\n", (char)get_int_value_on(
              (char *) free_block + free_block->next_block_offset));
      printf("free_block ptr: %p\n", free_block);
      printf("size free_block for user: %d\n", free_block->mchunk_size);
      printf("offset free_block to end: %d\n", free_block->next_block_offset); */
    printf("Memory initialized-----------------------------\n");
}

//---------------------------------------------------------------------//
//                                                                     //
//       MAIN                                                          //
//                                                                     //
//---------------------------------------------------------------------//

int main() {
    char region[SIZE_INIT]; //blok pamate, na ktorom bude realizovana alokacia
    memory_init(region, SIZE_INIT); //inicializacia bloku pamate

    char *pointer = (char*) memory_alloc(MALLOC_SIZE); //alokacia
    if (pointer)
        memset(pointer, 0, MALLOC_SIZE);

    char *pointer1 = (char*) memory_alloc(7); //alokacia
    if (pointer1)
        memset(pointer1, 0, 7);

    char *pointer2 = (char*) memory_alloc(2); //alokacia
    if (pointer2)
        memset(pointer2, 0, 2);

    char *pointer3 = (char*) memory_alloc(7); //alokacia
    if (pointer3)
        memset(pointer3, 0, 7);

//    for (int i = 0; i < SIZE_INIT; i++) { //vypis pamate
//        printf("\n%d %p", region[i], &region[i]);
//    }

//    if (pointer)
//        memory_free(pointer);
    return 0;
}