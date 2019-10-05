#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define OFFSET sizeof(int)
#define HOFFSET sizeof(int)

char *mem;

//funkcia zmení hlavičku pamätového bloku na volnú
void clear(int * point){
    *point = abs(*point);
}

//funkcia spojí dve hlavičky do jednej
void connect(int * first, int * second){
    *first += *second + OFFSET;
}

//funkcia zistí čí je hlavička volná alebo nie
int isFree(int * point){
    return *point > 0;
}

//funkcia vrát ukazovatel typu int na hlavičku
int *getPointOn(char * memory){
    return (int*)memory;
}

//funkcia vráti hodnotu napísanu v hlavičke
int getValOn(char * memory){
    return *getPointOn(memory);
}

//funkcia zistí či sa pointer nachádza v pamäti s ktorouje možné pracovať
int isInMem(char * point){
    return point >= mem + HOFFSET && point < mem + getValOn(mem) - OFFSET -HOFFSET;
}

//funkcia najde dalšiu hlavičku a ak nieje dalšia hlavička vráti NULL
char *findNextMemBlock(char * memory){
    if(mem - memory > getValOn(mem))
        return NULL;
    if(!isInMem((char*)(memory + OFFSET + (abs(getValOn(memory))))))
        return NULL;
    return (char*)(memory + OFFSET + (abs(getValOn(memory))));
}

//funkcia napíše velkosť a obsadenisť všetkých pamäťových blokov
void showAllBlocks(){
    char * actBlock = mem + HOFFSET;
    do
        printf("%p: %d\n", actBlock, getValOn(actBlock));
    while((actBlock = findNextMemBlock(actBlock)) != NULL && actBlock < mem + getValOn(mem));
}

//funkcia vykreslí všetky bajty v pamäti, ich adresu aj hodnotu
void showAllMem(){
    int i;
    for(i=0 ; i<*mem ; i++)
        printf("%d %p: %d\n",i, getPointOn(mem + i), getValOn(mem + i));
}

//funkcia z vizualizuje pamäť
void drawMemory(){
    char * actBlock = mem + HOFFSET;
    int i;
    printf("MEMO");
    do{
        for(i=0 ; i<OFFSET ; i++)
            printf("H");
        for(i=0 ; i<abs(getValOn(actBlock)) ; i++)
            printf("%c",(getValOn(actBlock) > 0 ? '_' : '*'));

    }
    while((actBlock = findNextMemBlock(actBlock)) != NULL && actBlock < mem + getValOn(mem));
    printf("\n\n");
}

void *memory_alloc(unsigned int size){
    char *actMem = mem + HOFFSET;
    do{
        if((int)((getValOn(actMem) ) - size) >= 0){
            int memSize = getValOn(actMem);

            //ak by sa nezmestiha hlavčka tak sa pridelí celý blok pamäte
            if(memSize <= OFFSET + size)
                size = memSize;

            *(getPointOn(actMem)) = -size;

            //ak ostane v bloku volné miesto tak sa prepíše
            if(memSize - size > 0)
                *(getPointOn(actMem + size + OFFSET )) = memSize - OFFSET - size;

            return actMem + OFFSET;
        }
        actMem = findNextMemBlock(actMem);
    }
    while(actMem != NULL);

    //ak nenašlo volné miesto vráti null;
    return NULL;
}

int memory_free(void *valid_ptr){
    int * p_prev = getPointOn(mem + HOFFSET) ;
    int * p_next = getPointOn(findNextMemBlock((char*)(valid_ptr - OFFSET)));
    int * p_act  = getPointOn((char *)(valid_ptr - OFFSET));

    //vyčistí aktuálny blok pamäťe;
    clear(p_act);

    //ak je volný nasledujúci tak ich spojí;
    if(isInMem((char*)p_next) && isFree(p_next))
        connect(p_act, p_next);

    //nájde predchádzajúci
    while(getPointOn(findNextMemBlock((char )p_prev)) != p_act && isInMem((char)p_prev))
        p_prev = getPointOn(findNextMemBlock((char *)p_prev));


    //ak je volný predchádzajúci tak ich spojí;
    if(p_prev != 0 && getPointOn(mem + HOFFSET) != p_prev && isFree(p_prev))
        connect(p_prev, p_act);

    return 0;
}

int memory_check(void *ptr){
    char * point = mem + HOFFSET;

    //ak je pointer volný vráti 0
    if(getValOn((char *)(ptr - OFFSET)) > 0)
        return 0;

    //ak je pointer prvý block pamäte vráti 1
    if(ptr - OFFSET == point)
        return 1;

    //prechádza blocky a ak nájde tento istý vráti 1
    while(isInMem(point))
        if((point = findNextMemBlock(point)) == ptr - OFFSET)
            return 1;

    //ináč vráti 0
    return 0;
}

void memory_init(void *ptr, unsigned int size){
    //priradím na začiatok velkosť
    *(getPointOn(ptr)) = size;

    //priradím pointer ku globálnej premennej
    mem = ((char *)ptr);

    //vytvorím prví blok pamäte
    *(getPointOn(ptr + HOFFSET)) = size - OFFSET - HOFFSET;
}

int main() {
    char region[50];
    memory_init(region, 50);
    char* pointer = (char*) memory_alloc(10);
    char* pointer2 = (char*) memory_alloc(20);
    if (pointer)
        memset(pointer, 0, 10);
    drawMemory();
    if (pointer2)
        memset(pointer, 0, 10);
    drawMemory();
    if (pointer)
        memory_free(pointer);
    if (pointer2)
        memory_free(pointer2);

    drawMemory();
    return 0;
}