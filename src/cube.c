#include "cube.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Green-F White-U
// Stored as
// ... UUU ... ...
// ... U0U ... ...
// ... UUU ... ...
// LLL FFF RRR BBB     012
// L1L F2F R3R B4B     345
// LLL FFF RRR BBB     678
// ... DDD ... ...
// ... D5D ... ...
// ... DDD ... ...
struct cube *init_cube(void)
{
	struct cube *cube = malloc(sizeof *cube);
	for (int i = 0; i < 54; i++) {
		cube->stickers[i] = i;
	}

	return cube;
}

enum moves invert_move(enum moves move)
{
	if (move <= MOVE_U) {
		return move + 6;
	}
	if (move <= MOVE_Up) {
		return move - 6;
	}

	return move;
}

enum moves double_move(enum moves move)
{
	if (move <= MOVE_U) {
		return move + 12;
	}
	if (move <= MOVE_Up) {
		return move + 6;
	}

	return MOVE_END;
}

void apply_algorithm(struct cube *cube, enum moves *alg)
{
	enum moves m = alg[0];
	int i = 0;
	while (m != MOVE_END) {
		m = alg[i++];
		apply_move(cube, m);
	}
}

struct cube *copy_cube(struct cube *cube)
{
	struct cube *new_cube = malloc(sizeof *new_cube);
	memcpy(new_cube->stickers, cube->stickers, sizeof cube->stickers);
	return new_cube;
}

int cube_equal(struct cube *c1, struct cube *c2)
{
	for (int i = 0; i < 54; i++) {
		if (c1->stickers[i] != c2->stickers[i]) { return 0; }
	}

	return 1;
}

void rotate(struct cube *cube, int c, int s11, int s12, int s13, int s21, int s22, int s23, int s31, int s32, int s33, int s41, int s42, int s43)
{
	char tmp = cube->stickers[c+0];
	cube->stickers[c+0] = cube->stickers[c+6];
	cube->stickers[c+6] = cube->stickers[c+8];
	cube->stickers[c+8] = cube->stickers[c+2];
	cube->stickers[c+2] = tmp;

	tmp = cube->stickers[c+1];
	cube->stickers[c+1] = cube->stickers[c+3];
	cube->stickers[c+3] = cube->stickers[c+7];
	cube->stickers[c+7] = cube->stickers[c+5];
	cube->stickers[c+5] = tmp;

	tmp = cube->stickers[s11];
	cube->stickers[s11] = cube->stickers[s41];
	cube->stickers[s41] = cube->stickers[s31];
	cube->stickers[s31] = cube->stickers[s21];
	cube->stickers[s21] = tmp;

	tmp = cube->stickers[s12];
	cube->stickers[s12] = cube->stickers[s42];
	cube->stickers[s42] = cube->stickers[s32];
	cube->stickers[s32] = cube->stickers[s22];
	cube->stickers[s22] = tmp;

	tmp = cube->stickers[s13];
	cube->stickers[s13] = cube->stickers[s43];
	cube->stickers[s43] = cube->stickers[s33];
	cube->stickers[s33] = cube->stickers[s23];
	cube->stickers[s23] = tmp;
}

void rotate_p(struct cube *cube, int c, int s11, int s12, int s13, int s21, int s22, int s23, int s31, int s32, int s33, int s41, int s42, int s43)
{
	char tmp = cube->stickers[c+2];
	cube->stickers[c+2] = cube->stickers[c+8];
	cube->stickers[c+8] = cube->stickers[c+6];
	cube->stickers[c+6] = cube->stickers[c+0];
	cube->stickers[c+0] = tmp;

	tmp = cube->stickers[c+5];
	cube->stickers[c+5] = cube->stickers[c+7];
	cube->stickers[c+7] = cube->stickers[c+3];
	cube->stickers[c+3] = cube->stickers[c+1];
	cube->stickers[c+1] = tmp;

	tmp = cube->stickers[s21];
	cube->stickers[s21] = cube->stickers[s31];
	cube->stickers[s31] = cube->stickers[s41];
	cube->stickers[s41] = cube->stickers[s11];
	cube->stickers[s11] = tmp;

	tmp = cube->stickers[s22];
	cube->stickers[s22] = cube->stickers[s32];
	cube->stickers[s32] = cube->stickers[s42];
	cube->stickers[s42] = cube->stickers[s12];
	cube->stickers[s12] = tmp;

	tmp = cube->stickers[s23];
	cube->stickers[s23] = cube->stickers[s33];
	cube->stickers[s33] = cube->stickers[s43];
	cube->stickers[s43] = cube->stickers[s13];
	cube->stickers[s13] = tmp;
}

void apply_move(struct cube *cube, enum moves move)
{
	switch (move) {
	case MOVE_U:
		rotate(cube, 0*9, 2*9+2, 2*9+1, 2*9+0, 1*9+2, 1*9+1, 1*9+0, 4*9+2, 4*9+1, 4*9+0, 3*9+2, 3*9+1, 3*9+0);
		break;
	case MOVE_U2:
		rotate(cube, 0*9, 2*9+2, 2*9+1, 2*9+0, 1*9+2, 1*9+1, 1*9+0, 4*9+2, 4*9+1, 4*9+0, 3*9+2, 3*9+1, 3*9+0);
		rotate(cube, 0*9, 2*9+2, 2*9+1, 2*9+0, 1*9+2, 1*9+1, 1*9+0, 4*9+2, 4*9+1, 4*9+0, 3*9+2, 3*9+1, 3*9+0);
		break;
	case MOVE_Up:
		rotate_p(cube, 0*9, 2*9+2, 2*9+1, 2*9+0, 1*9+2, 1*9+1, 1*9+0, 4*9+2, 4*9+1, 4*9+0, 3*9+2, 3*9+1, 3*9+0);
		break;
	case MOVE_L:
		rotate(cube, 1*9, 0*9+0, 0*9+3, 0*9+6, 2*9+0, 2*9+3, 2*9+6, 5*9+0, 5*9+3, 5*9+6, 4*9+8, 4*9+5, 4*9+2);
		break;
	case MOVE_L2:
		rotate(cube, 1*9, 0*9+0, 0*9+3, 0*9+6, 2*9+0, 2*9+3, 2*9+6, 5*9+0, 5*9+3, 5*9+6, 4*9+8, 4*9+5, 4*9+2);
		rotate(cube, 1*9, 0*9+0, 0*9+3, 0*9+6, 2*9+0, 2*9+3, 2*9+6, 5*9+0, 5*9+3, 5*9+6, 4*9+8, 4*9+5, 4*9+2);
		break;
	case MOVE_Lp:
		rotate_p(cube, 1*9, 0*9+0, 0*9+3, 0*9+6, 2*9+0, 2*9+3, 2*9+6, 5*9+0, 5*9+3, 5*9+6, 4*9+8, 4*9+5, 4*9+2);
		break;
	case MOVE_F:
		rotate(cube, 2*9, 0*9+6, 0*9+7, 0*9+8, 3*9+0, 3*9+3, 3*9+6, 5*9+2, 5*9+1, 5*9+0, 1*9+8, 1*9+5, 1*9+2);
		break;
	case MOVE_F2:
		rotate(cube, 2*9, 0*9+6, 0*9+7, 0*9+8, 3*9+0, 3*9+3, 3*9+6, 5*9+2, 5*9+1, 5*9+0, 1*9+8, 1*9+5, 1*9+2);
		rotate(cube, 2*9, 0*9+6, 0*9+7, 0*9+8, 3*9+0, 3*9+3, 3*9+6, 5*9+2, 5*9+1, 5*9+0, 1*9+8, 1*9+5, 1*9+2);
		break;
	case MOVE_Fp:
		rotate_p(cube, 2*9, 0*9+6, 0*9+7, 0*9+8, 3*9+0, 3*9+3, 3*9+6, 5*9+2, 5*9+1, 5*9+0, 1*9+8, 1*9+5, 1*9+2);
		break;
	case MOVE_R:
		rotate(cube, 3*9, 0*9+8, 0*9+5, 0*9+2, 4*9+0, 4*9+3, 4*9+6, 5*9+8, 5*9+5, 5*9+2, 2*9+8, 2*9+5, 2*9+2);
		break;
	case MOVE_R2:
		rotate(cube, 3*9, 0*9+8, 0*9+5, 0*9+2, 4*9+0, 4*9+3, 4*9+6, 5*9+8, 5*9+5, 5*9+2, 2*9+8, 2*9+5, 2*9+2);
		rotate(cube, 3*9, 0*9+8, 0*9+5, 0*9+2, 4*9+0, 4*9+3, 4*9+6, 5*9+8, 5*9+5, 5*9+2, 2*9+8, 2*9+5, 2*9+2);
		break;
	case MOVE_Rp:
		rotate_p(cube, 3*9, 0*9+8, 0*9+5, 0*9+2, 4*9+0, 4*9+3, 4*9+6, 5*9+8, 5*9+5, 5*9+2, 2*9+8, 2*9+5, 2*9+2);
		break;
	case MOVE_B:
		rotate(cube, 4*9, 0*9+2, 0*9+1, 0*9+0, 1*9+0, 1*9+3, 1*9+6, 5*9+6, 5*9+7, 5*9+8, 3*9+8, 3*9+5, 3*9+2);
		break;
	case MOVE_B2:
		rotate(cube, 4*9, 0*9+2, 0*9+1, 0*9+0, 1*9+0, 1*9+3, 1*9+6, 5*9+6, 5*9+7, 5*9+8, 3*9+8, 3*9+5, 3*9+2);
		rotate(cube, 4*9, 0*9+2, 0*9+1, 0*9+0, 1*9+0, 1*9+3, 1*9+6, 5*9+6, 5*9+7, 5*9+8, 3*9+8, 3*9+5, 3*9+2);
		break;
	case MOVE_Bp:
		rotate_p(cube, 4*9, 0*9+2, 0*9+1, 0*9+0, 1*9+0, 1*9+3, 1*9+6, 5*9+6, 5*9+7, 5*9+8, 3*9+8, 3*9+5, 3*9+2);
		break;
	case MOVE_D:
		rotate(cube, 5*9, 2*9+6, 2*9+7, 2*9+8, 3*9+6, 3*9+7, 3*9+8, 4*9+6, 4*9+7, 4*9+8, 1*9+6, 1*9+7, 1*9+8);
		break;
	case MOVE_D2:
		rotate(cube, 5*9, 2*9+6, 2*9+7, 2*9+8, 3*9+6, 3*9+7, 3*9+8, 4*9+6, 4*9+7, 4*9+8, 1*9+6, 1*9+7, 1*9+8);
		rotate(cube, 5*9, 2*9+6, 2*9+7, 2*9+8, 3*9+6, 3*9+7, 3*9+8, 4*9+6, 4*9+7, 4*9+8, 1*9+6, 1*9+7, 1*9+8);
		break;
	case MOVE_Dp:
		rotate_p(cube, 5*9, 2*9+6, 2*9+7, 2*9+8, 3*9+6, 3*9+7, 3*9+8, 4*9+6, 4*9+7, 4*9+8, 1*9+6, 1*9+7, 1*9+8);
		break;
	default:
		return;
	}
}

const char *get_color(unsigned char sticker)
{
	if (sticker < 9) { return "\x1b[47mW\x1b[0m"; }
	if (sticker < 18) { return "\x1b[48;5;208mO\x1b[0m"; }
	if (sticker < 27) { return "\x1b[102mG\x1b[0m"; }
	if (sticker < 36) { return "\x1b[41mR\x1b[0m"; }
	if (sticker < 45) { return "\x1b[44mB\x1b[0m"; }
	if (sticker < 54) { return "\x1b[103mY\x1b[0m"; }
	return ".";
}

void print_cube(struct cube *cube)
{
	for (int i = 0; i < 9; i+=3) {
		printf("    %s%s%s        \n",
			get_color(cube->stickers[i]),
			get_color(cube->stickers[i+1]),
			get_color(cube->stickers[i+2])
		);
	}
	for (int i = 0; i < 9; i+= 3) {
		for (int j = 1; j < 5; j++) {
			printf("%s%s%s ",
				get_color(cube->stickers[9*j+i]),
				get_color(cube->stickers[9*j+i+1]),
				get_color(cube->stickers[9*j+i+2])
			);
		}
		printf("\n");
	}
	for (int i = 0; i < 9; i+=3) {
		printf("    %s%s%s        \n",
			get_color(cube->stickers[9*5+i]),
			get_color(cube->stickers[9*5+i+1]),
			get_color(cube->stickers[9*5+i+2])
		);
	}
}

static enum moves move_list[120] = { MOVE_END };
static int move_list_len = 0;
void append_move(enum moves move)
{
	if (move == MOVE_END) { return; }

	if (move_list_len == 0) {
		move_list[move_list_len++] = move;
		return;
	}

	enum moves last = move_list[move_list_len-1];
	if (move % 6 != last % 6) {
		move_list[move_list_len++] = move;
		return;
	}

	/*
	Case 1: Cancellation
	X  X' = I
	
	Case 2: Doubling
	X2 X2 = I
	X  X  = X2
	X' X' = X2

	Case 3: Inversion
	X  X2 = X'
	X' X2 = X
	*/

	if (move == last) { // Case 2
		move = double_move(move);
		move_list[move_list_len-1] = move;
		if (move == MOVE_END) {
			move_list_len--;
		}
		return;
	}

	if (last == invert_move(move)) { // Case 1
		move_list[--move_list_len] = MOVE_END;
		return;
	}

	// Case 3
	move = move >= MOVE_D2 ? last : move;
	move_list[move_list_len-1] = invert_move(move);
}

void print_move_list(void)
{
	for (int i = 0; i < move_list_len; i++) {
		print_move(move_list[i]);
	}
	printf("\n");
}

void print_move(enum moves move)
{
	switch (move) {
		case MOVE_END: printf("\n"); return;
		case MOVE_U:  printf("U "); return;
		case MOVE_L:  printf("L "); return;
		case MOVE_F:  printf("F "); return;
		case MOVE_R:  printf("R "); return;
		case MOVE_B:  printf("B "); return;
		case MOVE_D:  printf("D "); return;
		case MOVE_Up: printf("U' "); return;
		case MOVE_Lp: printf("L' "); return;
		case MOVE_Fp: printf("F' "); return;
		case MOVE_Rp: printf("R' "); return;
		case MOVE_Bp: printf("B' "); return;
		case MOVE_Dp: printf("D' "); return;
		case MOVE_U2: printf("U2 "); return;
		case MOVE_L2: printf("L2 "); return;
		case MOVE_F2: printf("F2 "); return;
		case MOVE_R2: printf("R2 "); return;
		case MOVE_B2: printf("B2 "); return;
		case MOVE_D2: printf("D2 "); return;
	}
}