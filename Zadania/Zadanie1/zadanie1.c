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

#define SHORT_OFF sizeof(short)
#define INT_OFF sizeof(int)
#define CHAR_OFF sizeof(char)

typedef struct BLOCK_HEAD { //struktura ("hlavicka") pre explicitny zoznam blokov volnej pamate
    int block_size;  //velkost volneho bloku, aj s velkostou hlavicky
    int next_block_off; //offset k dalsiemu volnemu bloku v pamati
    //t.j. pocet bajtov od danej hlavicky k dalsej hlavicke volneho bloku
} BLOCK_HEAD;
#define BLOCK_HEAD_OFF sizeof(BLOCK_HEAD) //8

typedef struct ALLOC_HEAD { //struktura ("hlavicka") pre alokovanu pamat
    int alloc_size; //udavana v minusovej hodnote, kvoli oznaceniu, ze dany blok pamate je zabraty (alokovany)
    //absolutna hodnota alloc_size predstavuje velkost alokovanej pamate
} ALLOC_HEAD;
#define ALLOC_HEAD_OFF sizeof(ALLOC_HEAD) //4

typedef struct MEM_HEAD { //struktura ("hlavicka") pre celkovu inicializovanu pamat
    int first_free_offset; //offset k prvemu volnemu bloku v pamati (pocet bajtov od danej hlavicky k dalsej hlavicke volneho bloku)
    int end_off;
} MEM_HEAD;
#define MEM_HEAD_OFF sizeof(MEM_HEAD) //8

static MEM_HEAD * memory_head = NULL; //ukazovatel na inicializovanu pamat

//---------------------------------------------------------------------//
//                                                                     //
//       SECONDARY FUNCTIONS                                           //
//                                                                     //
//---------------------------------------------------------------------//

void * allocated_memory_as_NULL(void * alloc_mem) { //funkcia, ktora vrati NULL hodnotu pre alokovanu pamat
    //atribut funkcie: ukazovatel na alokovany blok pamate
    alloc_mem = NULL;
    return alloc_mem;
}

int * get_on_memory_address(char * memory){ //funkcia, ktora vrati ukazovatel typu int
    return (int *)memory;
}

int get_int_value_on(char * memory){ //funkcia, ktora vrati int hodnotu zapisanu na adrese, kde odkazuje ukazovatel typu char
    return * get_on_memory_address(memory);
}

//---------------------------------------------------------------------//
//                                                                     //
//       MAIN 4 FUNCTIONS                                              //
//                                                                     //
//---------------------------------------------------------------------//

void * memory_alloc(unsigned int size) { //funckia na alokaciu pamate
    //atribut funkcie: velkost bloku pamate, ktoru chce pouzivatel alokovat
    BLOCK_HEAD * previous_free = NULL, * current_free = NULL, 
    * best_free = NULL, * next_free = NULL; //pomocne ukazovatele na volne bloky pamate
    ALLOC_HEAD * alloc_head = NULL; //hlavicka pre alokovanu pamat
    void * allocated_memory = NULL; //ukazovatel na blok pamate, ktory bude alokovany pre pouzivatela
    char * end_addr = (char *)((char *)memory_head + memory_head->end_off); //ukazovatel na koniec inicializovanej pamate
    int curr_block_size = get_int_value_on(((char *)memory_head + memory_head->first_free_offset)); //velkost prveho volneho bloku v pamati
    current_free = (BLOCK_HEAD *)((char *)memory_head + memory_head->first_free_offset); //ukazovatel na prvy volny blok

    if (((char *)current_free < (char *)end_addr) && (curr_block_size > 0)) { //ak ukazovatel na prvy volny blok nesmeruje na koniec pamate a vieme, ze este stale je volna pamat
        //best fit
        while((char *)current_free != end_addr) { //ak nesmeruje na koniec pamate
            if ((current_free->block_size) < (size + (int)INT_OFF)) { //ak volny blok nie je dostatocne velky
                previous_free = current_free; //ulozenie terajsieho volneho bloku ako predosli
                current_free = (BLOCK_HEAD *)((char *)previous_free + previous_free->next_block_off); //posunutie sa na dalsi volny blok
                continue;
            }

            //ak je volny blok dostaocne velky
            if (best_free == NULL) { 
                best_free = current_free;
            } else {
                if (best_free->block_size > current_free->block_size) {
                    best_free = current_free; //ak uz je najdeny vhodny blok, ale najde sa novy, dostatocne velky a mensi, ako terajsi vhodny
                }
            }

            previous_free = current_free; //posunie sa dalej
            current_free = (BLOCK_HEAD *)((char *)previous_free + previous_free->next_block_off);
        }

    } else { //ak ukazovatel na prvy volny blok smeruje na koniec pamate...
        return allocated_memory_as_NULL(allocated_memory); //...vrati NULL
    }

    if (best_free == NULL) { //ak nenaslo vhodny volny blok
        return allocated_memory_as_NULL(allocated_memory); //...vrati NULL
    }

    alloc_head = (ALLOC_HEAD *)best_free; //urcenie miesta hlavicky alokovanej pamate
    int alloc_size = (int)size; //pretypovanie zadanej velkosti
    int remain_size = best_free->block_size - (int)ALLOC_HEAD_OFF - alloc_size; //zostatok volnej pamate po alokacii

    if (remain_size >= (int)BLOCK_HEAD_OFF) { //ak zostatok volnej pamate je vacsi ako velkost hlavicky volneho bloku...
        next_free = (BLOCK_HEAD *)((char *)best_free + best_free->next_block_off); //zisti adresu nadchadzajuceho volneho bloku
        current_free = (BLOCK_HEAD *)((char *)best_free + (int)ALLOC_HEAD_OFF + alloc_size);
        current_free->block_size = remain_size; //adresu volneho bloku presunie o alokovanu pamat+velkost hlavicky alokovanej pamate
        current_free->next_block_off = (char *)next_free - (char *)current_free; //nova velkost volnej pamate je zostatok-velkost hlavicky volnej pamate (bez udaju o offsete)
        //novy offset k dalsiemu bloku volnej pamate -> addresa dalsieho bloku-adresa aktualneho

        alloc_head->alloc_size = (-1) * alloc_size; //oznacenie pamate, ze je alokovana (zaporna hodnota alokovanej velkosti)
        allocated_memory = (char *)((char *)alloc_head + (int)ALLOC_HEAD_OFF); //nasmerovanie na adresu alokovanej pamate

        if ((char *)((char *)memory_head + memory_head->first_free_offset) == (char *)alloc_head) { //ak sa obsadil prvy volny blok...
            memory_head->first_free_offset = (char*)current_free - (char*)memory_head; //ulozi sa offset na dalsi vytvoreny volny blok
        } else {
            next_free = (BLOCK_HEAD *)((char *)memory_head + memory_head->first_free_offset); //prvy volny blok

            while((char *)next_free != end_addr) { //pokial nedojde na koniec inicializovanej pamate
                if ((char *)next_free == (char *)alloc_head) { //ak dalsi blok je novy alokovany blok
                    previous_free->next_block_off = previous_free->next_block_off + (int)ALLOC_HEAD_OFF + alloc_size; 
                    //nastavi sa offset na dalsi volny blok z predosleho bloku
                    break;
                } else {
                    previous_free = next_free; //posunie sa dalej
                    next_free = (BLOCK_HEAD *)((char *)previous_free + previous_free->next_block_off);
                }
                
            }
        }

        return allocated_memory; //alokovana pamat vratena uzivatelovi

    } else if ((remain_size < (int)BLOCK_HEAD_OFF) && (remain_size >= 0)){ //ak zostatok volnej pamate je mensi ako velkost hlavicky volneho bloku...
        //...tak prideli celu velkost volneho bloku pre alokovanu pamat
        next_free = (BLOCK_HEAD *)((char *)best_free + best_free->next_block_off); //zisti adresu nadchadzajuceho volneho bloku
        alloc_head->alloc_size = (-1) * previous_free->block_size; //oznacenie pamate, ze je alokovana (zaporna hodnota alokovanej velkosti)
        allocated_memory = (char *)((char *)alloc_head + sizeof(ALLOC_HEAD)); //nasmerovanie na adresu alokovanej pamate

        if ((char *)((char *)memory_head + memory_head->first_free_offset) == (char *)alloc_head) { //ak sa obsadil prvy volny blok...
            memory_head->first_free_offset = (char*)next_free - (char*)memory_head; //ulozi sa offset na dalsi vytvoreny volny blok
        } else {
            current_free = (BLOCK_HEAD *)((char *)memory_head + memory_head->first_free_offset); //prvy volny blok

            while((char *)current_free != end_addr) { //pokial nedojde na koniec inicializovanej pamate
                if ((char *)current_free == (char *)alloc_head) { //ak dalsi blok je novy alokovany blok
                    previous_free->next_block_off = previous_free->next_block_off + (int)ALLOC_HEAD_OFF + ((-1) * alloc_head->alloc_size);
                    //nastavi sa offset na dalsi volny blok z predosleho bloku
                    break;
                } else {
                    previous_free = current_free; //posunie sa dalej
                    current_free = (BLOCK_HEAD *)((char *)previous_free + previous_free->next_block_off);
                }
                
            }
        }

        return allocated_memory; //alokovana pamat vratena uzivatelovi

    } else { //ak nenaslo vhodny volny blok
        return allocated_memory_as_NULL(allocated_memory);
    }
}

int memory_check(void * ptr){ //funkcia zisti, ci ukazovatel v atribute funckie je platny
    if (ptr == NULL) { //ak smeruje ukazovatel na NULL (nikam), nie je platny
        return 0;
    } else if ((ptr < (void *)memory_head) && (ptr > (void *)((char *)memory_head + memory_head->end_off))) {
        //ak ukazovatel smeruje na pamat mimo inicializovanej pamate nie je platny
        return 0;
    } else if (get_int_value_on((char *)ptr - (int)ALLOC_HEAD_OFF) > 0) {
        //ak v hlavicke ma kladne cislo ulozene, tzn. ze je blok uvolneny -> nie je platny
        return 0;
    } else { //ukazovatel je platny
        return 1;
    }
}

int memory_free(void * valid_ptr) { //funkcia uvolnenia pamate
    if (memory_check(valid_ptr)) { //kontrola, ci je ukazovatel platny alebo nie
        BLOCK_HEAD * prev_free = NULL, * alloc_mem = NULL, * next = NULL, * free_block = NULL; //pomocne ukazovatele na bloky pamate
        alloc_mem = (BLOCK_HEAD *)((char *)valid_ptr - (int)ALLOC_HEAD_OFF); //ukazovatel na alokovanu pamate aj s hlavickou
        int alloc_mem_size = get_int_value_on((char *)alloc_mem); //ukladanie hodnoty z hlavicky alokovanej pamate
        alloc_mem_size *= -1; //kladna velkost alokovanej pamate

        free_block = (BLOCK_HEAD *)((char *)memory_head + memory_head->first_free_offset); //ukazovatel na prvy volny blok
        next = (BLOCK_HEAD *)((char *)alloc_mem + (int)ALLOC_HEAD_OFF + alloc_mem_size); //ukazovatel na blok za alokovanou pamatou

        if (&free_block->block_size > &alloc_mem->block_size) { //ak pred alokovanou pamatou nie je ziaden volny blok
            if (get_int_value_on((char *)next) > 0) { //ak je nasledujuci blok volny ->
                //->spojenie uvolneneho bloku pamate s nasledujucim blokom volnej pamate
                memory_head->first_free_offset = (char*)alloc_mem - (char*)memory_head;
                //nastavenie offsetu v hlavicke celkovej pamati na prvy volny blok
                alloc_mem->block_size = next->block_size + ((char *)next - (char *)alloc_mem);
                alloc_mem->next_block_off = next->next_block_off + ((char *)next - (char *)alloc_mem);

            } else { //iba uvolni blok alokovanej pamate a nastavi na neho offset, kedze je to celkovo prvy blok volnej pamate
                alloc_mem->block_size = alloc_mem_size + (int)ALLOC_HEAD_OFF;
                memory_head->first_free_offset = (char*)alloc_mem - (char*)memory_head;
                alloc_mem->next_block_off = (char*)free_block - (char*)alloc_mem;
            }

            return 0;

        } else { //ak pred alokovanou pamatou je volny blok
            prev_free = free_block;

            while ((BLOCK_HEAD *)((char *)free_block + free_block->block_size) != (BLOCK_HEAD *)alloc_mem) {
                //ak po volnom bloku nenasleduje alokovany blok pamati...
                prev_free = free_block; //ulozenie terajsieho volneho bloku ako predosleho
                free_block = (BLOCK_HEAD *)((char *)prev_free + prev_free->next_block_off); //posun na dalsi volny blok

                if (&free_block->block_size > &alloc_mem->block_size) {
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

                alloc_mem->block_size = next->block_size + ((char *)next - (char *)alloc_mem);
                alloc_mem->next_block_off = next->next_block_off + ((char *)next - (char *)alloc_mem);

                if ((BLOCK_HEAD *)((char *)prev_free + prev_free->block_size) == (BLOCK_HEAD*)alloc_mem) {
                    //ak predchadzajuci volny blok pamate je priamo pred alokovanim blokom, spoja sa
                    prev_free->block_size = prev_free->block_size + alloc_mem->block_size;
                    prev_free->next_block_off = alloc_mem->next_block_off + ((char *)alloc_mem - (char *)prev_free) ;
                } else {
                    prev_free->next_block_off = (char *)alloc_mem - (char *)prev_free;
                }

                return 0;

            } else { //iba uvolni blok alokovanej pamate a nastavi na neho offset, kedze je to celkovo prvy blok volnej pamate
                alloc_mem->block_size = alloc_mem_size + (int)ALLOC_HEAD_OFF;
                alloc_mem->next_block_off = (char *)((char *)prev_free + prev_free->next_block_off) - (char *)alloc_mem;

                if (free_block != NULL) {
                    free_block->block_size = free_block->block_size + alloc_mem->block_size;
                } else {
                    prev_free->next_block_off = (char*)alloc_mem - (char*)prev_free;
                }

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
    BLOCK_HEAD * free_block = (BLOCK_HEAD *)((char *)ptr + (int)MEM_HEAD_OFF); //ukazovatel na hlavicku prveho volneho bloku
    memory_head->first_free_offset = (char *)free_block - (char *)memory_head;
    memory_head->end_off = size;
    //offset od hlavicky inicializovanej pamate ku hlavicke prveho volneho bloku pamate
    free_block->block_size = (int)size - (int)MEM_HEAD_OFF;
    //celkova velkost inicializovanej pamate - velkost hlavicky volneho bloku - velkost "paticky"(konca) pamate
    free_block->next_block_off = (int)(free_block->block_size); //offset ku koncu inicializovanej pamate
    memset((char *)free_block + free_block->next_block_off, 0, 1); //nastavenie "paticky" na hodnotu 0
}