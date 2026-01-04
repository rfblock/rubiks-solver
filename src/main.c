#include "cube.h"
#include "solve.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cube *cube;

enum moves superflip[] = { MOVE_D, MOVE_R2, MOVE_B, MOVE_F, MOVE_R, MOVE_F2, MOVE_R, MOVE_D2, MOVE_L, MOVE_F2, MOVE_R, MOVE_Dp, MOVE_Up, MOVE_R2, MOVE_B, MOVE_Rp, MOVE_L, MOVE_F2, MOVE_D2, MOVE_B2, MOVE_END };

void scramble(void);

int main(int argc, char *argv[])
{
	cube = init_cube();

	if (argc < 2) {
		scramble();
	} else if (argc == 2) {
		if (strcmp(argv[1], "--superflip") == 0) {
			apply_algorithm(cube, superflip);
		} else {
			printf("Invalid number of arguments\n");
			return -1;
		}
	} else if (argc == 7) {
		printf("TODO: Color entry");
		return -1;
	} else {
		printf("Invalid number of arguments\n");
		return -1;
	}

	print_cube(cube);
	printf("\n");
	
	set_cube(cube);
	solve();

	print_cube(cube);
	print_move_list();

	return 0;
}

void scramble(void)
{
	srand(0);
	for (int i = 0; i < 20; i++) {
		enum moves move = rand() % 18 + 1;
		apply_move(cube, move);
		print_move(move);
	}
	printf("\n");
}