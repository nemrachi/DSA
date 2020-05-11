#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

//struktura pre mapu, ktora uchovava informacie o jednotlivych poliach
typedef struct MAP_I {
    uint32_t dist; //dlzka od zaciatocneho bodu
    uint32_t xy_p; //suradnice pola, z akeho sme pristupovali do aktualeho pola
} MAP_I;

//struktura pre poziciu
typedef struct POSITION {
    uint32_t xy;
} POSITION;

//        |x
//        |
// y______|________ y
//        |
//        |x
//
//x a y su suradnice polohy na mape

//konstatny pre pracu so stavom Popolvara (pre bitove operacie)
#define POS_SIZE 13 //pocet bitov pre x, resp. y
#define BIT_SIZE 1
#define X_OFF 19
#define Y_OFF 6
#define D_OFF 5
#define P1_OFF 4
#define P2_OFF 3
#define P3_OFF 2
#define P4_OFF 1
#define P5_OFF 0

#define SIZE_INT sizeof(int)
#define SIZE_CHAR sizeof(char)
#define SIZE_P_CHAR sizeof(char *)
#define SIZE_MAP_I sizeof(MAP_I)
#define SIZE_P_MAP_I sizeof(MAP_I *)
#define SIZE_POSITION sizeof(POSITION)

#define WIDTH 100 //m (sirka mapy)
#define HEIGHT 100 //n (vyska mapy)
#define TIME_TO_KILL 100
#define UNRELAXED 4294967 //"nekonecno"
#define PRINCESS_NUM 5

//*************************************************//
//                Hlavne funkcie                   //
//*************************************************//
int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty);
MAP_I **dijkstra(char **mapa, MAP_I **map_info, uint32_t state, int height, int width, int time);
void change_xy_POS(POSITION *neighbour, int new_x, int new_y);
void change_map_info(MAP_I ***map_info, int new_x, int new_y, int x, int y, unsigned actual_dist, unsigned neighbour_dist);
void find_positions(char **mapa, int height, int width, POSITION *dragon_pos, POSITION **princess_pos, int *princess_counter);
void set_path(POSITION **path, int size);
void get_path(POSITION **path, int *path_size, MAP_I **map_info, POSITION last_pos);
void reverse_path(POSITION **path, int start, int end);
void print_path(POSITION *path);
void swap_x_y_path(POSITION **path);
int *convert_position_arr_to_int(POSITION *shortest_path, int *path_size);
uint32_t shortest_princess_path(MAP_I **map_info, uint32_t state, POSITION **princess_pos, int princess_counter);
//*************************************************//
//              Vedlajsie funckie                  //
//*************************************************//
int get_random_range(int lower, int upper);
double rand_between_0_1();
// int factorial(int num);
// void swap_int(int *a, int *b);
// void princess_permutation(int *arr, int start, int end, FILE **fp);
//*************************************************//
//          Funckie pre pracu s mapami             //
//*************************************************//
char **allocate_char_map(int height, int width);
void free_char_map(char **mapa, int height);
char **generate_map(int height, int width, int princess_num);
int get_dist(char element);
void print_map(char **mapa, int height, int width, int time);
MAP_I **allocate_map_info(int height, int width);
void free_map_info(MAP_I **map_info, int height);
MAP_I **set_map_info(int height, int width);
MAP_I **reset(MAP_I **map_info, int height, int width);
void print_map_info(MAP_I **map_info, int height, int width, char ch);
int get_obstacles_num(char **mapa, int height, int width);
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
unsigned get_d(uint32_t state);
uint32_t change_d(uint32_t state, unsigned D);
unsigned get_p(uint32_t state, unsigned num);
uint32_t change_p(uint32_t state, unsigned num, unsigned P);
unsigned print_bin32(uint32_t num, int bit);
//*************************************************//
//                   Testovanie                    //
//*************************************************//
void main_zo_zadania();
void main_zo_zadania_case_2(int *height, int *width, int *time_to_kill, char ***map);
void test_bit_operations();
void test_find_positions();
void test_print_structure_size();

//*************************************************//
//                      MAIN                       //
//*************************************************//
int main() {
    main_zo_zadania();
    //test_bit_operations();
    //test_find_positions();
    //test_print_structure_size();
    return 0;
}
//*************************************************//

//*************************************************//
//                Hlavne funkcie                   //
//*************************************************//
//m (sirka mapy), n (vyska mapy)
int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty) {
    int path_size_init = n * m, princess_path_size = 0;
    (*dlzka_cesty) = 0;
    POSITION *cesta = (POSITION *)malloc(SIZE_POSITION * path_size_init); //vysledna cesta
    POSITION p_pos;
    set_path(&cesta, path_size_init);
    //druha mapa, ktora udrziava extra informacie* o kazdom poli v povodnej mape
    //*extra informacie: vzdialenost od zaciatocneho bodu a
    //suradnice predosleho policka, z ktoreho sme sa dostali na aktualne
    MAP_I **map_info = set_map_info(n, m);
    //ulozenie x,y zaciatku cesty a stavy, ci je drak zabity a princezne zachranene
    //       x             y       D P P P P P
    //|0000000000000|0000000000000|0|0|0|0|0|0 => state
    uint32_t state = 0;
    //ulozene dolezite pozicie v mape
    POSITION dragon_pos, *princess_pos = (POSITION *)malloc(5 * SIZE_POSITION);
    int princess_counter = 0; //pocet princezien

    find_positions(mapa, n, m, &dragon_pos, &princess_pos, &princess_counter);

    if (mapa[0][0] == 'N') {
        printf("\nPopolvar sa nevie dostat zo skaly dole.\n");
        return NULL; //ak popolvar zacina na prekazke, konci
    }

    //zabi draka
    map_info = dijkstra(mapa, map_info, state, n, m, t); 

    if (map_info[get_x(dragon_pos.xy)][get_y(dragon_pos.xy)].dist == UNRELAXED) {
        printf("\nPopolvar sa nevie dostat ku drakovi.\n");
        return NULL; //ak sa Popolvar nedostal k drakovi 
    }
    if (t < map_info[get_x(dragon_pos.xy)][get_y(dragon_pos.xy)].dist) {
        printf("\nDrak zjedol princezne. Smola.\n");
        return NULL; //ak Popolvar nestihol zabit draka v danom case
    }    

    //drak zabity
    cesta[(*dlzka_cesty)++].xy = 0; //prva pozicia cesty
    get_path(&cesta, dlzka_cesty, map_info, dragon_pos);

    state = change_d(state, 1); //zapise 1 na bit draka =>drak zabity
    //do stavu si zapise poziciu draka, od ktorej nasledujuca dijkstra bude zacinat
    state = change_x(state, get_x(dragon_pos.xy));
    state = change_y(state, get_y(dragon_pos.xy));

    //zachran princeze
    printf("Princene\n");
    for (int i = princess_counter; i > 0; i--) {
        map_info = reset(map_info, n, m); //resetne informacnu mapu
        map_info = dijkstra(mapa, map_info, state, n, m, t);
        for (int j = 0; j < princess_counter; j++) {
            if (map_info[get_x(princess_pos[j].xy)][get_y(princess_pos[j].xy)].dist == UNRELAXED){
                printf("\nPopolvar sa nevedel dostat k princeznej.\n");
                (*dlzka_cesty) = 0;
                return NULL; //ak jedna princezna bola zatarasena, Popolvar je neuspesny
            }
        }
        state = shortest_princess_path(map_info, state, &princess_pos, princess_counter);
        p_pos.xy = change_x(p_pos.xy, get_x(state));
        p_pos.xy = change_y(p_pos.xy, get_y(state));
        get_path(&cesta, dlzka_cesty, map_info, p_pos);
    }

    free_map_info(map_info, n);
    free_char_map(mapa, n);
    free(princess_pos);

    swap_x_y_path(&cesta);
    return convert_position_arr_to_int(cesta, dlzka_cesty);
}

MAP_I **dijkstra(char **mapa, MAP_I **map_info, uint32_t state, int height, int width, int time) {
    POSITION *min_heap = (POSITION *)malloc(SIZE_POSITION * height * width);
    int heap_size = 0, obstacles_num = get_obstacles_num(mapa, height, width);

    POSITION neighbour /*sused pola*/, chosen_one;
    int x, y;
    int count = 0, time_count = 0;

    //zaciatocna pozicia je ulozena v premennej state
    x = get_x(state);
    y = get_y(state);
    map_info[x][y].dist = 0;

    while (1) {
        count++;

        if (((x - 1) >= 0) && ((x - 1) < height)) {
            //look up
            if (((count == 1) || ((x - 1) != get_x(map_info[x][y].xy_p)))
                && (mapa[x-1][y] != 'N') &&
                (map_info[x-1][y].dist > (map_info[x][y].dist + get_dist(mapa[x-1][y])))) {
                change_xy_POS(&neighbour, x - 1, y);
                change_map_info(&map_info, x - 1, y, x, y, map_info[x][y].dist, get_dist(mapa[x-1][y]));
                add_min_heap(map_info, &min_heap, neighbour, &heap_size);
            }
        }

        if (((y + 1) < width) && ((y + 1) >= 0)) {
            //look right
            if (((count == 1) || ((y + 1) != get_y(map_info[x][y].xy_p)))
                && (mapa[x][(y+1)] != 'N') &&
                (map_info[x][(y+1)].dist > (map_info[x][y].dist + get_dist(mapa[x][y+1])))) {
                change_xy_POS(&neighbour, x, y + 1);
                change_map_info(&map_info, x, y + 1, x, y, map_info[x][y].dist, get_dist(mapa[x][y+1]));             
                add_min_heap(map_info, &min_heap, neighbour, &heap_size);
            }
        }
        
        if (((x + 1) < height) && ((x + 1) >= 0)) {
            //look down
            if (((count == 1) || ((x + 1) != get_x(map_info[x][y].xy_p)))
                && (mapa[x+1][y] != 'N') &&
                (map_info[(x+1)][y].dist > (map_info[x][y].dist + get_dist(mapa[x+1][y])))) {
                change_xy_POS(&neighbour, x + 1, y);
                change_map_info(&map_info, x + 1, y, x, y, map_info[x][y].dist, get_dist(mapa[x+1][y]));
                add_min_heap(map_info, &min_heap, neighbour, &heap_size);
            }
        }

        if (((y - 1) >= 0) && ((y - 1) < width)) {
            //look left
            if (((count == 1) || ((y - 1) != get_y(map_info[x][y].xy_p)))
                && (mapa[x][y-1] != 'N') &&
                (map_info[x][(y-1)].dist > (map_info[x][y].dist + get_dist(mapa[x][y-1])))) {
                change_xy_POS(&neighbour, x, y - 1);
                change_map_info(&map_info, x, y - 1, x, y, map_info[x][y].dist, get_dist(mapa[x][y-1]));
                add_min_heap(map_info, &min_heap, neighbour, &heap_size);
            }
        }

        chosen_one = pop(map_info, &min_heap, &heap_size); //vyberie prve pole s min-heapu
        x = get_x(chosen_one.xy);
        y = get_y(chosen_one.xy);

        if((heap_size <= 0) && (count >= ((height * width) - obstacles_num)))
            break;
    }

    free(min_heap);
    return map_info;
}

void change_xy_POS(POSITION *neighbour, int new_x, int new_y) {
    (*neighbour).xy = change_x((*neighbour).xy, new_x);
    (*neighbour).xy = change_y((*neighbour).xy, new_y);
}

void change_map_info(MAP_I ***map_info, int new_x, int new_y, int x, int y, unsigned actual_dist, unsigned neighbour_dist) {
    (*map_info)[new_x][new_y].xy_p = change_x((*map_info)[new_x][new_y].xy_p, x);
    (*map_info)[new_x][new_y].xy_p = change_y((*map_info)[new_x][new_y].xy_p, y);
    (*map_info)[new_x][new_y].dist = actual_dist + neighbour_dist;
}

//funkcia si ulozi pozicie draka a princezien z mapy
void find_positions(char **mapa, int height, int width, POSITION *dragon_pos, POSITION **princess_pos, int *princess_counter) {
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if(mapa[i][j] == 'D') { //ulozi si poziciu draka
                (*dragon_pos).xy = change_x((*dragon_pos).xy, i);
                (*dragon_pos).xy = change_y((*dragon_pos).xy, j);
            }
            else if (mapa[i][j] == 'P') { //ulozi si poziciu princezien, aj ich spocita
                (*princess_pos)[(*princess_counter)].xy = change_x((*princess_pos)[(*princess_counter)].xy, i);
                (*princess_pos)[(*princess_counter)].xy = change_y((*princess_pos)[(*princess_counter)].xy, j);
                (*princess_counter)++;
            }
}

//funkcia vsade v poli nastavi hodnotu na INT_MAX
void set_path(POSITION **path, int size) {
    for (int i = 0; i < size; i++)
        (*path)[i].xy = INT_MAX;
}

//funkcia ziska cestu od poslednej pozicie (last_pos) k policku, kde je vzdialenost 0
void get_path(POSITION **path, int *path_size, MAP_I **map_info, POSITION last_pos) {
    int tmp_x, tmp_y, x = get_x(last_pos.xy), y = get_y(last_pos.xy), i = (*path_size), start_i = i;
    while (map_info[x][y].dist != 0) {
        (*path)[i].xy = change_x((*path)[i].xy, x);
        (*path)[i].xy = change_y((*path)[i].xy, y);
        i++;
        tmp_x = get_x(map_info[x][y].xy_p);
        tmp_y = get_y(map_info[x][y].xy_p);
        x = tmp_x;
        y = tmp_y;
    }
    (*path_size) = i;
    reverse_path(path, start_i, (*path_size));
}

//funkcia obrati cestu - z prvych pozicii da data na posledne
void reverse_path(POSITION **path, int start, int end) {
    uint32_t temp;
    while (start < end) { 
        temp = (*path)[start].xy;  
        end--; 
        (*path)[start].xy = (*path)[end].xy; 
        (*path)[end].xy = temp; 
        start++; 
    }
}

void print_path(POSITION *path) {
    printf("x y\n");
    int i = 0;
    while ((path)[i].xy != INT_MAX) {
        printf("%d %d\n", get_x((path)[i].xy), get_y((path)[i].xy));
        i++; 
    }
}

//funkcia vymeni x-ove a y-ove hodnoty
void swap_x_y_path(POSITION **path) {
    unsigned int temp;
    int i = 0;
    while ((*path)[i].xy != INT_MAX) {
        temp = get_x((*path)[i].xy);
        (*path)[i].xy = change_x((*path)[i].xy, get_y((*path)[i].xy));
        (*path)[i].xy = change_y((*path)[i].xy, temp);
        i++; 
    }
}

//funkcia vrati cestu v int array-i
int *convert_position_arr_to_int(POSITION *shortest_path, int *path_size) {
    int j = 0;
    int *shortest_path_int = (int *)malloc(SIZE_INT * (*path_size) * 2);
    for (int i = 0; i < (*path_size); i++) {
        shortest_path_int[j++] = (int) get_x(shortest_path[i].xy);
        shortest_path_int[j++] = (int) get_y(shortest_path[i].xy);
    }
    free(shortest_path);
    return shortest_path_int;    
}

//funkcia najde najblizsiu princeznu od pozicie, ktora je uvedena v state
uint32_t shortest_princess_path(MAP_I **map_info, uint32_t state, POSITION **princess_pos, int princess_counter) {
    POSITION temp, best;
    int best_i = 0, i;
    for (i = 0; i < princess_counter; i++) {
        if (get_p(state, i) == (unsigned) 0) {
            //ak este princezna nemala zaznam v state, nebola este zachranena
            best = (*princess_pos)[i];
            best_i = i;
            break;
        }
    }
    for (i = 0; i < princess_counter; i++) {
        temp = (*princess_pos)[i];
        if ((map_info[get_x(temp.xy)][get_y(temp.xy)].dist < map_info[get_x(best.xy)][get_y(best.xy)].dist) && (get_p(state, i) == (unsigned) 0)) {
            best = temp;
            best_i = i;
        }
    }
    state = change_p(state, best_i, 1);
    //zapise jej polohu do stavu a od nej bude zacinat dalsia dijskra
    state = change_x(state, get_x((*princess_pos)[best_i].xy));
    state = change_y(state, get_y((*princess_pos)[best_i].xy));
    return state;
}
//*************************************************//

//*************************************************//
//              Vedlajsie funckie                  //
//*************************************************//
//funkcia vrati nahodne cisla z rozmedzia urceneho pouzivatelom (vratane tych hodnot)
int get_random_range(int lower, int upper) {
    return ((rand() % (upper + 1 - lower)) + lower);
}

//funkcia vrati nahodne cisla z rozmedzia 0 a 1
double rand_between_0_1() {
    return ((double)rand() / (double)RAND_MAX);
}
// int factorial(int num) {
//     if (num <= 1)
//         return 1;
//     int k = 1;
//     for (int i = 2; i <= num; i++)
//         k *= i;
//     return k;
// }
// void swap_int(int *a, int *b) {
//     int temp = (*a);
//     (*a) = (*b);
//     (*b) = temp;
// }
// void princess_permutation(int *arr, int start, int end, FILE **fp) {
//     if (start == end) {
//         char str[5];
//         for (int i = 0 ; i < end; ++i) {
//             str[i] = arr[i] + '0';
//             if (isdigit(str[i]) == 0)
//                 return;
//         }
//         fprintf((*fp), "%s\n", str);
//         return;
//     }
//     for(int i = start; i <= end; i++) {
//         swap_int((arr+i), (arr+start));
//         princess_permutation(arr, start+1, end, fp);
//         swap_int((arr+i), (arr+start));
//     }
// }
//*************************************************// 

//*************************************************//
//          Funckie pre pracu s mapami             //
//*************************************************//
char **allocate_char_map(int height, int width) {
    char **map = (char **)malloc(height * SIZE_P_CHAR);
    for (int i = 0; i < height; i++)
        map[i] = (char *)malloc(width * SIZE_CHAR);

    return map;
}

void free_char_map(char **mapa, int height) {
    for (int i = 0; i < height; i++)
        free(mapa[i]);

    free(mapa);
}

//generovanie mapy
char **generate_map(int height, int width, int princess_num) {
    srand(time(0)); //kvoli generovaniu nahodnych cisel pri generovani mapy
    char **map = allocate_char_map(height, width);
    double r;
    // Aby mapa nebola príliš nepriechodná, pomocou funkcie rand 
    // viete pre každé políčko určiť pravdepodobnosť výberu typu 
    // políčka (napr.: 0<=n<=0.05<c<=0.6<h<=1)

    //vkladanie elementov cesty, hustia a prekazky do mapy
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            r = rand_between_0_1();
            if (r <= 0.05) {
                map[i][j] = 'N';
            } else if (r <= 0.6) {
                map[i][j] = 'C';
            } else {
                map[i][j] = 'H';
            }          
        }
    }
    //vlozenie draka do mapy
    int h = get_random_range(0, height - 1);
    int w = get_random_range(0, width - 1);
    map[h][w] = 'D';
    //vlozenie princezien
    while (princess_num > 0) {
        h = get_random_range(0, height - 1);
        w = get_random_range(0, width - 1);
        if ((map[h][w] == 'D') || (map[h][w] == 'P')) {
            continue;
        }
        map[h][w] = 'P';
        princess_num--;
    }
    return map;
}

//funckia vrati hodnotu casu podla hodnoty pola
int get_dist(char element) {
    switch (element) {
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
void print_map(char **mapa, int height, int width, int time) {
    printf("%d %d %d\n", height, width, time);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            printf("%c", mapa[i][j]);

        printf("\n");
    }
}

MAP_I **allocate_map_info(int height, int width) {
    MAP_I **map = (MAP_I **)malloc(height * SIZE_P_MAP_I);
    for (int i = 0; i < height; i++)
        map[i] = (MAP_I *)malloc(width * SIZE_MAP_I);
    return map;
}

void free_map_info(MAP_I **map_info, int height) {
    for (int i = 0; i < height; i++)
        free(map_info[i]);

    free(map_info);
}

MAP_I **set_map_info(int height, int width) {
    MAP_I **map_info = allocate_map_info(height, width);
    return reset(map_info, height, width);
}

MAP_I **reset(MAP_I **map_info, int height, int width) {
    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            map_info[i][j].dist = UNRELAXED; //"nekonecna" vzdialenost od zaciatocneho bodu
            map_info[i][j].xy_p = 0;
        }
    }
    return map_info;
}

void print_map_info(MAP_I **map_info, int height, int width, char ch) {
    if (ch == 'd') //print distance
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++)
                if (map_info[i][j].dist == UNRELAXED)
                    printf("X ");
                else
                    printf("%d ", map_info[i][j].dist);

            printf("\n");
        }
    else //print position of field from which was actual field accessed
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++)    
                printf("%d.%d | ", get_x(map_info[i][j].xy_p), get_y(map_info[i][j].xy_p));

            printf("\n");
        }
}

//funkcia ziska pocet prekazok na mape
int get_obstacles_num(char **mapa, int height, int width) {
    int count = 0;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (mapa[i][j] == 'N')
                count++;
    return count;
}
//*************************************************//

//*************************************************//
//              Funckie - min heap                 //
//*************************************************//
//funkcia prida poziciu pola do min heapu
void add_min_heap(MAP_I **map_info, POSITION **min_heap, POSITION relaxed, int *heap_size) {
    (*min_heap)[*heap_size] = relaxed; //nove data da na koniec min heapu
    (*heap_size)++;

    if (((*heap_size) <= 1))
        return;

    int i = (*heap_size) - 1; //posledne cislo v heape - jeho index
    int parent_i = get_parent_index(i); //index rodica indexu i

    int parent_x = get_x((*min_heap)[parent_i].xy);
    int parent_y = get_y((*min_heap)[parent_i].xy);
    int curr_x = get_x((*min_heap)[i].xy);
    int curr_y = get_y((*min_heap)[i].xy);

    while ((i != 0) && (map_info[parent_x][parent_y].dist > map_info[curr_x][curr_y].dist)) {
        //vymiena pokial rodic nebude mat mensiu hodnotu ako dieta
        swap(&(*min_heap)[i], &(*min_heap)[parent_i]); //vymena rodica s dietatom
        i = get_parent_index(i); //ziska noveho rodica
        parent_i = get_parent_index(i);
        parent_x = get_x((*min_heap)[parent_i].xy);
        parent_y = get_y((*min_heap)[parent_i].xy);
        curr_x = get_x((*min_heap)[i].xy);
        curr_y = get_y((*min_heap)[i].xy);
    }
}

//funkcia vyhodi najmensie cislo z min heapu (teda prve v min heape)
POSITION pop(MAP_I **map_info, POSITION **min_heap, int *heap_size) {
    if ((*heap_size) == 1) {
        (*heap_size)--;
        return (*min_heap)[0];
    } else if ((*heap_size) == 0) {
        POSITION pos;
        pos.xy = 0;
        return pos;
    }
    POSITION popped = (*min_heap)[0];
    (*min_heap)[0] = (*min_heap)[((*heap_size)-1)]; //posledne cislo dam na prve
    (*heap_size)--;
    min_heapify(map_info, &(*min_heap), *heap_size, 0); //usporiadam min heap
    return popped;
}

//funkcia usporiadava min heap
void min_heapify(MAP_I **map_info, POSITION **min_heap, int heap_size, int i) {
    if (heap_size <= 0)
        return;

    unsigned int l = left(i), r = right(i); //indexy praveho a laveho dietata rodica i
    unsigned int l_x = USHRT_MAX, l_y = USHRT_MAX, r_x = USHRT_MAX, r_y = USHRT_MAX;
    if (l < heap_size) {
        l_x = get_x((*min_heap)[l].xy);
        l_y = get_y((*min_heap)[l].xy);
    }
    if (r < heap_size) {
        r_x = get_x((*min_heap)[r].xy);
        r_y = get_y((*min_heap)[r].xy);
    }

    unsigned int curr_x = get_x((*min_heap)[i].xy), curr_y = get_y((*min_heap)[i].xy);
    unsigned int smallest = i;

    if ((l < heap_size) && (map_info[l_x][l_y].dist < map_info[curr_x][curr_y].dist))
        smallest = l; //ak je lave dieta mensie, ulozime si ho
    if ((r < heap_size) && (map_info[r_x][r_y].dist < map_info[curr_x][curr_y].dist))
        smallest = r; //ak je prave dieta mensie, ulozime si ho
    if (smallest != i) { //ak mame ulozene nejake dieta, mozme ho swapovat s rodicom i
        swap(&(*min_heap)[i], &(*min_heap)[smallest]);
        min_heapify(map_info, min_heap, heap_size, smallest);
    }
}

void swap(POSITION *a, POSITION *b) {
    POSITION temp = *a;
    *a = *b;
    *b = temp;
}

int get_parent_index(int num) {
    return ((int)ceil((double)((double)num/(double)2)) - 1);
}

int left(int parent_i) {
    return ((2*parent_i) + 1);
}

int right(int parent_i) {
    return ((2*parent_i) + 2);
}

void print_min_heap(MAP_I **map_info, POSITION *min_heap, int heap_size) {
    printf("MIN-HEAP\n");
    for (int i = 0; i < heap_size; i++) {
        printf("dist:%d ", map_info[get_x(min_heap[i].xy)][get_y(min_heap[i].xy)].dist);
        printf("x:%d y:%d\n", get_x(min_heap[i].xy), get_y(min_heap[i].xy));
    }
}
//*************************************************//

//*************************************************//
//         Funckie na binarne operacie             //
//*************************************************//
uint32_t change_state(uint32_t state, unsigned new_val, unsigned offset, unsigned bit_size) {
    new_val <<= offset;
    unsigned reset_mask = ~(((1 << bit_size) - 1) << offset); // 3 - rozdiel medzi offsetmi
    state &= reset_mask; //nastavi na 0 tam, kde bude davat nove bity
    return (state | new_val);
}

unsigned get_x(uint32_t state) {
    return (state >> X_OFF);
}

uint32_t change_x(uint32_t state, unsigned x) {
    return change_state(state, x, X_OFF, POS_SIZE);
}

unsigned get_y(uint32_t state) {
    return (state & (~(((1 << POS_SIZE) - 1) << X_OFF))) >> Y_OFF;
}

uint32_t change_y(uint32_t state, unsigned y) {
    return change_state(state, y, Y_OFF, POS_SIZE);
}

unsigned get_d(uint32_t state) {
    int pos = (2 * POS_SIZE) + 1;
    return (state & (~(((1 << pos) - 1) << Y_OFF))) >> D_OFF;
}

uint32_t change_d(uint32_t state, unsigned D) {
    return change_state(state, D, D_OFF, BIT_SIZE);
}

unsigned get_p(uint32_t state, unsigned num_of_p) {
    num_of_p++;
    int p_pos = (2 * POS_SIZE) + num_of_p + 1;
    switch (num_of_p) {
        case 1:
            return (state & (~(((1 << p_pos) - 1) << D_OFF))) >> P1_OFF;

        case 2:
            return (state & (~(((1 << p_pos) - 1) << P1_OFF))) >> P2_OFF;

        case 3:
            return (state & (~(((1 << p_pos) - 1) << P2_OFF))) >> P3_OFF;

        case 4:
            return (state & (~(((1 << p_pos) - 1) << P3_OFF))) >> P4_OFF;

        case 5:
            return (state & ((1 << 1) - 1));
        default:
            return 0;
    }
}

uint32_t change_p(uint32_t state, unsigned num_of_p, unsigned P) {
    num_of_p++;
    switch (num_of_p) {
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
unsigned print_bin32(uint32_t num, int bit) {
    if (bit >= 8 * sizeof(num))
        return USHRT_MAX;

    print_bin32(num/2, bit+1);
    printf("%d", num%2);

    if (bit % 8 == 0)
        printf(" ");

    if (bit == 0)
        printf("\n");
}
//*************************************************//

//*************************************************//
//                   Testovanie                    //
//*************************************************//
void main_zo_zadania() {
    char **map;
    int test, dlzka_cesty, cas_popolvara, *cesta;
    int height, width, time_to_kill, run = 1;
    FILE* file; // subor s mapou

    while (run) {
        dlzka_cesty = 0, height = 0, width = 0, time_to_kill = 0;
        printf("\nZadajte cislo:\n0 : koniec programu\n1 : mapa zo suboru mapa.txt\n"
        "2 : testovacie predefinovane mapy\n3 : vygenerovana nahodna mapa\n");
        scanf("%d", &test);
        printf("\n");

        switch (test) {
        case 0:
            run = 0;
            break;

        case 1:
            file = fopen("mapa.txt", "r");
            if (file) {
                fscanf(file, "%d %d %d", &height, &width, &time_to_kill);
            } else {
                printf("Subor nebol najdeny alebo ho nemozno otvorit.\n");
                continue;
            }
            char field;
            map = allocate_char_map(height, width);
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    field = fgetc(file);
                    if (field == '\n')
                        field = fgetc(file);
                    map[i][j] = field; 
                }   
            }
            fclose(file);
            break;

        case 2:
            main_zo_zadania_case_2(&height, &width, &time_to_kill, &map);
            break;
            
        case 3:
            height = HEIGHT;
            width = WIDTH;
            time_to_kill = TIME_TO_KILL;
            map = generate_map(HEIGHT, WIDTH, PRINCESS_NUM); //generovanie nahodnej mapy 
            break;

        default:
            run = 0;
            break;
        }

        if (!run) {
            break;
        }

        print_map(map, height, width, time_to_kill);

        //*************zachrana*************
        int *cesta = zachran_princezne(map, height, width, time_to_kill, &dlzka_cesty);
        
        if (dlzka_cesty) {
            printf("\nVysledna cesta:\n");
            for (int i = 0; i < dlzka_cesty; ++i)
                printf("%d %d\n", cesta[i*2], cesta[i*2+1]);
            printf("Dlzka cesty: %d\n", dlzka_cesty);
        } else {
            printf("\nCesta nebola najdena.\n");
            printf("Dlzka cesty: %d\n", dlzka_cesty);
        }
    }
}

void main_zo_zadania_case_2(int *height, int *width, int *time_to_kill, char ***map) {
    //testovacie mapy - hranicne situacie
    int map_num, run = 1;
    printf("Zadajte cislo mapy:\n0 : mapa zo zadania\n1 : mapa so zatarasenym Popolvarom"
    "\n2 : mapa so zatarasenou 1. najblizsou princeznou\n3 : mapa so zatarasenou inou princeznou"
    "\n4 : mapa, kde sa Popolvar musi rozhodnut pre 2. najblizsiu princeznu"
    "\n5 : mapa, kde najkratsia cesta vedie cez prejdenie viacero poli ako dlhsia"
    "\n6 : mapa s prekazkou na pozicii [0, 0]\n7 : mapa, kedy Popolvar nestihne zabit draka\n");
    scanf("%d", &map_num);
    printf("\n");
    while (run) {
        switch (map_num) {
        case 0:
            (*height) = 10;
            (*width) = 10;
            (*time_to_kill) = 12;
            (*map) = (char **)malloc((*height) * SIZE_P_CHAR);
            (*map)[0]="CCHCNHCCHN";
            (*map)[1]="NNCCCHHCCC";
            (*map)[2]="DNCCNNHHHC";
            (*map)[3]="CHHHCCCCCC";
            (*map)[4]="CCCCCNHHHH";
            (*map)[5]="PCHCCCNNNN";
            (*map)[6]="NNNNNHCCCC";
            (*map)[7]="CCCCCPCCCC";
            (*map)[8]="CCCNNHHHHH";
            (*map)[9]="HHHPCCCCCC";
            run = 0;
            break;

        case 1:
            (*height) = 4;
            (*width) = 6;
            (*time_to_kill) = 10;
            (*map) = (char **)malloc((*height) * SIZE_P_CHAR);
            (*map)[0]="CCCNHC";
            (*map)[1]="CCHNNC";
            (*map)[2]="CCCNCH";
            (*map)[3]="PCNHCD";
            run = 0;
            break;

        case 2:
            (*height) = 10;
            (*width) = 10;
            (*time_to_kill) = 12;
            (*map) = (char **)malloc((*height) * SIZE_P_CHAR);
            (*map)[0]="CCHCNHCCHN";
            (*map)[1]="NNCCCHHCCP";
            (*map)[2]="DNCCNNHHHC";
            (*map)[3]="CHHHCCCCCC";
            (*map)[4]="NNCCCNHHHH";
            (*map)[5]="PCNCCCNNNN";
            (*map)[6]="NNNNNHCCCC";
            (*map)[7]="CCCCCPCCCC";
            (*map)[8]="CCCNNHHHHH";
            (*map)[9]="HHHPCCCCCC";
            run = 0;
            break;

        case 3:
            (*height) = 10;
            (*width) = 10;
            (*time_to_kill) = 12;
            (*map) = (char **)malloc((*height) * SIZE_P_CHAR);
            (*map)[0]="CCHCNHCCHN";
            (*map)[1]="NNCCCHHCCC";
            (*map)[2]="DNCCNNHHHC";
            (*map)[3]="CHHHCCCCCC";
            (*map)[4]="CCCCCNHHHH";
            (*map)[5]="PCHCCCNNNN";
            (*map)[6]="NNNNNHCCCC";
            (*map)[7]="CCCNPNCCCC";
            (*map)[8]="CCCNNHHHHH";
            (*map)[9]="HHHPCCCCCC";
            run = 0;
            break;

        case 4:
            (*height) = 5;
            (*width) = 4;
            (*time_to_kill) = 10;
            (*map) = (char **)malloc((*height) * SIZE_P_CHAR);
            (*map)[0]="CCPC";
            (*map)[1]="CCCC";
            (*map)[2]="DCPC";
            (*map)[3]="CCCC";
            (*map)[4]="PCCC";
            run = 0;
            break;

        case 5:
            (*height) = 4;
            (*width) = 6;
            (*time_to_kill) = 15;
            (*map) = (char **)malloc((*height) * SIZE_P_CHAR);
            (*map)[0]="CHHHCD";
            (*map)[1]="CCCCCC";
            (*map)[2]="CHHHCC";
            (*map)[3]="PNHHCP";
            run = 0;
            break;
        
        case 6:
            (*height) = 4;
            (*width) = 6;
            (*time_to_kill) = 15;
            (*map) = (char **)malloc((*height) * SIZE_P_CHAR);
            (*map)[0]="NHHHCD";
            (*map)[1]="CCCCCC";
            (*map)[2]="CHHHCC";
            (*map)[3]="PNHHCP";
            run = 0;
            break;

        case 7:
            (*height) = 4;
            (*width) = 6;
            (*time_to_kill) = 1;
            (*map) = (char **)malloc((*height) * SIZE_P_CHAR);
            (*map)[0]="CCCNHC";
            (*map)[1]="CCHNNC";
            (*map)[2]="CCCCCH";
            (*map)[3]="PCNHCD";
            run = 0;
            break;
        
        default:
            continue;
        }
        if (!run)
            break;
    }
}

void test_bit_operations() {
    POSITION state;
    state.xy = 0;
    int x = 5, y = 1, d = 1;
    int p1 = 1, p2 = 0, p3 = 0, p4 = 1, p5 = 0;
    printf("x:%d, y:%d, d:%d, p1:%d, p2:%d, p3:%d, p4:%d, p5:%d\n", x, y, d, p1, p2, p3, p4, p5);
    state.xy = change_d(state.xy, 1);
    state.xy = change_x(state.xy, x++);
    state.xy = change_y(state.xy, y++);
    state.xy = change_p(state.xy, 0, p1);
    state.xy = change_p(state.xy, 1, p2);
    state.xy = change_p(state.xy, 2, p3);
    state.xy = change_p(state.xy, 3, p4);
    state.xy = change_p(state.xy, 4, p5);
    printf("x:%d, y:%d, d:%d, p1:%d, p2:%d, p3:%d, p4:%d, p5:%d\n", get_x(state.xy), get_y(state.xy), 
    get_d(state.xy), get_p(state.xy, 0), get_p(state.xy, 1), get_p(state.xy, 2), get_p(state.xy, 3), 
    get_p(state.xy, 4));
}

void test_find_positions() {
    char **map = generate_map(10, 10, 3); //generovanie nahodnej mapy 
    POSITION dragon_pos, *princess_pos = (POSITION *)malloc(5 * SIZE_POSITION);
    int princess_counter = 0;
    find_positions(map, 10, 10, &dragon_pos, &princess_pos, &princess_counter);
    print_map(map, 10, 10, 0);
    printf("princess positions\n");
    for (int i = 0; i < princess_counter; i++)
       printf("p%d:[%d,%d] ", i+1, get_x(princess_pos[i].xy), get_y(princess_pos[i].xy));
    printf("\n");
    printf("dragon positions:[%d,%d]\n", get_x(dragon_pos.xy), get_y(dragon_pos.xy));
}

void test_print_structure_size() {
    printf("Velkost MAP_I: %d\nVelkost POSITION: %d\n", SIZE_MAP_I, SIZE_POSITION);
}
//*************************************************//

/*
    //ziska cisla princezien
    int *princess_num_arr = (int *)malloc(SIZE_INT * princess_counter);
    for (int i = 0; i < princess_counter; i++) {
        princess_num_arr[i] = i;
    }

    FILE *fp = fopen("princess_permutations.txt", "r+");
    if (fp == NULL) {
        printf("Error pri otvarani suboru.\n");
        exit(1);
    }
    princess_permutation(princess_num_arr, 0, princess_counter, &fp);
    free(princess_num_arr);
    int permutation_num = factorial(princess_counter);
    int **all_princess_permutation = (int **)malloc(sizeof(int *) * permutation_num);
    for (int i = 0; i < permutation_num; i++) {
        all_princess_permutation[i] = (int *)malloc(sizeof(int) * princess_counter);
    }
    fseek(fp, 0, SEEK_SET);
    char ch;
    for (int i = 0; i < permutation_num; i++) {        
        for (int j = 0; j < princess_counter; j++) {
            ch = (char)fgetc(fp);
            if (ch != '\n') {
                all_princess_permutation[i][j] = ch - '0';
            }
        }
        ch = (char)fgetc(fp);
    }
    fclose(fp);
    free(fp);
    
    POSITION *tmp_cesta = (POSITION *)malloc(SIZE_POSITION * (n * m / 2));
    POSITION *best_cesta = (POSITION *)malloc(SIZE_POSITION * (n * m / 2));
    set_path(&best_cesta, n * m / 2);
    set_path(&tmp_cesta, n * m / 2);
    int tmp_dlzka = 0, best_dlzka = INT_MAX;
    for (int i = 0; i < permutation_num; i++) {
        for (int j = 0; j < princess_counter; j++) {
            map_info = reset(map_info, n, m);
            map_info = dijkstra(mapa, map_info, state, n, m, t);
            p_pos = princess_pos[all_princess_permutation[i][j]];
            get_path(&tmp_cesta, &tmp_dlzka, map_info, p_pos);
            state = change_x(state, get_x(p_pos.xy));
            state = change_y(state, get_y(p_pos.xy));
        }
        if (tmp_dlzka < best_dlzka) {
                best_dlzka = tmp_dlzka;
                set_path(&best_cesta, best_dlzka);
                for (int k = 0; k < best_dlzka; k++) {
                    best_cesta[k] = tmp_cesta[k];
                }
        } 
        state = change_x(state, get_x(dragon_pos.xy));
        state = change_y(state, get_y(dragon_pos.xy));
        set_path(&tmp_cesta, tmp_dlzka);

    }
    print_path(best_cesta);
    free(tmp_cesta);
    */