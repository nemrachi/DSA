#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>

//dlzka hrany sa odvija od toho, kam nasledujem

//"definovanie" bool typu
typedef char BOOL;
#define true 1
#define false 0

//minheap spravit s pointrami, tak si ulozim o ktore konkretne pole ide

//struktura pre mapu, ktora uchovava informacie o jednotlivych poliach
typedef struct MAP_I
{
    unsigned short int dist; //dlzka od zaciatocneho bodu //max +65,535
    //z akeho pola sme pristupovali do aktualeho pola
    unsigned short int x_p : 10;
    unsigned short int y_p : 10;
} MAP_I;

typedef struct RELAXED
{
    unsigned short int x : 10;
    unsigned short int y : 10;
} RELAXED;

#define WIDTH 7 //m (sirka mapy)
#define HEIGHT 5 //n (vyska mapy)
//konstatny spojene s bitovymi stavmi
#define POS_SIZE 10 //velkost x a y v bitoch
#define BIT_SIZE 1
#define X_OFF 23
#define Y_OFF 13
#define G_OFF 12
#define D_OFF 11
#define P1_OFF 10
#define P2_OFF 9
#define P3_OFF 8
#define P4_OFF 7
#define P5_OFF 6

#define SIZE_INT sizeof(int)
#define SIZE_P_INT sizeof(int *)
#define SIZE_CHAR sizeof(char)
#define SIZE_P_CHAR sizeof(char *)
#define UNVISITED 1023

#define PRINCESS_NUM 2

int *zachran_princezne(char **map, int n, int m, int t, int *dlzka_cesty);
int *get_short_path_dragon(char **mapa, MAP_I **map_info, int *dlzka_cesty);
RELAXED get_pos_dragon(char **mapa);
MAP_I **set_map_info(int n, int m, char **map);
char **allocate_char_map(int width, int height);
char **generate_map(int width, int height, int princess_num);
MAP_I **allocate_map(int width, int height);
void free_map(void **map);
char get_element(int element);
void print_map_info(MAP_I **map, int width, int height);
void print_map(char **map, int width, int height);
int get_dist(char ch);
int get_max(int a, int b);
int get_random_range(int lower, int upper);
int get_percentage(int num, int perc);
void add_min_heap(MAP_I **map, RELAXED **min_heap, RELAXED relaxed, int *heap_size);
RELAXED pop(MAP_I **map, RELAXED **min_heap, int *heap_size);
void min_heapify(MAP_I **map, RELAXED **min_heap, int size, int i);
void swap(RELAXED *a, RELAXED *b);
int get_parent_index(int num);
int left(int parent_index);
int right(int parent_index);
int32_t change_state(int32_t state, int new_val, int offset, int bit_size);
unsigned int count_bits(int num);
void dec_to_binary(int n);
int get_x(int32_t state);
int get_y(int32_t state);
int32_t change_x(int32_t state, int x);
int32_t change_y(int32_t state, int y);
int32_t change_G(int32_t state, int G);
int32_t change_D(int32_t state, int D);
int32_t change_P(int32_t state, int P, int num);

int main() 
{
    srand(time(0));

    //char **map = generate_map(WIDTH, HEIGHT, PRINCESS_NUM); //generovanie mapy (testovanie)
    char mapa[5][7] = {'H', 'H', 'H', 'C', 'N', 'N', 'H',
                    'C', 'P', 'D', 'C', 'C', 'H', 'C',
                    'H', 'H', 'P', 'C', 'H', 'C', 'C',
                    'N', 'C', 'C', 'C', 'C', 'C', 'C',
                    'C', 'C', 'C', 'C', 'C', 'C', 'C'};
    char **map = allocate_char_map(WIDTH, HEIGHT);
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            map[i][j] = mapa[i][j];
        }
    }
    
    int dlzka_cesty = 0;

    print_map(map, WIDTH, HEIGHT);

    //zachrana
    //t - cas, kedy sa drak zobudi (3. argument funkcie)
    int *cesta = zachran_princezne(map, HEIGHT, WIDTH, 35, &dlzka_cesty);
    printf("\n");
    for (int i = 0;i<dlzka_cesty;++i)
          printf("%d %d\n", cesta[i*2], cesta[i*2+1]);

    return 0;
}

//int dijskstra(MAP_I **map, int width, int height, )

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty) //n - vyska, m - sirka
{
    int *shortest_path_0 = malloc(SIZE_INT * 2 * n * m); //bez G
    int *shortest_path_1 = malloc(SIZE_INT * 2 * n * m); //s G
    int path_index = 0;

    //min-heap
    int heap_size = sizeof(RELAXED) * m * n;
    RELAXED *relaxed_min_heap = malloc(heap_size);
    heap_size = 0;

    //susedia pola
    RELAXED up, right, down, left, chosen_one; //aj hore a aj dole sa musim pozerat
    int x, y;

    //x a y su v binarnej sustave \/ (x a y su suradnicove hodnoty aktualnej polohy)
    //                          x          y      G D P P P P P         
    //     char state[] = "|0000000000|0000000000|0|0|0|0|0|0|0|00000"; //32 bitov (5 nevyuzitych)
    static int32_t state = 0;

    //druha mapa, ktora udrziava extra informacie o kazdom poli
    MAP_I **map_info = set_map_info(n, m, mapa);
    
    //        |x
    //        |
    // y______|________ y
    //        |
    //        |x
    x = get_x(state);
    y = get_y(state);
    map_info[x][y].x_p = 0;
    map_info[x][y].y_p = 0;
    map_info[x][y].dist = 0;

    //dijkstra_____________________________________________________________________
    while (true)
    {
        shortest_path_0[path_index++] = x;
        shortest_path_0[path_index++] = y;
        //look up
        if (((x - 1) >= 0) && (mapa[x-1][y] != 'N'))
        {
            if ((map_info[x-1][y].x_p == UNVISITED) || (map_info[x-1][y].dist > (map_info[x][y].dist + get_dist(mapa[x-1][y]))))
            {
                up.x = x - 1;
                up.y = y;
                map_info[x-1][y].x_p = x;
                map_info[x-1][y].y_p = y;
                map_info[x-1][y].dist = map_info[x][y].dist + get_dist(mapa[x-1][y]);
                add_min_heap(map_info, &relaxed_min_heap, up, &heap_size);
            }
        }
        //look right
        if (((y + 1) <= m) && (mapa[x][y+1] != 'N'))
        {
            if ((map_info[x][y+1].x_p == UNVISITED) || (map_info[x][y+1].dist > (map_info[x][y].dist + get_dist(mapa[x][y+1]))))
            {
                right.x = x;
                right.y = y + 1;
                map_info[x][y+1].x_p = x;
                map_info[x][y+1].y_p = y;
                map_info[x][y+1].dist = map_info[x][y].dist + get_dist(mapa[x][y+1]);
                add_min_heap(map_info, &relaxed_min_heap, right, &heap_size);
            }
        }
        //look down
        if (((x + 1) < n) && (mapa[x+1][y] != 'N'))
        {
            if ((map_info[x+1][y].x_p == UNVISITED) || (map_info[x+1][y].dist > (map_info[x][y].dist + get_dist(mapa[x+1][y]))))
            {
                down.x = x + 1;
                down.y = y;
                map_info[x+1][y].x_p = x;
                map_info[x+1][y].y_p = y;
                map_info[x+1][y].dist = map_info[x][y].dist + get_dist(mapa[x+1][y]);
                add_min_heap(map_info, &relaxed_min_heap, down, &heap_size);
            }
        }
        //look left
        if (((y - 1) >= 0) && (mapa[x][y-1] != 'N'))
        {
            if ((map_info[x][y-1].x_p == UNVISITED) || (map_info[x][y-1].dist > (map_info[x][y].dist + get_dist(mapa[x][y-1]))))
            {
                left.x = x;
                left.y = y - 1;
                map_info[x][y-1].x_p = x;
                map_info[x][y-1].y_p = y;
                map_info[x][y-1].dist = map_info[x][y].dist + get_dist(mapa[x][y-1]);
                add_min_heap(map_info, &relaxed_min_heap, left, &heap_size);
            }
        }

        chosen_one = pop(map_info, &relaxed_min_heap, &heap_size);
        x = chosen_one.x;
        y = chosen_one.y;

        if(heap_size == 0)
            break;
    }    
    //dijkstra_____________________________________________________________________
    print_map_info(map_info, WIDTH, HEIGHT);
    free(shortest_path_0);
    free(shortest_path_1);
    free(relaxed_min_heap);
  
    return get_short_path_dragon(mapa, map_info, dlzka_cesty);
}

int *get_short_path_dragon(char **mapa, MAP_I **map_info, int *dlzka_cesty)
{
    RELAXED dragon_pos = get_pos_dragon(mapa);
    (*dlzka_cesty) += map_info[dragon_pos.x][dragon_pos.y].dist; 
    int counter = 0, i = 0;
    int x = dragon_pos.x, y = dragon_pos.y, temp_x;
    int *short_path = malloc(SIZE_INT * WIDTH * HEIGHT * 2);
    while (true)
    {
        short_path[i++] = x;
        short_path[i++] = y;
        if ((x == 0) && (y == 0))
            break;
        temp_x = map_info[x][y].x_p;
        y = map_info[x][y].y_p;
        x = temp_x;
    }

    return short_path;
}

RELAXED get_pos_dragon(char **mapa)
{
    RELAXED pos;
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if(mapa[i][j] == 'D')
            {
                pos.x = i;
                pos.y = j;
                return pos;
            }
        }
    }
}

void print_map_info(MAP_I **map, int width, int height) 
{
    printf("====================\n");
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(map[i][j].dist == USHRT_MAX)
                printf("X ");
            else
                printf("%d ", map[i][j].dist);
        }
        printf("\n");
    }
}

MAP_I **set_map_info(int height, int width, char **map) 
{
    MAP_I **map_info = (MAP_I **)allocate_map(width, height);
    for(int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            map_info[i][j].dist = USHRT_MAX; //"nekonecna" vzdialenost od zaciatocneho bodu
            map_info[i][j].x_p = UNVISITED;
            map_info[i][j].y_p = UNVISITED;
        }
    }
    return map_info;
}

MAP_I **allocate_map(int width, int height)
{
    MAP_I **map = (MAP_I **)malloc(height * sizeof(MAP_I *));
    for (int i = 0; i < height; i++)
    {
        map[i] = (MAP_I *)malloc(width * sizeof(MAP_I));
    }
    return map;
}

//generovanie mapy (testovanie vyhladavacieho algoritmu)
char **generate_map(int width, int height, int princess_num)
{    
    char **map = (char **)allocate_char_map(width, height);
    int element_index, propability_index;

    /*
    urcenie poctu jednotlivych prvkov na mape
    1 drak                              \
    (zatial konstantne) 2 princezne    -- staticke udaje
    (zatial konstantne) 2 teleporty     /

    pocet lesnych ciest = 65% zo zostatku (novy zostatok)
    pocet porastov = 70% z noveho zostatku (uplny zvysok)
    pocet nepriechodnych ciest = uplny zvysok
    */
    int dragon_num = 1;
    int teleport_num = 0; //teleporty su len 2 zatial s 2 vchodmi //predtym 4 (teleport moze mat vacero vrcholov)
    int generator_num = 0; //predtym 1
    int remainder = (width * height) - dragon_num - princess_num - teleport_num - generator_num;
    int trail_num = get_percentage(remainder, 65);
    remainder -= trail_num;
    int undergrowth_num = get_percentage(remainder, 70);
    int impassable_num = remainder - undergrowth_num;

    int elements_arr[] = {trail_num, undergrowth_num, impassable_num,
                            dragon_num, princess_num, teleport_num, generator_num};
    
    //velkost: 30
    int propability_arr[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //C -> pravdepodobnost: 0,36
                            1, 1, 1, 1, 1, 1, 1, //H -> pravdepodobnost: 0,23
                            2, 2, 2, //N -> pravdepodobnost: 0,1
                            3, //D -> pravdepodobnost: 0,03
                            4, 4, //P -> pravdepodobnost: 0,06
                            5, 5, 5, 5, //0-9 -> pravdepodobnost: 0,13
                            6, 6}; //G -> pravdepodobnost: 0,06

    //vkladanie elementov do mapy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            while (1)
            {   
                if (i == 0 && j == 0)
                {
                    element_index = get_random_range(0, 1);
                    map[i][j] = get_element(element_index);
                    elements_arr[element_index]--;
                    break;
                }
                propability_index = get_random_range(0, 29);
                element_index = propability_arr[propability_index];
                if (element_index != 5) {
                    if (elements_arr[element_index]) 
                    {
                        map[i][j] = get_element(element_index);
                        elements_arr[element_index]--;
                        break;
                    }
                }
            }
        }
    }

    return map;
}

char **allocate_char_map(int width, int height)
{
    char **map = (char **)malloc(height * SIZE_P_CHAR);
    for (int i = 0; i < height; i++)
    {
        map[i] = (char *)malloc(width * SIZE_CHAR);
    }
    return map;
}

void free_map(void **map)
{
    for (int i = 0; i < WIDTH; i++)
    {
        free(map[i]);
    }
    free(map);
}

//funkcia vrati element podla cisla, ktore je vlozene do funkcie
char get_element(int element) 
{
    static char teleport_id = '0';
    static int teleport_count = 0;

    switch (element)
    {
        case 0:
            return 'C'; //cesta
        
        case 1:
            return 'H'; //hustina

        case 2:
            return 'N'; //neprekonatelna prekazka

        case 3:
            return 'D'; //drak

        case 4:
            return 'P'; //princezna

        case 5: //teleporty //momentalne generujem len 2 vchody pre jeden teleport
            if (2 == teleport_count++)
            {
                teleport_id++;
                teleport_count = 0;
            }
            return teleport_id;

        case 6:
            return 'G'; //generator
        
        default:
            return ' ';
    }
}

//funkcia vytlaci mapu
void print_map(char **map, int width, int height) 
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            printf("%c ", map[i][j]);
        }
        printf("\n");
    }
}

int get_dist(char ch)
{
    static int princess_counter = PRINCESS_NUM;
    switch (ch)
    {
    case 'C':
        return 1;
        break;
    
    case 'H':
        return 2;
        break;
    
    case 'N':
        return 0;
        break;
    
    case 'D':
        return 1;
        break;
    
    case 'P':
        princess_counter--;
        return ((princess_counter) ? 1 : INT_MAX);
        break;
    
    default:
        return -1;
        break;
    }
}

int get_max(int a, int b)
{
     return((a > b) ? a : b);
}

//funkcia vrati nahodne cisla z rozmedzia urceneho pouzivatelom
int get_random_range(int lower, int upper) 
{
    return (rand() % (upper + 1 - lower)) + lower;
}

//funckia vypocita hodnotu  percenta(perc) z cisla num
int get_percentage(int num, int perc) 
{   
    float div = (float)perc / 100;
    float result = div * num;
    return ceil(result); //zaokruhly nahor
}


//*************************************************//
//              Funckie - min heap                 //
//*************************************************//
void add_min_heap(MAP_I **map, RELAXED **min_heap, RELAXED relaxed, int *heap_size)
{
    (*heap_size)++;
    (*min_heap)[((*heap_size) - 1)] = relaxed;

    if ((*heap_size) == 1)
        return;

    int i = (*heap_size) - 1; //posledne cislo v heape
    int parent_i = get_parent_index(i);

    int parent_x = (*min_heap)[parent_i].x;
    int parent_y = (*min_heap)[parent_i].y;
    int curr_x = (*min_heap)[i].x;
    int curr_y = (*min_heap)[i].y;

    while ((i != 0) && (map[parent_x][parent_y].dist > map[curr_x][curr_y].dist)) 
    {
        swap(&(*min_heap)[i], &(*min_heap)[parent_i]);
        i = get_parent_index(i);
        parent_i = get_parent_index(i);
        parent_x = (*min_heap)[parent_i].x;
        parent_y = (*min_heap)[parent_i].y;
        curr_x = (*min_heap)[i].x;
        curr_y = (*min_heap)[i].y;
    }
}

//funkcia vyhodi najmensie cislo z min heapu
RELAXED pop(MAP_I **map, RELAXED **min_heap, int *heap_size)
{
    if ((*heap_size) == 1)
    {
        (*heap_size)--;
        return (*min_heap)[0];
    }

    RELAXED popped = (*min_heap)[0];
    (*min_heap)[0] = (*min_heap)[((*heap_size)-1)]; //posledne cislo dam na prve
    (*heap_size)--;
    min_heapify(map, min_heap, *heap_size, 0);

    return popped;
}

void min_heapify(MAP_I **map, RELAXED **min_heap, int size, int i)
{
    int l = left(i);
    int r = right(i);
    int l_x = (*min_heap)[l].x, l_y = (*min_heap)[l].y;
    int r_x = (*min_heap)[r].x, r_y = (*min_heap)[r].y;
    int curr_x = (*min_heap)[i].x, curr_y = (*min_heap)[i].y;
    int smallest = i;

    if ((l < size) && (map[l_x][l_y].dist < map[curr_x][curr_y].dist))
        smallest = l;
    if ((r < size) && (map[r_x][r_y].dist < map[curr_x][curr_y].dist))
        smallest = r;
    if (smallest != i)
    {
        swap(&(*min_heap)[i], &(*min_heap)[smallest]);
        min_heapify(map, min_heap, size, smallest);
    }
}

void swap(RELAXED *a, RELAXED *b)
{
    RELAXED temp = *a;
    *a = *b;
    *b = temp;
}

int get_parent_index(int num)
{
    return ((int)ceil((double)((double)num/(double)2)) - 1);
}

int left(int parent_index)
{
    return (2*parent_index + 1);
}

int right(int parent_index)
{
    return (2*parent_index + 2);
}
//*************************************************//

//*************************************************//
//         Funckie na binarne operacie             //
//*************************************************//
int32_t change_state(int32_t state, int new_val, int offset, int bit_size)
{
    int val_size = count_bits(new_val);
    new_val <<= offset;
    int reset_mask = ~(((1 << bit_size) - 1) << offset); // 3 - rozdiel medzi offsetmi
    state &= reset_mask; //nastavi na 0 tam, kde bude davat nove bity
    return (state & (~new_val)) | new_val;
}

unsigned int count_bits(int num) 
{       
      return (int)log2(num)+1; 
} 

//iba pre kontrolu - vypise cislo v 2 sustave
void dec_to_binary(int n) 
{ 
    int binaryNum[32]; 
    int i = 0; 
    while (n > 0) { 
        binaryNum[i] = n % 2; 
        n = n / 2; 
        i++; 
    }
    for (int j = i - 1; j >= 0; j--) 
        printf("%d\n",  binaryNum[j]);
    printf("\n");
} 

int get_x(int32_t state)
{
    return (state >> X_OFF);
}

int get_y(int32_t state)
{
    return (state & (~(((1 << POS_SIZE) - 1) << POS_SIZE))) >> Y_OFF;
}

int32_t change_x(int32_t state, int x)
{
    return change_state(state, x, X_OFF, POS_SIZE);
}

int32_t change_y(int32_t state, int y)
{
    return change_state(state, y, Y_OFF, POS_SIZE);
}

int32_t change_G(int32_t state, int G)
{
    return change_state(state, G, G_OFF, BIT_SIZE);
}

int32_t change_D(int32_t state, int D)
{
    return change_state(state, D, D_OFF, BIT_SIZE);
} 

int32_t change_P(int32_t state, int P, int num)
{
    switch (num)
    {
    case 1:
        return change_state(state, P, P1_OFF, BIT_SIZE);
        break;

    case 2:
        return change_state(state, P, P2_OFF, BIT_SIZE);
    break;

    case 3:
        return change_state(state, P, P3_OFF, BIT_SIZE);
    break;

    case 4:
        return change_state(state, P, P4_OFF, BIT_SIZE);
    break;

    case 5:
        return change_state(state, P, P5_OFF, BIT_SIZE);
    break;
    
    default:
        break;
    }
}
//*************************************************//