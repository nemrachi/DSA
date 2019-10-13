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
#define SIZE_INIT 30 //velkost inicializovanej pamate
#define MALLOC_SIZE 8 //velkost pamate, ktoru chce pouzivatel alokovat
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

void draw_memory(char *region) {
    for (int i = 0; i < SIZE_INIT; i++) {
        printf("%d %p\n", region[i], &region[i]);
    }
}

void *get_last_mem_address() {
    BLOCK_HEAD *prev = NULL;
    BLOCK_HEAD *curr = (BLOCK_HEAD*)((char*)memory_head + memory_head->first_free_offset);
    int curr_mchunk_size, next_block_offset_val;

    if ((char)get_int_value_on((char*)curr) == 0) {
        return curr;
    }

    while(1) {
        if (((char)get_int_value_on((char*)((char*)curr + curr->next_block_offset)) != 0) &&
            ((char)get_int_value_on((char*)(curr)) != 0)) {
            prev = curr;
            next_block_offset_val = get_int_value_on(
                    (char*)((char*)curr + curr->next_block_offset + INT_OFFSET));
            curr_mchunk_size = get_int_value_on((char*)((char*)curr + curr->next_block_offset));
            curr = (BLOCK_HEAD*)((char*)prev + prev->next_block_offset);
            curr->mchunk_size = curr_mchunk_size;
            curr->next_block_offset = next_block_offset_val;
        } else {
            return (char*)curr + curr->next_block_offset;
        }
    }
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
    char * next = NULL;

    //velkost prveho volneho bloku v pamati
    int curr_mchunk_size = get_int_value_on(((char*)memory_head + memory_head->first_free_offset));
    int curr_block_offset_val;

    if ((char)curr_mchunk_size != 0){//ak sa ukazovatel na prvy volny blok nenachadza na konci pamate...
        //offset (na dalsi volny blok) ulozeny v prvom volnom bloku

        current = (BLOCK_HEAD*)((char*)memory_head + memory_head->first_free_offset); //ukazovatel na prvy volny blok

        printf("next_block_val_offset: (int)%d\n", current->next_block_offset);

        //hlada vhodny volny blok pre alokaciu - FIRST FIT
        while(((current->mchunk_size) < (size)) &&
              ((char)get_int_value_on(((char*)current + current->next_block_offset)) != 0)) {
            previous = current; //ulozi si terajsi volny blok ako predosli a bude sa posuvat na dalsi volny blok
//            curr_block_offset_val = get_int_value_on(
//                    (char*)((char*)current + current->next_block_offset + INT_OFFSET));
            printf("vo while v alloku");
//            curr_mchunk_size = get_int_value_on((char*)((char*)current + current->next_block_offset));
            current = (BLOCK_HEAD*)((char*)previous + previous->next_block_offset);
        }

        if (((current->mchunk_size) < (size)) &&
            ((char) get_int_value_on(((char *) current + current->next_block_offset)) == 0)) {
            return allocated_memory_as_NULL(allocated_memory);
        }

    } else { //ak sa ukazovatel na prvy volny blok nachadza na konci pamate...
        return allocated_memory_as_NULL(allocated_memory); //...vrati NULL
    }

    alloc_head = (ALLOC_HEAD*)current;
    int alloc_size = (int)size; //pretypovanie zadanej velkosti
    int remain_size = current->mchunk_size - alloc_size;//kolko zostane v bloku po alokacii aj pre hlavicku volneho bloku
    printf("current: %d %d %p\n",current->mchunk_size, current->next_block_offset, current);
    printf("remain: %d\n", remain_size);
    printf("next val offset : %d\n=======================\n",
           (char)get_int_value_on(((char*)current + current->next_block_offset)));

    if (remain_size >= 2*(int)INT_OFFSET) { //NIE OK
        next = (char *)((char*)current + current->next_block_offset);
        current = (BLOCK_HEAD*)((char*)current + alloc_size + (int)INT_OFFSET);
        current->mchunk_size = remain_size - (int)INT_OFFSET;
        //tuto je nejaka chyba
        current->next_block_offset = (char*)next - (char*)current;
        printf("after malloc: %d %d %p\n",current->mchunk_size, current->next_block_offset, current);

        alloc_head->alloc_size = (-1)*alloc_size;
        allocated_memory = (char*)((char*)alloc_head + (int)INT_OFFSET);


        if (previous == NULL) {
            memory_head->first_free_offset = (char*)current - (char*)memory_head;
        } else {
            previous->next_block_offset = (char*)current - (char*)previous;
        }

        printf("After allocation, there remains extra space in chunk. (%d)\n", current->mchunk_size);
        printf("memory_head: %d %p\n", memory_head->first_free_offset, memory_head);
        return allocated_memory;

    } else if ((remain_size < 2*(int)INT_OFFSET) && (remain_size >= 0)){
        printf("current: %p %d--------------\n", current, current->next_block_offset);
        next = (char *)((char*)current + current->next_block_offset);

        if ((char)get_int_value_on(next) == 0) {
            printf("memory is at the end.\n");
            alloc_head->alloc_size = (-1)*current->mchunk_size;
            allocated_memory = (char*)((char*)alloc_head + (int)INT_OFFSET);
        } else {
            alloc_head->alloc_size = (-1)*current->mchunk_size;
            allocated_memory = (char*)((char*)alloc_head + (int)INT_OFFSET);
            current = (BLOCK_HEAD*)((char*)current + current->next_block_offset);
        }


        if (previous == NULL) {
            memory_head->first_free_offset = (char*)next - (char*)memory_head;
        } else {
            previous->next_block_offset = (char*)next - (char*)previous;
        }

        printf("After allocation, there not remains extra space in chunk.\n"
               "Allocated memory consumed whole block. (%d)\n", (-1)*alloc_head->alloc_size);
        printf("memory_head: %d %p\n", memory_head->first_free_offset, memory_head);
        return allocated_memory;

    } else {
        return allocated_memory_as_NULL(allocated_memory);
    }
}

//funkcia zisti, ci ukazovatel v atribute funckie je platny, tzn. ci bol v nejakom z predchádzajúcich volaní vrátení
//funkciou memory_alloc a zatiaľ nebol uvoľnený funkciou memory_free
int memory_check(void *ptr){ //asi funguje, ale ak by dal uzivatel ptr+1, tak by to nebolo validne ale funkcia vypise 1
    int ch = get_int_value_on((char*)ptr - INT_OFFSET);
    void *last_maddr = get_last_mem_address();

    if (ptr == NULL) {
        return 0;
    } else if ((ptr < (void*)memory_head) && (ptr > last_maddr)) {
        return 0;
    } else if (ch > 0) {
        return 0;
    } else {
        return 1;
    }
}

//memory free function
int memory_free(void *valid_ptr) {
    printf("\nFREE %p\n", valid_ptr - INT_OFFSET);
    if (memory_check(valid_ptr)) {
        printf("FREE after check\n");
        BLOCK_HEAD *prev, *alloc_mem, *next, *free_block;
        int alloc_mem_size = get_int_value_on((char*)valid_ptr - INT_OFFSET);
        int next_block_offset_val, mchunk_size;
        alloc_mem_size *= -1;

        alloc_mem = (BLOCK_HEAD*)((char*)valid_ptr - INT_OFFSET);
        free_block = (BLOCK_HEAD*)((char*)memory_head + memory_head->first_free_offset);
        valid_ptr = NULL;

        if (&free_block->mchunk_size > &alloc_mem->mchunk_size) { //tuto viem, ze pred alokovanou pamatou nie je ziaden volny blok
            next = (BLOCK_HEAD*)((char*)alloc_mem + (int)INT_OFFSET + alloc_mem_size);

            if ((char)get_int_value_on((char*)next) > 0) { //asi ok
                memory_head->first_free_offset = (char*)alloc_mem - (char*)memory_head;
                alloc_mem->mchunk_size = next->mchunk_size + (int)INT_OFFSET + ((char*)next - (char*)alloc_mem - (int)INT_OFFSET);
                alloc_mem->next_block_offset = next->next_block_offset + ((char*)next - (char*)alloc_mem);
                printf("uvolnena pamet: >size %d >offset %d\n", alloc_mem->mchunk_size, alloc_mem->next_block_offset);
                printf("after free > memory_head offset: %d\n", memory_head->first_free_offset);

            } else {
                alloc_mem->mchunk_size = alloc_mem_size;
                memory_head->first_free_offset = (char*)alloc_mem - (char*)memory_head;
                alloc_mem->next_block_offset = (char*)free_block - (char*)alloc_mem;

                printf("uvolnena pamet: >size %d >offset %d\n", alloc_mem->mchunk_size, alloc_mem->next_block_offset);
                printf("after free > memory_head offset: %d\n", memory_head->first_free_offset);
            }

            return 0;

        } else { //tu je niekde velka chyba
            next = (BLOCK_HEAD*)((char*)alloc_mem + INT_OFFSET + alloc_mem_size);
            printf("free block je pred alokovanym blokom > next:%d\n", (char)get_int_value_on((char*)next));
            //urcit predchdzajuci

            prev = free_block;

            while ((BLOCK_HEAD*)((char*)free_block + free_block->mchunk_size + INT_OFFSET) != (BLOCK_HEAD*)alloc_mem) {
                prev = free_block; //ulozi si terajsi volny blok ako predosli a bude sa posuvat na dalsi volny blok
                next_block_offset_val = get_int_value_on(
                        (char*)((char*)free_block + free_block->next_block_offset + INT_OFFSET));
                mchunk_size = get_int_value_on((char*)((char*)free_block + free_block->next_block_offset));
                free_block = (BLOCK_HEAD*)((char*)prev + prev->next_block_offset);
                free_block->mchunk_size = mchunk_size;
                free_block->next_block_offset = next_block_offset_val;
                if (&free_block->mchunk_size > &alloc_mem->mchunk_size) {
                    free_block = NULL;
                    break;
                }

                if ((char)get_int_value_on((char*)free_block) == 0) {
                    break;
                }
            }

            //prev = free_block;

            if (get_int_value_on((char*)next) > 0) {

                if (free_block != NULL) {
                    prev = free_block;
                }

                alloc_mem->mchunk_size = next->mchunk_size + (int)INT_OFFSET + ((char*)next - (char*)alloc_mem - (int)INT_OFFSET);
                alloc_mem->next_block_offset = next->next_block_offset + ((char*)next - (char*)alloc_mem);

                if ((BLOCK_HEAD*)((char*)prev + (int)INT_OFFSET + prev->mchunk_size) == (BLOCK_HEAD*)alloc_mem) {
                    prev->mchunk_size = prev->mchunk_size + (int)INT_OFFSET + alloc_mem->mchunk_size;
                    prev->next_block_offset = alloc_mem->next_block_offset + ((char*)alloc_mem - (char*)prev) ;
                } else {
                    prev->next_block_offset = (char*)alloc_mem - (char*)prev;
//                    if (free_block != NULL) {
//                        free_block->mchunk_size = free_block->mchunk_size + alloc_mem_size + (int)INT_OFFSET;
//                    }
                }

                printf("uvolnena pamet: >size %d >offset %d\n", alloc_mem->mchunk_size, alloc_mem->next_block_offset);
                printf("after free > memory_head offset: %d\n", memory_head->first_free_offset);

                return 0;

            } else {

                if (free_block != NULL) {
                    prev = free_block;
                }

                alloc_mem->mchunk_size = alloc_mem_size;
                alloc_mem->next_block_offset = (char*)((char*)prev + prev->next_block_offset) - (char*)alloc_mem;

                if (free_block != NULL) {
                    free_block->mchunk_size = free_block->mchunk_size + alloc_mem_size + (int)INT_OFFSET;
                } else {
                    prev->next_block_offset = (char*)alloc_mem - (char*)prev;
                }
                printf("uvolnena pamet: >size %d >offset %d\n", alloc_mem->mchunk_size, alloc_mem->next_block_offset);
                printf("after free > memory_head offset: %d\n", memory_head->first_free_offset);

                return 0;
            }
        }
    } else {
        return 1;
    }
}

/*funckia na inicializaciu pamate
    atribut funkcie: ukazovatel na volny blok pamate, velkost inicializovanej pamate*/
void memory_init(void *ptr, unsigned int size) {
    memory_head = (MEM_HEAD*)ptr; //ulozenie ukazovatela na volny blok pamate do globalneho ukazovatela
    BLOCK_HEAD *free_block = (BLOCK_HEAD*)((char*)ptr + (int)INT_OFFSET); //ukazovatel na hlavicku prveho volneho bloku
    //offset od hlavicky inicializovanej pamate ku hlavicke prveho volneho bloku pamate
    memory_head->first_free_offset = (char*)free_block - (char*)memory_head;
    //celkova velkost inicializovanej pamate - 2*velkost hlaviciek - velkost "paticky"(konca) pamate
    free_block->mchunk_size = (int)size - 2*(int)INT_OFFSET - (int)CHAR_OFFSET;
    free_block->next_block_offset = (int)(size - CHAR_OFFSET - INT_OFFSET); //offset ku koncu inicializovanej pamate
    //nastavenie "paticky" na hodnotu 0
    memset((char*)free_block + free_block->next_block_offset, 0, 1);
}

//---------------------------------------------------------------------//
//                                                                     //
//       MAIN                                                          //
//                                                                     //
//---------------------------------------------------------------------//

int main() {
    char region[SIZE_INIT]; //blok pamate, na ktorom bude realizovana alokacia
    memory_init(region, SIZE_INIT); //inicializacia bloku pamate

    char *pointer = (char*) memory_alloc(4); //alokacia
    if (pointer)
        memset(pointer, 0, 4);

    char *pointer1 = (char*) memory_alloc(8); //alokacia
    if (pointer1)
        memset(pointer1, 0, 8);

    draw_memory(region);

    if (pointer1)
        memory_free(pointer1);

    draw_memory(region);
    return 0;
}