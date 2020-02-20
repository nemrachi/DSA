#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#define POS_SIZE 10
#define BIT_SIZE 1
#define X_OFF 22
#define Y_OFF 12

void change_state(uint32_t *state, unsigned new_val, unsigned offset, unsigned bit_size)
{
    new_val <<= offset;
    unsigned reset_mask = ~(((1 << bit_size) - 1) << offset); // 3 - rozdiel medzi offsetmi
    (*state) &= reset_mask; //nastavi na 0 tam, kde bude davat nove bity
    (*state) = (*state) | new_val;
}

int get_x(uint32_t state)
{
    return (state >> X_OFF);
}

void change_x(uint32_t *state, int x)
{
    change_state((state), x, X_OFF, POS_SIZE);
}

int get_y(uint32_t state)
{
    return (state & (~(((1 << POS_SIZE) - 1) << X_OFF))) >> Y_OFF;
}

void change_y(uint32_t *state, int y)
{
    change_state((state), y, Y_OFF, POS_SIZE);
}

int print_bin32(uint32_t num, int bit)
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

int main()
{
    uint32_t state = 0;

    change_x(&state, 5);
    change_y(&state, 1023);

    printf("%d %d\n", get_x(state), get_y(state));

    print_bin32(state, 0);

    return 0;
}