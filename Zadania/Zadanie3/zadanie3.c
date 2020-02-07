//zmena distance na int
//dlzka hrany sa odvija od toho, kam nasledujem
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>

//V MIN HEAPE MOZEM MAT ROVNAKE POLIA - TREBA ICH DETEKOVAT a updatnut

//struktura pre mapu, ktora uchovava informacie o jednotlivych poliach
typedef struct MAP_I
{
    unsigned dist; //dlzka od zaciatocneho bodu //max +65,535
    //pozicia pola, z akeho sme pristupovali do aktualeho pola
    uint32_t xy_p;
} MAP_I;

//struktura pre poziciu
typedef struct POSITION
{
    uint32_t xy;
} POSITION;

//konstatny pre pracu so stavom Popolvara (pre bitove operacie)
#define POS_SIZE 12
#define BIT_SIZE 1
#define X_OFF 22
#define Y_OFF 12
#define G_OFF 11
#define D_OFF 10
#define P1_OFF 9
#define P2_OFF 8
#define P3_OFF 7
#define P4_OFF 6
#define P5_OFF 5

#define SIZE_INT sizeof(int)
#define SIZE_CHAR sizeof(char)
#define SIZE_P_CHAR sizeof(char *)
#define SIZE_MAP_I sizeof(MAP_I)
#define SIZE_P_MAP_I sizeof(MAP_I *)

#define WIDTH 6 //m (sirka mapy)
#define HEIGHT 4 //n (vyska mapy)
#define UNRELAXED 1000 //"nekonecno" (USHRT_MAX)
#define PRINCESS_NUM 2

//*************************************************//
//                Hlavne funkcie                   //
//*************************************************//
int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty);
MAP_I **dijkstra(char **mapa, MAP_I **map_info, uint32_t state, int height, int width, int **shortest_path, int *path_index);
void change_xy_POS(POSITION *neighbour, int new_x, int new_y);
void change_map_info(MAP_I ***map_info, int new_x, int new_y, int x, int y, unsigned actual_dist, unsigned neighbour_dist);
void find_positions(char **mapa, int height, int width, POSITION *dragon_pos, POSITION **princess_pos, int *princess_counter);
void set_path(int **path, int size);
void get_path(int **path, int path_size, MAP_I **map_info, POSITION last_pos);
void reverse_path(int **path, int path_size);
void print_path(int *path);
void shortest_princess_path(MAP_I **map_info, uint32_t *state, POSITION **princess_pos, int princess_counter, int **dlzka_cesty);
//*************************************************//
//          Funckie pre pracu s mapami             //
//*************************************************//
char **allocate_char_map(int height, int width);
void free_char_map(char **mapa, int height);
char **generate_map(int height, int width, int princess_num);
char get_element(int element);
int get_dist(char element);
void print_map(char **mapa, int height, int width);
MAP_I **allocate_map_info(int height, int width);
void free_map_info(MAP_I **map_info, int height);
MAP_I **set_map_info(int height, int width);
MAP_I **reset(MAP_I **map_info, int height, int width);
void print_map_info(MAP_I **map_info, int height, int width, char ch);
//*************************************************//
//              Vedlajsie funckie                  //
//*************************************************//
int get_random_range(int lower, int upper);
int get_percentage(int num, int percentage);
//*************************************************//
//         Funckie pre min heap                    //
//*************************************************//
void add_min_heap(MAP_I **map_info, POSITION **min_heap, POSITION relaxed, int *heap_size);
POSITION pop(MAP_I **map_info, POSITION **min_heap, int *heap_size);
void min_heapify(MAP_I **map_info, POSITION **min_heap, int heap_size, int i);
void swap(POSITION *a, POSITION *b);
int get_parent_index(int num);
int left(int parent_i);
int right(int parent_i);
void print_min_heap(MAP_I **map_info, POSITION *min_heap, int heap_size);
//*************************************************//
//         Funckie na binarne operacie             //
//*************************************************//
uint32_t change_state(uint32_t state, unsigned new_val, unsigned offset, unsigned bit_size);
unsigned get_x(uint32_t state);
uint32_t change_x(uint32_t state, unsigned x);
unsigned get_y(uint32_t state);
uint32_t change_y(uint32_t state, unsigned y);
unsigned get_G(uint32_t state);
uint32_t change_G(uint32_t state, unsigned G);
unsigned get_D(uint32_t state);
uint32_t change_D(uint32_t state, unsigned D);
unsigned get_P(uint32_t state, unsigned num);
uint32_t change_P(uint32_t state, unsigned num, unsigned P);
unsigned print_bin32(uint32_t num, int bit);

int main()
{
    srand(time(0)); //kvoli generovaniu nahodnych cisel pre generovanie mapy

    //char **map = generate_map(HEIGHT, WIDTH, PRINCESS_NUM); //generovanie mapy (pre testovanie)
    char mapa[HEIGHT][WIDTH] = {{'C', 'C', 'C', 'N', 'H', 'C'},
                                {'C', 'C', 'H', 'C', 'N', 'C'},
                                {'C', 'C', 'C', 'C', 'C', 'H'},
                                {'P', 'C', 'H', 'H', 'P', 'D'}};
    char **map = allocate_char_map(HEIGHT, WIDTH);
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            map[i][j] = mapa[i][j];
        }
    }

    //*************zachrana*************
    int dlzka_cesty = 0;
    //t - cas, kedy sa drak zobudi (3. argument funkcie)
    int *cesta = zachran_princezne(map, HEIGHT, WIDTH, 100, &dlzka_cesty);
    printf("dlzka cesty: %d\n", dlzka_cesty);

    for (int i = 0; i < (dlzka_cesty + 1); i++)
    {
        printf("%d %d\n", cesta[i*2], cesta[i*2+1]);
        // if ((cesta[i*2] == 0) && (cesta[i*2+1] == 0))
        //     break;
    }

    printf("KONIEC");

    return 0;
}

//m (sirka mapy)
//n (vyska mapy)
//*************************************************//
//                Hlavne funkcie                   //
//*************************************************//
int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty)
{
    print_map(mapa, n, m);
    int path_size = SIZE_INT * 4 * n * m;
    //zaznam najkratsej cesty (uchovava suradnice policok (x, y))
    int *shortest_path = malloc(path_size);
    set_path(&shortest_path, (2 * n * m));
    int path_index = 0;

    //druha mapa, ktora udrziava extra informacie* o kazdom poli v povodnej mape
    //*extra informacie: vzdialenost od zaciatocneho bodu a
    //suradnice predosleho policka, z ktoreho sme sa dostali na aktualne
    MAP_I **map_info = set_map_info(n, m);

    //(x a y su suradnicove hodnoty polohy zaciatku cesty)
    //              x          y          G D P P P P P
    //state = "|000000000000|000000000000|0|0|0|0|0|0|0|0";
    uint32_t state = 0;

    //        |x
    //        |
    // y______|________ y
    //        |
    //        |x
    //ulozene dolezite pozicie v mape //potom pridat aj generator
    POSITION dragon_pos, *princess_pos = malloc(5 * sizeof(POSITION));
    int princess_counter = 0;
    find_positions(mapa, n, m, &dragon_pos, &princess_pos, &princess_counter);

    //zabi draka
    map_info = dijkstra(mapa, map_info, state, n, m, &shortest_path, &path_index);
    //drak zabity
    (*dlzka_cesty) += map_info[get_x(dragon_pos.xy)][get_y(dragon_pos.xy)].dist;
    state = change_D(state, 1);
    state = change_x(state, get_x(dragon_pos.xy));
    state = change_y(state, get_y(dragon_pos.xy));
    printf("\npos p1 x:%d  y:%d\npos p2 x:%d  y:%d\n", get_x(princess_pos[0].xy), get_y(princess_pos[0].xy), get_x(princess_pos[1].xy), get_y(princess_pos[1].xy));
    printf("DRAK %d(%d) %d(%d) \tdlzka cesty: %d _____________________________________________\n", get_x(state), get_x(dragon_pos.xy), get_y(state), get_y(dragon_pos.xy), (*dlzka_cesty));
    get_path(&shortest_path, path_size, map_info, dragon_pos);
    print_path(shortest_path);
    print_map_info(map_info, n, m, 'a');
    //TUTO JE NIEKDE CHYBA
    //zachran princeze
    for (int i = princess_counter; i > 0; i--)
    {
        printf("\nprincezna %d\n\n", i);
        map_info = reset(map_info, n, m);
        map_info = dijkstra(mapa, map_info, state, n, m, &shortest_path, &path_index);
        printf("shortest to princess\n");
        shortest_princess_path(map_info, &state, &princess_pos, princess_counter, &dlzka_cesty);
        printf("P x:%d y:%d\n", get_x(state), get_y(state));
    }

    free_map_info(map_info, n);
    free_char_map(mapa, n);

    return shortest_path;
}

MAP_I **dijkstra(char **mapa, MAP_I **map_info, uint32_t state, int height, int width, int **shortest_path, int *path_index)
{
    //min-heap
    POSITION *min_heap = malloc(sizeof(POSITION) * height * width);
    int heap_size = 0;

    POSITION neighbour /*sused pola*/, chosen_one;
    int x, y;
    int count = 0;

    //zaciatocna pozicia je ulozena v premennej state
    x = get_x(state);
    y = get_y(state);
    map_info[x][y].dist = 0;

    while (1)
    {
        count++;

        if (((x - 1) >= 0) && ((x - 1) < height))
        {
            //look up
            if (((count == 1) || ((x - 1) != get_x(map_info[x][y].xy_p)))
                && ((x - 1) != get_x(state)) && (mapa[x-1][y] != 'N') &&
                (map_info[x-1][y].dist > (map_info[x][y].dist + get_dist(mapa[x-1][y]))))
            {
                change_xy_POS(&neighbour, x - 1, y);
                change_map_info(&map_info, x - 1, y, x, y, map_info[x][y].dist, get_dist(mapa[x-1][y]));
                add_min_heap(map_info, &min_heap, neighbour, &heap_size);
                // if (get_D(state) == 1)
                // {
                //     printf("up\tx:%d  y:%d\n", neighbour.x, neighbour.y);
                //     print_min_heap(map_info, min_heap, heap_size);
                // }
            }
        }

        if (((y + 1) < width) && ((y + 1) >= 0))
        {
            //look right
            if (((count == 1) || ((y + 1) != get_y(map_info[x][y].xy_p)))
                && (mapa[x][(y+1)] != 'N') && ((y+1) != get_y(state)) &&
                (map_info[x][(y+1)].dist > (map_info[x][y].dist + get_dist(mapa[x][y+1]))))
            {
                change_xy_POS(&neighbour, x, y + 1);
                change_map_info(&map_info, x, y + 1, x, y, map_info[x][y].dist, get_dist(mapa[x][y+1]));             
                add_min_heap(map_info, &min_heap, neighbour, &heap_size);
            }
        }
        
        if (((x + 1) < height) && ((x + 1) >= 0))
        {
            //look down
            if (((count == 1) || ((x + 1) != get_x(map_info[x][y].xy_p)))
                && (mapa[x+1][y] != 'N') && ((x+1) != get_x(state)) &&
                (map_info[(x+1)][y].dist > (map_info[x][y].dist + get_dist(mapa[x+1][y]))))
            {
                change_xy_POS(&neighbour, x + 1, y);
                change_map_info(&map_info, x + 1, y, x, y, map_info[x][y].dist, get_dist(mapa[x+1][y]));
                add_min_heap(map_info, &min_heap, neighbour, &heap_size);
            }
        }

        if (((y - 1) >= 0) && ((y - 1) < width))
        {
            //look left
            if (((count == 1) || ((y - 1) != get_y(map_info[x][y].xy_p)))
                && (mapa[x][y-1] != 'N') && ((y-1) != get_y(state)) &&
                (map_info[x][(y-1)].dist > (map_info[x][y].dist + get_dist(mapa[x][y-1]))))
            {
                change_xy_POS(&neighbour, x, y - 1);
                change_map_info(&map_info, x, y - 1, x, y, map_info[x][y].dist, get_dist(mapa[x][y-1]));
                add_min_heap(map_info, &min_heap, neighbour, &heap_size);
            }
        }

        chosen_one = pop(map_info, &min_heap, &heap_size);
        x = get_x(chosen_one.xy);
        y = get_y(chosen_one.xy);

        if (get_D(state) == 1)
        {
            printf("chosen x:%d  y:%d\n", x, y);
            print_map_info(map_info, height, width, 'd');
            printf("\n\n");
        }

        if((heap_size == 0) && (count > ((height * width) / 2)))
            break;
    }

    free(min_heap);
    return map_info;
}

void change_xy_POS(POSITION *neighbour, int new_x, int new_y)
{
    (*neighbour).xy = change_x((*neighbour).xy, new_x);
    (*neighbour).xy = change_y((*neighbour).xy, new_y);
}

void change_map_info(MAP_I ***map_info, int new_x, int new_y, int x, int y, unsigned actual_dist, unsigned neighbour_dist)
{
    (*map_info)[new_x][new_y].xy_p = change_x((*map_info)[new_x][new_y].xy_p, x);
    (*map_info)[new_x][new_y].xy_p = change_y((*map_info)[new_x][new_y].xy_p, y);
    (*map_info)[new_x][new_y].dist = actual_dist + neighbour_dist;
}

void find_positions(char **mapa, int height, int width, POSITION *dragon_pos, POSITION **princess_pos, int *princess_counter)
{
    //potom pridat aj poziciu generatora a aj teleportov (pre teleporty bud 2d pole alebo struktura)
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if(mapa[i][j] == 'D')
            {
                (*dragon_pos).xy = change_x((*dragon_pos).xy, i);
                (*dragon_pos).xy = change_y((*dragon_pos).xy, i);
            }
            else if (mapa[i][j] == 'P')
            {
                (*princess_pos)[(*princess_counter)].xy = change_x((*princess_pos)[(*princess_counter)].xy, i);
                (*princess_pos)[(*princess_counter)].xy = change_y((*princess_pos)[(*princess_counter)].xy, j);
                (*princess_counter)++;
            }
}

void set_path(int **path, int size)
{
    for (int i = 0; i < size; i++)
        (*path)[i] = INT_MAX;
}

void get_path(int **path, int path_size, MAP_I **map_info, POSITION last_pos)
{
    //printf("\tget path\n");
    int tmp_x, tmp_y, x = get_x(last_pos.xy), y = get_y(last_pos.xy), i = 0;
    while (map_info[x][y].dist != 0)
    {
        (*path)[i] = x;
        //printf("\t%d ", (*path)[i]);
        i++;
        (*path)[i] = y;
        //printf("%d\n", (*path)[i]);
        i++;
        tmp_x = get_x(map_info[x][y].xy_p);
        tmp_y = get_y(map_info[x][y].xy_p);
        x = tmp_x;
        y = tmp_y;
    }
    (*path)[i] = x;
    //printf("\t%d ", (*path)[i]);
    i++;
    (*path)[i] = y;
    //printf("%d\n", (*path)[i]);
    i++;
    (*path)[i] = x;
    //printf("\t%d ", (*path)[i]);
    i++;
    (*path)[i] = y;
    //printf("%d\n", (*path)[i]);
    reverse_path(path, i);
}

void reverse_path(int **path, int path_size)
{
    printf("REVERZUJEM %d\n", path_size);
    int end_x = (path_size/SIZE_INT) - 2, end_y = (path_size/SIZE_INT) - 1, i = 0, temp;
    while (i < end_x) 
    { 
        temp = (*path)[i];  
        (*path)[i] = (*path)[end_x]; 
        (*path)[end_x] = temp; 
        i++; 
        end_x -= 2;

        temp = (*path)[i];  
        (*path)[i] = (*path)[end_y]; 
        (*path)[end_y] = temp; 
        i++; 
        end_y -= 2; 
    }
}

void print_path(int *path)
{
    printf("x  y\n");
    int i = 0;
    while (path[i] != INT_MAX)
    {
        printf("%d  %d\n", path[i], path[i+1]);
        i += 2; 
    }
}

void shortest_princess_path(MAP_I **map_info, uint32_t *state, POSITION **princess_pos, int princess_counter, int **dlzka_cesty)
{
    printf("\nshortest p path\n");
    POSITION temp, best;
    int best_i = 0, i;

    for (i = 0; i < princess_counter; i++)
        if (get_P((*state), i) == (unsigned) 0)
        {
            best = (*princess_pos)[i];
            break;
        }

    for (i = 0; i < princess_counter; i++)
    {
        temp = (*princess_pos)[i];
        if ((map_info[get_x(temp.xy)][get_x(temp.xy)].dist < map_info[get_x(best.xy)][get_y(best.xy)].dist) && (get_P((*state), i) == 0))
        {
            best = temp;
            best_i = i;
        }
    }

    (*state) = change_P((*state), 1, best_i);
    (*state) = change_x((*state), get_x((*princess_pos)[best_i].xy));
    (*state) = change_y((*state), get_y((*princess_pos)[best_i].xy));
    (**dlzka_cesty) += map_info[get_x((*princess_pos)[best_i].xy)][get_y((*princess_pos)[best_i].xy)].dist;
}

//*************************************************//
//          Funckie pre pracu s mapami             //
//*************************************************//
char **allocate_char_map(int height, int width)
{
    char **map = (char **)malloc(height * SIZE_P_CHAR);
    for (int i = 0; i < height; i++)
        map[i] = (char *)malloc(width * SIZE_CHAR);

    return map;
}

void free_char_map(char **mapa, int height)
{
    for (int i = 0; i < height; i++)
        free(mapa[i]);

    free(mapa);
}

//generovanie mapy (testovanie vyhladavacieho algoritmu)
char **generate_map(int height, int width, int princess_num)
{
    char **map = allocate_char_map(height, width);
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
    int teleport_num = 0;
    int generator_num = 0; //predtym 1 //zatial bez teleportov
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
                if ((i == 0) && (j == 0))
                {
                    element_index = get_random_range(0, 1);
                    map[i][j] = get_element(element_index);
                    elements_arr[element_index]--;
                    break;
                }
                propability_index = get_random_range(0, 29);
                element_index = propability_arr[propability_index];
                if (elements_arr[element_index])
                {
                    map[i][j] = get_element(element_index);
                    elements_arr[element_index]--;
                    break;
                }
            }
        }
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

int get_dist(char element)
{
    switch (element)
    {
        case 'C':
            return 1;

        case 'H':
            return 2;

        case 'N':
            return 0;

        case 'D':
            return 1;

        case 'P':
            return 1;

        default:
            return INT_MIN;
    }
}

//funkcia vytlaci mapu
void print_map(char **mapa, int height, int width)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            printf("%c ", mapa[i][j]);

        printf("\n");
    }
}

MAP_I **allocate_map_info(int height, int width)
{
    MAP_I **map = (MAP_I **)malloc(height * SIZE_P_MAP_I);
    for (int i = 0; i < height; i++)
        map[i] = (MAP_I *)malloc(width * SIZE_MAP_I);

    return map;
}

void free_map_info(MAP_I **map_info, int height)
{
    for (int i = 0; i < height; i++)
        free(map_info[i]);

    free(map_info);
}

MAP_I **set_map_info(int height, int width)
{
    MAP_I **map_info = allocate_map_info(height, width);
    return reset(map_info, height, width);
}

MAP_I **reset(MAP_I **map_info, int height, int width)
{
    for(int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            map_info[i][j].dist = UNRELAXED; //"nekonecna" vzdialenost od zaciatocneho bodu
            map_info[i][j].xy_p = 0;
        }
    return map_info;
}

void print_map_info(MAP_I **map_info, int height, int width, char ch)
{
    if (ch == 'd') //print distance
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
                if (map_info[i][j].dist == UNRELAXED)
                    printf("X ");
                else
                    printf("%d ", map_info[i][j].dist);

            printf("\n");
        }
    else //print position of field from which was actual field accessed
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)    
                printf("%d.%d | ", get_x(map_info[i][j].xy_p), get_y(map_info[i][j].xy_p));

            printf("\n");
        }
}

//funkcia vrati nahodne cisla z rozmedzia urceneho pouzivatelom
int get_random_range(int lower, int upper)
{
    return ((rand() % (upper + 1 - lower)) + lower);
}

//funckia vypocita hodnotu  percenta(perc) z cisla num
int get_percentage(int num, int percentage)
{
    double div = (double)percentage / 100;
    double result = (double)(div * num);
    return ceil(result); //zaokruhly nahor
}


//*************************************************//
//              Funckie - min heap                 //
//*************************************************//
void add_min_heap(MAP_I **map_info, POSITION **min_heap, POSITION relaxed, int *heap_size)
{
    (*min_heap)[*heap_size] = relaxed;
    (*heap_size)++;

    if ((*heap_size) == 1)
        return;

    int i = (*heap_size) - 1; //posledne cislo v heape - jeho index
    int parent_i = get_parent_index(i);

    int parent_x = get_x((*min_heap)[parent_i].xy);
    int parent_y = get_y((*min_heap)[parent_i].xy);
    int curr_x = get_x((*min_heap)[i].xy);
    int curr_y = get_y((*min_heap)[i].xy);

    while ((i != 0) && (map_info[parent_x][parent_y].dist > map_info[curr_x][curr_y].dist))
    {
        swap(&(*min_heap)[i], &(*min_heap)[parent_i]);
        i = get_parent_index(i);
        parent_i = get_parent_index(i);
        parent_x = get_x((*min_heap)[parent_i].xy);
        parent_y = get_y((*min_heap)[parent_i].xy);
        curr_x = get_x((*min_heap)[i].xy);
        curr_y = get_y((*min_heap)[i].xy);
    }
}

//funkcia vyhodi najmensie cislo z min heapu
POSITION pop(MAP_I **map_info, POSITION **min_heap, int *heap_size)
{
    if ((*heap_size) == 1)
    {
        (*heap_size)--;
        return (*min_heap)[0];
    }

    POSITION popped = (*min_heap)[0];
    (*min_heap)[0] = (*min_heap)[((*heap_size)-1)]; //posledne cislo dam na prve
    (*heap_size)--;
    min_heapify(map_info, &(*min_heap), *heap_size, 0);

    return popped;
}

void min_heapify(MAP_I **map_info, POSITION **min_heap, int heap_size, int i)
{
    if (heap_size <= 0)
        return;

    unsigned int l = left(i), r = right(i);
    unsigned int l_x = USHRT_MAX, l_y = USHRT_MAX, r_x = USHRT_MAX, r_y = USHRT_MAX;
    if (l < heap_size) {
        l_x = get_x((*min_heap)[l].xy);
        l_y = get_y((*min_heap)[l].xy);
    }
    if (r < heap_size)
    {
        r_x = get_x((*min_heap)[r].xy);
        r_y = get_y((*min_heap)[r].xy);
    }

    unsigned int curr_x = get_x((*min_heap)[i].xy), curr_y = get_y((*min_heap)[i].xy);
    unsigned int smallest = i;

    if ((l < heap_size) && (map_info[l_x][l_y].dist < map_info[curr_x][curr_y].dist))
        smallest = l;
    if ((r < heap_size) && (map_info[r_x][r_y].dist < map_info[curr_x][curr_y].dist))
        smallest = r;
    if (smallest != i)
    {
        swap(&(*min_heap)[i], &(*min_heap)[smallest]);
        min_heapify(map_info, min_heap, heap_size, smallest);
    }
}

void swap(POSITION *a, POSITION *b)
{
    POSITION temp = *a;
    *a = *b;
    *b = temp;
}

int get_parent_index(int num)
{
    return ((int)ceil((double)((double)num/(double)2)) - 1);
}

int left(int parent_i)
{
    return ((2*parent_i) + 1);
}

int right(int parent_i)
{
    return ((2*parent_i) + 2);
}

void print_min_heap(MAP_I **map_info, POSITION *min_heap, int heap_size)
{
    printf("MIN-HEAP\n");
    for (int i = 0; i < heap_size; i++)
    {
        printf("dist:%d ", map_info[get_x(min_heap[i].xy)][get_y(min_heap[i].xy)].dist);
        printf("x:%d y:%d\n", get_x(min_heap[i].xy), get_y(min_heap[i].xy));
    }
}
//*************************************************//

//*************************************************//
//         Funckie na binarne operacie             //
//*************************************************//
uint32_t change_state(uint32_t state, unsigned new_val, unsigned offset, unsigned bit_size)
{
    new_val <<= offset;
    unsigned reset_mask = ~(((1 << bit_size) - 1) << offset); // 3 - rozdiel medzi offsetmi
    state &= reset_mask; //nastavi na 0 tam, kde bude davat nove bity
    return (state | new_val);
}

unsigned get_x(uint32_t state)
{
    return (state >> X_OFF);
}

uint32_t change_x(uint32_t state, unsigned x)
{
    return change_state(state, x, X_OFF, POS_SIZE);
}

unsigned get_y(uint32_t state)
{
    return (state & (~(((1 << POS_SIZE) - 1) << X_OFF))) >> Y_OFF;
}

uint32_t change_y(uint32_t state, unsigned y)
{
    return change_state(state, y, Y_OFF, POS_SIZE);
}

// USHRT get_G(uint32_t state)
// {
//     return (state & (~(((1 << ((2*POS_SIZE)+1)) - 1) << (POS_SIZE+1)))) >> D_OFF;
// }

// uint32_t change_G(uint32_t state, USHRT G)
// {
//     return change_state(state, G, G_OFF, BIT_SIZE);
// }

unsigned get_D(uint32_t state)
{
    return (state & (~(((1 << ((2*POS_SIZE)+1)) - 1) << (POS_SIZE+1)))) >> D_OFF;
}

uint32_t change_D(uint32_t state, unsigned D)
{
    return change_state(state, D, D_OFF, BIT_SIZE);
}

unsigned get_P(uint32_t state, unsigned num)
{
    num++;
    printf("get P... num %d\n", num);
    int p_pos = (2 * POS_SIZE) + (num);
    switch (num)
    {
        case 1:
            return (state & (~(((1 << p_pos) - 1) << (POS_SIZE + num + 1)))) >> P1_OFF;

        case 2:
            return (state & (~(((1 << p_pos) - 1) << (POS_SIZE + num + 1)))) >> P2_OFF;

        case 3:
            return (state & (~(((1 << p_pos) - 1) << (POS_SIZE + num + 1)))) >> P3_OFF;

        case 4:
            return (state & (~(((1 << p_pos) - 1) << (POS_SIZE + num + 1)))) >> P4_OFF;

        case 5:
            return (state & (~(((1 << p_pos) - 1) << (POS_SIZE + num + 1)))) >> P5_OFF;
        default:
            return 0;
    }
}

uint32_t change_P(uint32_t state, unsigned num, unsigned P)
{
    num++;
    printf("chagne P... num %d\n", num);
    switch (num)
    {
        case 1:
            return change_state(state, P, P1_OFF, BIT_SIZE);

        case 2:
            return change_state(state, P, P2_OFF, BIT_SIZE);

        case 3:
            return change_state(state, P, P3_OFF, BIT_SIZE);

        case 4:
            return change_state(state, P, P4_OFF, BIT_SIZE);

        case 5:
            return change_state(state, P, P5_OFF, BIT_SIZE);

        default:
            return 0;
    }
}

//iba pre kontrolu - vypise cislo v 2 sustave (32 bitov)
unsigned print_bin32(uint32_t num, int bit)
{
    if (bit >= 8 * sizeof(num))
        return USHRT_MAX;

    print_bin32(num/2, bit+1);
    printf("%d", num%2);

    if (bit % 8 == 0)
        printf(" ");

    if (bit == 0)
        printf("\n");
}