#include <stdio.h>
#include <string.h>
#include <stdint.h>
//---------------------------------------------------------------------//
//Poznamka ku komentarom:                                              //
//                                                                     //
//Komentar ku kodu sa nachadza bud v tom istom riadku daneho           //
//kodu alebo pod kodom                                                 //
//                                                                     //
//---------------------------------------------------------------------//
#define SIZE_INIT 100000 //velkost inicializovanej pamate
#define INT_OFFSET sizeof(int) //velkost int typu (4)
#define CHAR_OFFSET sizeof(char) //velkost char typu (1)

typedef struct BLOCK_HEAD { //struktura ("hlavicka") pre explicitny zoznam blokov volnej pamate
    int mchunk_size;  //velkost volneho bloku, ktoru moze pouzivatel vyuzit pri alokacii
    int next_block_offset; //offset k dalsiemu volnemu bloku v pamati
    //t.j. pocet bajtov od danej hlavicky k dalsej hlavicke volneho bloku
} BLOCK_HEAD;

typedef struct ALLOC_HEAD { //struktura ("hlavicka") pre alokovanu pamat
    int alloc_size; //udavana v minusovej hodnote, kvoli oznaceniu, ze dany blok pamate je zabraty (alokovany)
    //absolutna hodnota alloc_size predstavuje velkost alokovanej pamate
} ALLOC_HEAD;

typedef struct MEM_HEAD { //struktura ("hlavicka") pre celkovu inicializovanu pamat
    int first_free_offset; //offset k prvemu volnemu bloku v pamati
    //t.j. pocet bajtov od danej hlavicky k dalsej hlavicke volneho bloku
} MEM_HEAD;

static MEM_HEAD * memory_head = NULL; //ukazovatel na inicializovanu pamat

//---------------------------------------------------------------------//
//                                                                     //
//       SECONDARY FUNCTIONS                                           //
//                                                                     //
//---------------------------------------------------------------------//

void * allocated_memory_as_NULL(void * alloc_mem) { //funkcia, ktora vrati NULL hodnotu pre alokovanu pamat
    //atribut funkcie: ukazovatel na alokovany blok pamate
    alloc_mem = NULL;
    //printf("Error:\tnot enough memory\n");
    return alloc_mem;
}

int * get_on_memory_address(char * memory){ //funkcia, ktora vrati ukazovatel typu int
    //atribut funkcie: ukazovatel typu char
    return (int *)memory;
}

int get_int_value_on(char * memory){
    //funkcia, ktora vrati int hodnotu zapisanu na adrese, kde odkazuje ukazovatel typu char
    //atribut funkcie: ukazovatel typu char
    return * get_on_memory_address(memory);
}

void draw_memory(char * region) { //funkcia pre vizualne testovanie funkcii memory_alloc a memory_free
    for (int i = 0; i < SIZE_INIT; i++) {
        printf("%d %p\n", region[i], &region[i]);
    }
}

//---------------------------------------------------------------------//
//                                                                     //
//       MAIN 4 FUNCTIONS                                              //
//                                                                     //
//---------------------------------------------------------------------//

void * memory_alloc(unsigned int size) { //funckia na alokaciu pamate
    //atribut funkcie: velkost bloku pamate, ktoru chce pouzivatel alokovat
    //printf("\nMemory of %d B starting allocating---------------\n", size);
    BLOCK_HEAD * previous_free = NULL, * current_free = NULL; //pomocne ukazovatele na volne bloky pamate
    ALLOC_HEAD * alloc_head = NULL; //hlavicka pre alokovanu pamat
    void * allocated_memory = NULL; //ukazovatel na blok pamate, ktory bude alokovany pre pouzivatela
    char * next_free = NULL; //pomocny ukazovatel na nadchadzajuci volny blok
    int curr_mchunk_size = get_int_value_on(((char *)memory_head + memory_head->first_free_offset)); //velkost prveho volneho bloku v pamati

    if ((char)curr_mchunk_size != 0){ //ak ukazovatel na prvy volny blok nesmeruje na koniec pamate...
        //vieme, ze este stale je volna pamat
        current_free = (BLOCK_HEAD *)((char *)memory_head + memory_head->first_free_offset); //ukazovatel na prvy volny blok
        //printf("next_block_val_offset: (int)%d\n", current_free->next_block_offset);

        while(((current_free->mchunk_size) < (size)) &&
              ((char)get_int_value_on(((char *)current_free + current_free->next_block_offset)) != 0)) { //hladanie vhodneho volneho bloku pre alokaciu - FIRST FIT
            previous_free = current_free; //ulozenie terajsieho volneho bloku ako predosli
            current_free = (BLOCK_HEAD *)((char *)previous_free + previous_free->next_block_offset); //posunutie sa na dalsi volny blok
        }

        if (((current_free->mchunk_size) < (size)) &&
            ((char) get_int_value_on(((char *) current_free + current_free->next_block_offset)) == 0)) {
            //ak sa nenasiel ziaden volny blok s vyhovujucou velkostou a nachadzame sa na konci, vrati NULL
            return allocated_memory_as_NULL(allocated_memory);
        }

    } else { //ak ukazovatel na prvy volny blok smeruje na koniec pamate...
        return allocated_memory_as_NULL(allocated_memory); //...vrati NULL
    }

    alloc_head = (ALLOC_HEAD *)current_free; //urcenie miesta hlavicky alokovanej pamate
    int alloc_size = (int)size; //pretypovanie zadanej velkosti
    int remain_size = current_free->mchunk_size - alloc_size; //zostatok volnej pamate po alokacii (aj pre hlavicku volneho bloku)

    if (remain_size >= sizeof(BLOCK_HEAD)) { //ak zostatok volnej pamate je vacsi ako velkost hlavicky volneho bloku...
        next_free = (char *)((char *)current_free + current_free->next_block_offset); //zisti adresu nadchadzajuceho volneho bloku
        current_free = (BLOCK_HEAD *)((char *)current_free + alloc_size + sizeof(ALLOC_HEAD));
        //adresu volneho bloku presunie o alokovanu pamat+velkost hlavicky alokovanej pamate
        current_free->mchunk_size = remain_size - (int)INT_OFFSET;
        //nova velkost volnej pamate je zostatok-velkost hlavicky volnej pamate (bez udaju o offsete)
        current_free->next_block_offset = (char *)next_free - (char *)current_free;
        //novy offset k dalsiemu bloku volnej pamate -> addresa dalsieho bloku-adresa aktualneho
        //printf("after malloc: %d %d %p\n",current_free->mchunk_size, current_free->next_block_offset, current_free);

        alloc_head->alloc_size = (-1) * alloc_size; //oznacenie pamate, ze je alokovana (zaporna hodnota alokovanej velkosti)
        allocated_memory = (char *)((char *)alloc_head + sizeof(ALLOC_HEAD)); //nasmerovanie na adresu alokovanej pamate

        if (previous_free == NULL) {
            memory_head->first_free_offset = (char*)current_free - (char*)memory_head;
        } else {
            previous_free->next_block_offset = (char*)current_free - (char*)previous_free;
        }

        return allocated_memory;

    } else if ((remain_size < sizeof(BLOCK_HEAD)) && (remain_size >= 0)){ //ak zostatok volnej pamate je mensi ako velkost hlavicky volneho bloku...
        //...tak prideli celu velkost volneho bloku pre alokovanu pamat
        next_free = (char *)((char *)current_free + current_free->next_block_offset); //zisti adresu nadchadzajuceho volneho bloku
        alloc_head->alloc_size = (-1) * current_free->mchunk_size; //oznacenie pamate, ze je alokovana (zaporna hodnota alokovanej velkosti)
        allocated_memory = (char *)((char *)alloc_head + sizeof(ALLOC_HEAD)); //nasmerovanie na adresu alokovanej pamate

        if (previous_free == NULL) {
            memory_head->first_free_offset = (char *)next_free - (char *)memory_head;
        } else {
            previous_free->next_block_offset = (char *)next_free - (char *)previous_free;
        }
        //printf("After allocation, there not remains extra space in chunk.\n"
        //"Allocated memory consumed whole block. (%d)\n", (-1)*alloc_head->alloc_size);
        //printf("memory_head: %d %p\n", memory_head->first_free_offset, memory_head);
        return allocated_memory;

    } else {
        return allocated_memory_as_NULL(allocated_memory);
    }
}

int memory_check(void * ptr){ /*funkcia zisti, ci ukazovatel v atribute funckie je platny, tzn. ci bol v nejakom z predchadzajucich volani vrateni
                                funkciou memory_alloc a zatial nebol uvolneny funkciou memory_free*/
    int size_of_block = get_int_value_on((char *)ptr - sizeof(ALLOC_HEAD)); //zisti velkost v hlavicke bloku

    if (ptr == NULL) { //ak smeruje ukazovatel na NULL (nikam), nie je platny
        return 0;
    } else if ((ptr < (void *)memory_head) && ((char)get_int_value_on((char *)ptr - size_of_block))) {
        //ak ukazovatel smeruje na pamat mimo inicializovanej pamate alebo na 0 (ktora oznacuje koniec pamate), nie je platny
        return 0;
    } else if (size_of_block > 0) {
        //ak v hlavicke ma kladne cislo ulozene, tzn. ze je blok uvolneny -> nie je platny
        return 0;
    } else { //ukazovatel je platny
        return 1;
    }
}

int memory_free(void * valid_ptr) { //funkcia uvolnenia pamate
//    printf("\nFREE %p size %d\n", valid_ptr - INT_OFFSET, get_int_value_on((char*) valid_ptr - INT_OFFSET));
    if (memory_check(valid_ptr)) { //kontrola, ci je ukazovatel platny alebo nie
        BLOCK_HEAD * prev_free, * alloc_mem, * next, * free_block; //pomocne ukazovatele na bloky pamate
        int alloc_mem_size = get_int_value_on((char *)valid_ptr - sizeof(ALLOC_HEAD)); //ukladanie hodnoty z hlavicky alokovanej pamate
        int next_block_offset_val, mchunk_size; //pomocne premenne na udaje z hlaviciek
        alloc_mem_size *= -1; //kladna velkost alokovanej pamate

        alloc_mem = (BLOCK_HEAD *)((char *)valid_ptr - sizeof(ALLOC_HEAD)); //ukazovatel na alokovanu pamate aj s hlavickou
        free_block = (BLOCK_HEAD *)((char *)memory_head + memory_head->first_free_offset); //ukazovatel na prvy volny blok
//        printf("fisrt free block mchunk size: %d\n", get_int_value_on((char*)memory_head + memory_head->first_free_offset));

        next = (BLOCK_HEAD *)((char *)alloc_mem + sizeof(ALLOC_HEAD) + alloc_mem_size); //ukazovatel na blok za alokovanou pamatou

        if (&free_block->mchunk_size > &alloc_mem->mchunk_size) { //ak pred alokovanou pamatou nie je ziaden volny blok
            if (get_int_value_on((char *)next) > 0) { //ak je nasledujuci blok volny ->
                //->spojenie uvolneneho bloku pamate s nasledujucim blokom volnej pamate
                memory_head->first_free_offset = (char*)alloc_mem - (char*)memory_head;
                //nastavenie offsetu v hlavicke celkovej pamati na prvy volny blok
                alloc_mem->mchunk_size = next->mchunk_size + ((char *)next - (char *)alloc_mem);
                alloc_mem->next_block_offset = next->next_block_offset + ((char *)next - (char *)alloc_mem);
//                printf("uvolnena pamet: >size %d >offset %d\n", alloc_mem->mchunk_size, alloc_mem->next_block_offset);
//                printf("after free > memory_head offset: %d\n", memory_head->first_free_offset);

            } else { //iba uvolni blok alokovanej pamate a nastavi na neho offset, kedze je to celkovo prvy blok volnej pamate
                alloc_mem->mchunk_size = alloc_mem_size;
                memory_head->first_free_offset = (char*)alloc_mem - (char*)memory_head;
                alloc_mem->next_block_offset = (char*)free_block - (char*)alloc_mem;
//                printf("uvolnena pamet: >size %d >offset %d\n", alloc_mem->mchunk_size, alloc_mem->next_block_offset);
//                printf("after free > memory_head offset: %d\n", memory_head->first_free_offset);
            }

            return 0;

        } else { //ak pred alokovanou pamatou je volny blok
            prev_free = free_block;

            while ((BLOCK_HEAD *)((char *)free_block + free_block->mchunk_size + INT_OFFSET) != (BLOCK_HEAD *)alloc_mem) {
                //ak po volnom bloku nenasleduje alokovany blok pamati...
                prev_free = free_block; //ulozenie terajsieho volneho bloku ako predosleho
                free_block = (BLOCK_HEAD *)((char *)prev_free + prev_free->next_block_offset); //posun na dalsi volny blok

                if (&free_block->mchunk_size > &alloc_mem->mchunk_size) {
                    //ak dalsi volny blok je v pamati dalej, ako alokovany blok, ukonci cyklus
                    free_block = NULL;
                    break;
                }
            }

            if (free_block != NULL) {
                prev_free = free_block;
            }

            if (get_int_value_on((char *)next) > 0) { //ak je nasledujuci blok volny ->
                //->spojenie uvolneneho bloku pamate s nasledujucim blokom volnej pamate

                alloc_mem->mchunk_size = next->mchunk_size + (int)INT_OFFSET + ((char *)next - (char *)alloc_mem - (int)INT_OFFSET);
                alloc_mem->next_block_offset = next->next_block_offset + ((char *)next - (char *)alloc_mem);

                if ((BLOCK_HEAD *)((char *)prev_free + (int)INT_OFFSET + prev_free->mchunk_size) == (BLOCK_HEAD*)alloc_mem) {
                    //ak predchadzajuci volny blok pamate je priamo pred alokovanim blokom, spoja sa
                    prev_free->mchunk_size = prev_free->mchunk_size + (int)INT_OFFSET + alloc_mem->mchunk_size;
                    prev_free->next_block_offset = alloc_mem->next_block_offset + ((char *)alloc_mem - (char *)prev_free) ;
                } else {
                    prev_free->next_block_offset = (char *)alloc_mem - (char *)prev_free;
                }
//                printf("uvolnena pamet: >size %d >offset %d\n", alloc_mem->mchunk_size, alloc_mem->next_block_offset);
//                printf("after free > memory_head offset: %d\n", memory_head->first_free_offset);

                return 0;

            } else { //iba uvolni blok alokovanej pamate a nastavi na neho offset, kedze je to celkovo prvy blok volnej pamate
                alloc_mem->mchunk_size = alloc_mem_size;
                alloc_mem->next_block_offset = (char *)((char *)prev_free + prev_free->next_block_offset) - (char *)alloc_mem;

                if (free_block != NULL) {
                    free_block->mchunk_size = free_block->mchunk_size + alloc_mem_size + (int)INT_OFFSET;
                } else {
                    prev_free->next_block_offset = (char*)alloc_mem - (char*)prev_free;
                }
//                printf("uvolnena pamet: >size %d >offset %d\n", alloc_mem->mchunk_size, alloc_mem->next_block_offset);
//                printf("after free > memory_head offset: %d\n", memory_head->first_free_offset);

                return 0;

            }
        }
    } else {
        return 1;
    }
}

void memory_init(void * ptr, unsigned int size) { //funckia na inicializaciu volnej pamate
    //atribut funkcie: ukazovatel na volny blok pamate, velkost inicializovanej pamate
    memory_head = (MEM_HEAD *)ptr; //ulozenie ukazovatela na volny blok pamate do globalneho ukazovatela
    BLOCK_HEAD * free_block = (BLOCK_HEAD *)((char *)ptr + (int)INT_OFFSET); //ukazovatel na hlavicku prveho volneho bloku
    memory_head->first_free_offset = (char *)free_block - (char *)memory_head;
    //offset od hlavicky inicializovanej pamate ku hlavicke prveho volneho bloku pamate
    free_block->mchunk_size = (int)size - (int)sizeof(BLOCK_HEAD) - (int)CHAR_OFFSET;
    //celkova velkost inicializovanej pamate - velkost hlavicky volneho bloku - velkost "paticky"(konca) pamate
    free_block->next_block_offset = (int)(size - CHAR_OFFSET - INT_OFFSET); //offset ku koncu inicializovanej pamate
    memset((char *)free_block + free_block->next_block_offset, 0, 1); //nastavenie "paticky" na hodnotu 0
}

//---------------------------------------------------------------------//
//                                                                     //
//       MAIN                                                          //
//                                                                     //
//---------------------------------------------------------------------//

int main() {
    char region[SIZE_INIT]; //blok pamate, na ktorom bude realizovana alokacia
    memory_init(region, SIZE_INIT); //inicializacia volneho bloku pamate
    return 0;
}