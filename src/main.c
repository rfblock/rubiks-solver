#include "cube.h"
#include "solve.h"
#include "scan.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cube *cube;

enum moves superflip[] = { MOVE_D, MOVE_R2, MOVE_B, MOVE_F, MOVE_R, MOVE_F2, MOVE_R, MOVE_D2, MOVE_L, MOVE_F2, MOVE_R, MOVE_Dp, MOVE_Up, MOVE_R2, MOVE_B, MOVE_Rp, MOVE_L, MOVE_F2, MOVE_D2, MOVE_B2, MOVE_END };

void scramble(void);

void decode_cube(char *argv[]);

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
		decode_cube(argv);
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

void decode_cube(char *argv[])
{
	char stickers[54] = { 0 };

	for (int i = 0; i < 6; i++) {
		char *face = argv[i + 1];
		if (strlen(face) != 9) {
			printf("Invalid face: %s\n", face);
			continue;
		}

		for (int j = 0; j < 9; j++) {
			int idx = i*9 + j;
			stickers[idx] = face[j];
		}
	}

	scan_cube(cube, stickers);
}