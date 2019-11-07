typedef struct {
    char *data;
    int key;
} ELEMENT;

typedef struct{
    ELEMENT *table;
    int size, fullness;
} HASH_TABLE;

#define MAX_TABLE_SIZE 100003

//insert, search, print, prime number, resize
int collision_count = 1;

int get_next_prime(int size) {
    int is_prime = 1;
    ++size;

    while (size < 100) {
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

HASH_TABLE *MY_init(int size) {
    HASH_TABLE *hash_table = malloc(sizeof(HASH_TABLE));
    hash_table->size = size;
    hash_table->fullness = 0;
    hash_table->table = NULL;
    return  hash_table;
}

int hash_fun1(int key, int size){
    return (key % size);
}

int hash_fun2(int hash1, char *data, int size){
    int len = (int)strlen(data);
    return (hash1 + (len * ((2 * (int)data[0]) + ((int)data[len-1] % 11)))) % size;
}

HASH_TABLE *MY_insert(HASH_TABLE *hash_table, char *data, int key) {
    if (hash_table->size <= hash_table->fullness) {
        //rehash
    }

    int index1 = hash_fun1(key, hash_table->size);
    int index2 = index1;
    ELEMENT *table;

    if (hash_table->table == NULL) {
         table = malloc(hash_table->size * sizeof(ELEMENT));
    } else{
         table = hash_table->table;
    }


    while (1) {
        if (table[index2].data == NULL) {
            table[index2].data = data;
            table[index2].key = key;
            hash_table->fullness += 1;
            hash_table->table = table;
            return hash_table;
        } else {
            index2 = index1 + (collision_count * hash_fun2(index1, data, hash_table->size));
            collision_count++;
        }
    }
}

void MY_print(HASH_TABLE *hash) {
    for (int i = 0; i < hash->size; i++)
    {
        printf("%d. %s\n", i, (char*)hash->table[i].data);
    }
}

void MY_delete_hashmap(HASH_TABLE *hash)
{
    free(hash->table);
    free(hash);
}

