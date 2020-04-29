#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//struktura pre prvok v hashovacej tabulke
typedef struct {
    int key; //kluc prvku
    char *data; //data prvku
} ELEMENT;

//struktura pre hashovaciu tabulku
typedef struct{
    ELEMENT *table; //samotna hashovacia tabulka
    int size, number_of_elements;
    double fullness; //udaje o tabulke - velkost tabulky, naplnenost tabulky
} HASHMAP;

#define MAX_TABLE_SIZE INT_MAX  //maximalna velkost hashovacej tabulky
//vybrala som nahodne a o trochu vacsie prvocislo, ktore je vacsie ako 100 000

int collision_count = 1; //pocitadlo pre kolizie - dany udaj je sucastou rovnice pre vypocet indexu prvku

//funkcia, ktora zisti najblizsie dalsie prvocislo k vlozenemu cislu (vyuzivana pre velkosti tabulky)
int get_next_prime(int size) {
    int is_prime = 1;

    if (size > MAX_TABLE_SIZE) {
        return MAX_TABLE_SIZE;
    }

    while (size <= MAX_TABLE_SIZE) {
        for(int i = 2; i <= size/2; ++i) {
            if(size % i == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            return size;
        }

        ++size;
        is_prime = 1;
    }

    return 0;
}

//funkcia, ktora zisti najblizsie predosle prvocislo k vlozenemu cislu (vyuzivana v druhej hashovacej funckii)
int get_prev_prime(int size) {
    int is_prime = 1;

    size -= 2;

    while (size > 1) {
        for(int i = 2; i <= size/2; ++i) {
            if(size % i == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            return size;
        }

        size -= 2;
        is_prime = 1;
    }

    return 0;
}

//inicializacia hashovacej tabulky
HASHMAP *MY_init(int size) {
    size = get_next_prime(size); //ak bolo vlozene aj nie prvocislo, velkost bude prvocislo
    HASHMAP *hash_table = malloc(sizeof(HASHMAP));
    hash_table->size = size;
    hash_table->number_of_elements = 0;
    hash_table->fullness = 0.9;
    hash_table->table = malloc(size * sizeof(ELEMENT));
    for (int i = 0; i < size; ++i) {
        hash_table->table[i].key = 0;
        hash_table->table[i].data = NULL;
    }
    return  hash_table;
}

//prva hashovacia funkcia - (kluc prvku) % (velkost tabulky)
int hash_fun1(int key, int size) {
    return (key % size);
}

//druha hashovacia funkcia
//premenna prev_prime - prvocislo, ktore je najblizsie mensie k velkosti tabulky
int hash_fun2(int index1, int count, int key, int prev_prime, int size){
    return ((index1 + ((count % 2003) * ((prev_prime - (key % prev_prime))))) % size);
}

//funkcia, ktora zmeni velkost hashovacej tabulky a preusporiada vsetky hodnoty
HASHMAP *MY_resize(HASHMAP *hash_table) {
    collision_count = 1; //resetne danu premennu, kedze sa ide hashovat od zaciatku
    //nova velkost bude o 10 nasobok vacsia ako predosla
    int new_size = get_next_prime((hash_table->size * 10));
    int prev_prime = get_prev_prime(new_size);
    int index1;
    int index2;
    int count = collision_count;
    ELEMENT *old_table = hash_table->table;
    ELEMENT *new_table = malloc(new_size * sizeof(ELEMENT));

    for (int j = 0; j < new_size; ++j) { //inicializcia novej tabulky
        new_table[j].key = 0;
        new_table[j].data = NULL;
    }

    for (int i = 0; i < hash_table->size; ++i) {
        if (old_table[i].key != 0) { //ak sa na danom mieste nachadza prvok...
            index1 = hash_fun1(old_table[i].key, new_size); //dostane novy index z prvej hashovacej funkcie
            index2 = index1;

            while (1) {
                if (new_table[index2].key == 0) { //ak sa na novom indexe nic nenachadza, ulozi data tam
                    new_table[index2].data = old_table[i].data;
                    new_table[index2].key = old_table[i].key;
                    break;
                } else {
                    //ale ak je dany index uz obsadeny, novy index dostaneme scitanim vysledku z prvej hashovacej
                    //funkcie a vysledku z druhej hashovacej funkcie vynasobenim pocitadlom kolizii
                    //(pocitadlo kolizii je preto modulovane %2000, lebo pri vkladani 100 000 prvkov uz hodnota
                    // pocitadla bola moc vysoka na typ int)
                    index2 = hash_fun2(index1, count, old_table[i].key, prev_prime, new_size);
                    count++;
                }
            }
        }
    }

    hash_table->table = new_table;
    free(old_table);
    hash_table->size = new_size;
    collision_count = count;

    return hash_table;
}

//funkcia na vkladanie prvku do hashovacej tabulky
HASHMAP *MY_insert(HASHMAP *hash_table, char *data, int key) {
    if ((double)((double)hash_table->number_of_elements / (double)hash_table->size) > hash_table->fullness) { //ak je tabulka plna, treba ju zvacsit
        hash_table = MY_resize(hash_table);
    }

    int count = collision_count, prev_prime = get_prev_prime(hash_table->size);

    int index1 = hash_fun1(key, hash_table->size); //vypocet prveho indexu z prvej hashovacej funkcie
    int index2 = index1;
    ELEMENT *table = hash_table->table;

    while (1) {
        if (table[index2].key == 0) { //ak sa na danom indexe nic nenachadza, ulozi data tam
            table[index2].data = data;
            table[index2].key = key;
            hash_table->number_of_elements += 1;
            collision_count = count;
            return hash_table;

        } else {
            //ale ak je dany index uz obsadeny, novy index dostaneme scitanim vysledku z prvej hashovacej
            //funkcie a vysledku z druhej hashovacej funkcie vynasobenim pocitadlom kolizii
            //(pocitadlo kolizii je preto modulovane %2000, lebo pri vkladani 100 000 prvkov uz hodnota
            // pocitadla bola moc vysoka na typ int)
            index2 = hash_fun2(index1, count, key, prev_prime, hash_table->size);
            count++;
        }
    }
}

//funckia, ktora vyhladava urcity prvok v tabulke
void MY_search(HASHMAP *hash_table, char *data, int wanted_key) {
    int index1 = hash_fun1(wanted_key, hash_table->size); //vypocita si index prvku
    int index2;
    int prev_prime = get_prev_prime(hash_table->size);

    if (hash_table->table[index1].key == wanted_key) {
        //ak na danom indexe sa nachadza rovnaky prvok, vypise jeho index a kolko iteracii prebehlo k jeho najdeniu
        printf("MY_HASH Data '%s' is on index %d\n", data, index1);
        printf("O(1)\n");
        return;
    } else {
        //ak prvok nie je na danom indexe, tak musi byt ulozeny na nejakom indexe, ktory bol vypocitany druhou
        //hashovacou funkciou
        for (int i = 1; i <= collision_count; i++) {
            index2 = hash_fun2(index1, i, wanted_key, prev_prime, hash_table->size);
            if (hash_table->table[index2].key == wanted_key) {
                //a kedze nevieme, pri akom pocte kolizii sa prvok ulozil do tabulky, musime hladat index pre
                //cely pocet kolizii
                printf("MY_HASH Data '%s' is on index %d\n", data, index2);
                printf("O(%d)\n", i + 1);
                return;
            }
        }
        printf("Find nothing");
    }
}

//funkcia, ktora vypisuje data hahsovacej tabulky
void MY_print(HASHMAP *hash) {
    for (int i = 0; i < hash->size; i++) {
        printf("%d. %s\n", i, (char*)hash->table[i].data);
    }
}

//funkcia, ktora uvolnuje pamat, kde je ulozena hashovacia tabulka
void MY_delete_hashmap(HASHMAP *hash) {
    free(hash->table);
    free(hash);
}