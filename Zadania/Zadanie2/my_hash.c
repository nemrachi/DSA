#include <math.h>

typedef struct {
    int key;
    char *data;
} ELEMENT;

typedef struct{
    ELEMENT *table;
    int size;
    int fullness;
} HASHMAP;

#define MAX_TABLE_SIZE 115211

int collision_count = 1;

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
            printf("\tnew prime %d\n", size);
            return size;
        }

        ++size;
        is_prime = 1;
    }

    return 0;
}

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

HASHMAP *MY_init(int size) {
    size = get_next_prime(size);
    HASHMAP *hash_table = malloc(sizeof(HASHMAP));
    hash_table->size = size;
    hash_table->fullness = 0;
    hash_table->table = malloc(size * sizeof(ELEMENT));
    for (int i = 0; i < size; ++i) {
        hash_table->table[i].key = (int)NULL;
        hash_table->table[i].data = NULL;
    }

    return  hash_table;
}

int hash_fun1(int key, int size) {
    return (key % size);
}

int hash_fun2(int key, int prev_prime){
    return (prev_prime - (key % prev_prime));
}

HASHMAP *MY_resize(HASHMAP *hash_table) {
    printf("\tRESIZE\n");

    collision_count = 1;
    int new_size = get_next_prime((hash_table->size * 10));
    int index1;
    int index2;
    int count = collision_count;
    ELEMENT *old_table = hash_table->table;
    ELEMENT *new_table = malloc(new_size * sizeof(ELEMENT));

    for (int i = 0; i < new_size; ++i) {
        new_table[i].key = (int)NULL;
        new_table[i].data = NULL;
    }

    for (int i = 0; i < hash_table->size; ++i) {
        if (old_table[i].key != (int)NULL) {
            index1 = hash_fun1(old_table[i].key, new_size);
            index2 = index1;

            while (1) {
                if (new_table[index2].key == (int)NULL) {
                    new_table[index2].data = old_table[i].data;
                    new_table[index2].key = old_table[i].key;
                    break;
                } else {
                    index2 = (index1 + ((count % 2000) * hash_fun2(old_table[i].key, hash_table->size))) % new_size;
                    count++;
//                    if (count > 100000) {
//                        hash_table->size = hash_table->size + 1;
//                        hash_table = MY_resize(hash_table);
//                        break;
//                    }
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


HASHMAP *MY_insert(HASHMAP *hash_table, char *data, int key) {
    if (hash_table->size <= hash_table->fullness) {
        hash_table = MY_resize(hash_table);
    }

    int count = collision_count;

    int index1 = hash_fun1(key, hash_table->size);
    int index2 = index1;
    ELEMENT *table = hash_table->table;
    int prev_prime = get_prev_prime(hash_table->size);

    while (1) {
        if (table[index2].key == (int)NULL) {
            table[index2].data = data;
            table[index2].key = key;
            hash_table->fullness += 1;
            collision_count = count;
            return hash_table;
        } else {
            index2 = (index1 + ((count % 2000) * hash_fun2(key, prev_prime))) % hash_table->size;
            count++;
        }
    }
}

void MY_search(HASHMAP *hash_table, char *data, int wanted_key) {
    int index1 = hash_fun1(wanted_key, hash_table->size);
    int index2;
    int prev_prime = get_prev_prime(hash_table->size);

    if (hash_table->table[index1].key == wanted_key) {
        printf("MY_HASH Data '%s' is on index %d\n", data, index1);
        printf("O(1)\n");
        return;
    } else {
        for (int i = 1; i <= collision_count; i++) {
            index2 = (index1 + ((i % 2000) * hash_fun2(wanted_key, prev_prime))) % hash_table->size;
            if (hash_table->table[index2].key == wanted_key) {
                printf("MY_HASH Data '%s' is on index %d\n", data, index2);
                printf("O(%d)\n", i + 1);
                return;
            }
        }
        printf("Find nothing");
    }
}

void MY_print(HASHMAP *hash) {
    for (int i = 0; i < hash->size; i++) {
        printf("%d. %s\n", i, (char*)hash->table[i].data);
    }
}

void MY_delete_hashmap(HASHMAP *hash) {
    free(hash->table);
    free(hash);
}

