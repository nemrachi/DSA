#include<stdio.h>
#define SIZE 50

char tiles[SIZE];
int count = 0;

void print_tiles(int n, int index) {
	if (n >= 2) {
		tiles[index] = '=';
		print_tiles(n - 2, index + 1);
	}
    
	if (n >= 1) {
		tiles[index] = '|';
		print_tiles(n - 1, index + 1);
	}
	
	else {
		for (int i = 0; i < index; ++i) {
			printf("%c", tiles[i]);
		}
        count++;
		printf("\n");
	}
}

int main() {
    int input;
    scanf("%d", &input);

    if (input >= 25) {
        return 0;
    }

	print_tiles(input, 0);
    printf("%d\n", count);
    return 0;
}