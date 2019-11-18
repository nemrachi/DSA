#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define WIDTH 7 //m
#define HEIGHT 5 //n
#define SIZE_INT sizeof(int)
#define SIZE_P_INT sizeof(int *)

//ascii hodnoty pre mapu
#define C 67 //lesny chodnik
#define H 72 //lesny porast (2x pomalsie ako C)
#define N 78 //nepriechodna prekazka
#define D 68 //drak
#define P 80 //princezna
//teleporty su cisla od 0-9
int teleport_id = 0;
#define G 71

#define PRINCESS_NUM 2

//funkcia vrati nahodne cisla z rozmedzia urceneho pouzivatelom
int get_random_range(int lower, int upper) 
{
    return (rand() % (upper + 1 - lower)) + lower;
}

int get_percentage(int num, int perc) 
{   
    float div = (float)perc / 100;
    printf("%.2f\n", div);
    float result = div * num;
    printf("%.2f\n", result);
    return ceil(result); // zaokruhly nahor
}

int **allocate_map(int width, int height)
{
    int **map = (int **)malloc(width * SIZE_P_INT);
    for (int i = 0; i < width; i++)
    {
        map[i] = (int *)malloc(height * SIZE_INT);
    }
    return map;
}

int get_element(int element) 
{
    switch (element)
    {
        case 0:
            return C; //trail
            break;
        
        case 1:
            return H; //undergrowth
            break;

        case 2:
            return N; //impassable
            break;

        case 3:
            return D; //dragon
            break;

        case 4:
            return P; //princess
            break;

        case 5: //teleporty
            return teleport_id;
            break;

        case 6:
            return G; //generator
            break;
        
        default:
            break;
    }
}

void print_map(int **map, int m, int n) {
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            switch (map[i][j])
            {
            case C:
                printf("C ");
                break;
            
            case H:
                printf("H ");
                break;
            
            case N:
                printf("N ");
                break;
            
            case D:
                printf("D ");
                break;
            
            case P:
                printf("P ");
                break;
            
            case 0 ... 9:
                printf("%d ", map[i][j]);
                break;
            
            case G:
                printf("G ");
                break;
            
            default:
                break;
            }
        }
        printf("\n");
    }
}

//kvoli testovaniu
int **generate_map(int width, int height, int princess_num)
{    
    //alokovanie mapy
    int **map = allocate_map(width, height);

    /*
    urcenie poctu jednotlivych prvkov na mape
    1 drak                              \
    (zatial konstantne) 2 princezne    -- suma 5 
    (zatial konstantne) 2 teleporty      /

    pocet lesnych ciest = 65% zo zostatku (novy zostatok)
    pocet porastov = 70% z noveho zostatku
    pocet nepriechodnych ciest = uplny zvysok
    */

    int dragon_num = 1;
    int teleport_num = 4; //teleporty su len 2 potom
    int generator_num = 1;
    int remainder = (width * height) - dragon_num - princess_num - teleport_num - generator_num; //27
    int trail_num = get_percentage(remainder, 65);
    remainder -= trail_num;
    int undergrowth_num = get_percentage(remainder, 70);
    remainder -= undergrowth_num;
    int impassable_num = remainder;

    int elements_arr[] = {trail_num, undergrowth_num, impassable_num,
                            dragon_num, princess_num, teleport_num, generator_num};
    
    printf("Num of element ");
    for (int i = 0; i < 7; i++)
    {
        printf("%d ", elements_arr[i]);
    }
    printf("\n");
    

    int element_index, teleport_count = 0;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            while (1)
            {
                element_index = get_random_range(0, 6);
                if (elements_arr[element_index]) 
                {
                    map[i][j] = get_element(element_index);
                    elements_arr[element_index]--;

                    if (element_index == 5)
                    {
                        if (1 == teleport_count++)
                        {
                            teleport_id++;
                            teleport_count = 0;
                        }
                    }
                    break;
                }
            }
        }
    }

    print_map(map, width, height);

    return map;
}

// int *zachran_princezne(char **map, int n, int m, int t, int *dlzka_cesty)
// {
// }


int main() 
{
    srand(time(0));

    //generovanie mapy + jej vykreslenie
    int **map = generate_map(WIDTH, HEIGHT, PRINCESS_NUM);


    //zachrana
    //int *cesta = zachran_princezne(map, n, m, t, &dlzka_cesty);
    // for (i = 0;i<dlzka_cesty;++i)
    //     printf("%d %d\n", cesta[i*2], cesta[i*2+1]);

    for (int i = 0; i < WIDTH; i++)
    {
        free(map[i]);
    }
    free(map);

    return 0;
}