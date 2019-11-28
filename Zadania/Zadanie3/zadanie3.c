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

//struktura pre mapu, ktora uchovava informacie o jednotlivych poliach
typedef struct MAP_I
{
    char element; //prvok v poli
    BOOL visited; //ci bolo pole relaxovane
    unsigned short int dist; //dlzka od zaciatocneho bodu //max +65,535
    //z akeho pola sme pristupovali do aktualeho pola
    unsigned short int x_p;
    unsigned short int y_p;
} MAP_I;

#define WIDTH 7 //m (sirka)
#define HEIGHT 5 //n (vyska)
#define SIZE_INT sizeof(int)
#define SIZE_P_INT sizeof(int *)
#define SIZE_CHAR sizeof(char)
#define SIZE_P_CHAR sizeof(char *)
#define PRINCESS_NUM 2

int *zachran_princezne(char **map, int n, int m, int t, int *dlzka_cesty);
char **generate_map(int width, int height, int princess_num);
char **allocate_char_map(int width, int height);
MAP_I **allocate_map(int width, int height);
MAP_I **set_map_info(int n, int m, char **map);
void print_map(char **map, int width, int height);
char get_element(int element);
int get_dist(char ch);
int get_max(int a, int b);
int get_random_range(int lower, int upper);
int get_percentage(int num, int perc);
int get_parent_index(int num);
int32_t change_state(int32_t state, int new_val, int val_size, int offset);
unsigned int count_bits(int num);
void dec_to_binary(int n);
int get_x(int32_t state);
int get_y(int32_t state);

int main() 
{
    srand(time(0));

    char **map = generate_map(WIDTH, HEIGHT, PRINCESS_NUM); //generovanie mapy (testovanie)
    unsigned short int dlzka_cesty;

    print_map(map, WIDTH, HEIGHT);

    //zachrana
    //t - cas, kedy sa drak zobudi (3. argument funkcie)
    int *cesta = zachran_princezne(map, HEIGHT, WIDTH, 35, &dlzka_cesty);
    // for (int i = 0;i<dlzka_cesty;++i)
    //      printf("%d %d\n", cesta[i*2], cesta[i*2+1]);

    for (int i = 0; i < WIDTH; i++)
    {
        free(map[i]);
    }
    free(map);

    return 0;
}

//int dijskstra(MAP_I **map, int width, int height, )

int *zachran_princezne(char **map, int n, int m, int t, int *dlzka_cesty) //n - vyska, m - sirka
{
    int *shortest_path_0 = malloc(SIZE_INT * 2 * n * m); //bez G
    int *shortest_path_1 = malloc(SIZE_INT * 2 * n * m); //s G
    int path_index = 0;
    MAP_I *relaxed_min_heap = malloc(sizeof(MAP_I) * m * n);
    MAP_I right, down;
    int x, y;

    //x a y su v binarnej sustave \/ (x a y su suradnicove hodnoty aktualnej polohy)
    //                          x          y    GDPPPPP         
    //                     |         |         ||||||||              
    //     char state[] = "0000000000000000000000000000000"; //32 bitov
    static int32_t state = 0;

    //druha mapa, ktora udrziava extra informacie o kazdom poli
    MAP_I **map_info = set_map_info(n, m, map);

    //        |x
    //        |
    // y______|________ y
    //        |
    //        |x
    x = get_x(state);
    y = get_y(state);
    map_info[x][y].visited = true;
    map_info[x][y].dist = 0;
    shortest_path_0[path_index++] = x;
    shortest_path_0[path_index++] = y;
    //dijkstra_____________________________________________________________________
    while (true)
    {
        if (((x + 1) >= m))
        {
            //len na y pozeram
        } 
        else if (((y + 1) >= n))
        {
            //len na x pozeram
        }
        else
        {
            //ak by platili 2 predosle podmienky, to este neviem co
            right = map_info[x][y+1];
            if (right.element != 'N')
                right.dist = map_info[x][y].dist + get_dist(right.element); //relax
            down = map_info[x+1][y];
            if (down.element != 'N')
                down.dist = map_info[x][y].dist + get_dist(down.element); //relax

        }
        
    }
    

    
  

    
    //dijkstra_____________________________________________________________________
}

MAP_I **set_map_info(int height, int width, char **map) 
{
    MAP_I **map_info = (MAP_I **)allocate_map(width, height);
    for(int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            map_info[i][j].element = map[i][j];
            map_info[i][j].visited = false;
            map_info[i][j].dist = USHRT_MAX; //"nekonecna" vzdialenost od zaciatocneho bodu
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
            switch (map[i][j])
            {
            case 'C':
                printf("C ");
                break;
            
            case 'H':
                printf("H ");
                break;
            
            case 'N':
                printf("N ");
                break;
            
            case 'D':
                printf("D ");
                break;
            
            case 'P':
                printf("P ");
                break;
            
            case 'G':
                printf("G ");
                break;
            
            default:
                printf("%c ", map[i][j]);
                break;
            }
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
//funkcie k min heapu________________________________
void add_min_heap(MAP_I **min_heap, MAP_I relaxed)
{
    static int last_index = 0;
    int act_index = last_index, parent_index = get_parent_index(last_index);
    MAP_I temp;
    (*min_heap)[last_index] = relaxed;

    while (true)
    {
        if (parent_index >= 0 && (*min_heap)[parent_index].dist > relaxed.dist)
        {
            temp = (*min_heap)[parent_index];
            (*min_heap)[parent_index] = relaxed;
            (*min_heap)[act_index] = temp;
            act_index = parent_index;
            parent_index = get_parent_index(parent_index);
            continue;
        }
        break;
    }
    last_index++;    
}

int get_parent_index(int num)
{
    return ((int)ceil((double)((double)num/(double)2)) - 1);
}
//funkcie k min heapu________________________________

//funkcie k binarnim operaciam_______________________
int32_t change_state(int32_t state, int new_val, int val_size, int offset)
{
    new_val <<= (val_size - count_bits(new_val));
    offset -= val_size; //nie je to osetrene, lebo nemoze sa stat ituacia, ze to bude <0
    int reset_mask = ~(((1 << val_size) - 1) << offset);
    state &= reset_mask; //nastavi na 0 tam, kde bude davat nove bity
    int bit_mask = (new_val << offset);
    return (state & (~bit_mask)) | bit_mask;
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
//funkcie k binarnim operaciam_______________________

//stavove funkcie___________________________________
int get_x(int32_t state)
{
    return (state >> 21);
}

int get_y(int32_t state)
{
    return ((state << 9) >> 20);
}

int32_t change_x(int32_t state, int x)
{
    return change_state(state, x, 10, 31);
}

int32_t change_y(int32_t state, int y)
{
    return change_state(state, y, 10, 21);
}
//stavove funkcie___________________________________