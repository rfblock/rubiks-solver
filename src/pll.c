#include "pll.h"

#include <stdio.h>

/*
	012
	345
	678
*/

// https://jperm.net/algs/pll
// and https://www.cubeskills.com/uploads/pdf/tutorials/pll-algorithms.pdf
// (for G perms)
const unsigned char pll_cases[][9] = {
	{ 0, 1, 2,  3, 4, 5,  6, 7, 8 }, // Identity (PLL Skip)
	{ 6, 1, 0,  3, 4, 5,  2, 7, 8 }, // Aa
	{ 8, 1, 2,  3, 4, 5,  0, 7, 6 }, // Ab
	{ 0, 7, 8,  3, 4, 5,  6, 1, 2 }, // F
	{ 6, 5, 0,  1, 4, 3,  2, 7, 8 }, // Ga
	{ 2, 3, 6,  5, 4, 1,  0, 7, 8 }, // Gb
	{ 8, 1, 2,  7, 4, 3,  0, 5, 6 }, // Gc
	{ 6, 7, 0,  1, 4, 5,  2, 3, 8 }, // Gd
	{ 0, 5, 8,  3, 4, 1,  6, 7, 2 }, // Ja
	{ 0, 1, 8,  3, 4, 7,  6, 5, 2 }, // Jb
	{ 0, 3, 8,  1, 4, 5,  6, 7, 2 }, // Ra
	{ 0, 1, 8,  7, 4, 5,  6, 3, 2 }, // Rb
	{ 0, 1, 8,  5, 4, 3,  6, 7, 2 }, // T
	{ 6, 1, 8,  3, 4, 5,  0, 7, 2 }, // E
	{ 0, 1, 6,  5, 4, 3,  2, 7, 8 }, // Na
	{ 8, 1, 2,  5, 4, 3,  6, 7, 0 }, // Nb
	{ 8, 5, 2,  3, 4, 1,  6, 7, 0 }, // V
	{ 8, 3, 2,  1, 4, 5,  6, 7, 0 }, // Y
	{ 0, 7, 2,  5, 4, 3,  6, 1, 8 }, // H
	{ 0, 1, 2,  5, 4, 7,  6, 3, 8 }, // Ua
	{ 0, 1, 2,  7, 4, 3,  6, 5, 8 }, // Ub
	{ 0, 5, 2,  7, 4, 1,  6, 3, 8 }, // Z
};

enum moves pll_algs[][22] = {
	{ MOVE_END },
	{ MOVE_L2, MOVE_F2, MOVE_Lp, MOVE_Bp, MOVE_L, MOVE_F2, MOVE_Lp, MOVE_B, MOVE_Lp, MOVE_END },
	{ MOVE_L2, MOVE_B2, MOVE_L, MOVE_F, MOVE_Lp, MOVE_B2, MOVE_L, MOVE_Fp, MOVE_L, MOVE_END },
	{ MOVE_Rp, MOVE_Dp, MOVE_Bp, MOVE_R, MOVE_D, MOVE_Rp, MOVE_Dp, MOVE_Rp, MOVE_B, MOVE_R2, MOVE_Dp, MOVE_Rp, MOVE_Dp, MOVE_R, MOVE_D, MOVE_Rp, MOVE_D, MOVE_R, MOVE_END },
	{ MOVE_R2, MOVE_D, MOVE_Rp, MOVE_D, MOVE_Rp, MOVE_Dp, MOVE_R, MOVE_Dp, MOVE_R2, MOVE_U, MOVE_Dp, MOVE_Rp, MOVE_D, MOVE_R, MOVE_Up, MOVE_D, MOVE_END },
	{ MOVE_Bp, MOVE_Dp, MOVE_B, MOVE_R2, MOVE_U, MOVE_Fp, MOVE_D, MOVE_F, MOVE_Dp, MOVE_F, MOVE_Up, MOVE_R2, MOVE_END },
	{ MOVE_R2, MOVE_Dp, MOVE_R, MOVE_Dp, MOVE_R, MOVE_D, MOVE_Rp, MOVE_D, MOVE_R2, MOVE_Up, MOVE_D, MOVE_R, MOVE_Dp, MOVE_Rp, MOVE_U, MOVE_Dp, MOVE_END },
	{ MOVE_Up, MOVE_R, MOVE_D, MOVE_Rp, MOVE_Dp, MOVE_U, MOVE_R2, MOVE_Dp, MOVE_R, MOVE_Dp, MOVE_Rp, MOVE_D, MOVE_Rp, MOVE_D, MOVE_R2, MOVE_D, MOVE_END },
	{ MOVE_R2, MOVE_U, MOVE_R, MOVE_Up, MOVE_R, MOVE_B2, MOVE_Lp, MOVE_D, MOVE_L, MOVE_B2, MOVE_END },
	{ MOVE_R, MOVE_D, MOVE_Rp, MOVE_Bp, MOVE_R, MOVE_D, MOVE_Rp, MOVE_Dp, MOVE_Rp, MOVE_B, MOVE_R2, MOVE_Dp, MOVE_Rp, MOVE_END },
	{ MOVE_R, MOVE_Dp, MOVE_Rp, MOVE_Dp, MOVE_R, MOVE_D, MOVE_R, MOVE_U, MOVE_Rp, MOVE_Dp, MOVE_R, MOVE_Up, MOVE_Rp, MOVE_D2, MOVE_Rp, MOVE_END },
	{ MOVE_R2, MOVE_B, MOVE_R, MOVE_D, MOVE_R, MOVE_Dp, MOVE_Rp, MOVE_Bp, MOVE_R, MOVE_D2, MOVE_Rp, MOVE_D2, MOVE_R, MOVE_END },
	{ MOVE_R, MOVE_D, MOVE_Rp, MOVE_Dp, MOVE_Rp, MOVE_B, MOVE_R2, MOVE_Dp, MOVE_Rp, MOVE_Dp, MOVE_R, MOVE_D, MOVE_Rp, MOVE_Bp, MOVE_END },
	{ MOVE_Lp, MOVE_F, MOVE_L, MOVE_Bp, MOVE_Lp, MOVE_Fp, MOVE_L, MOVE_B, MOVE_Lp, MOVE_Fp, MOVE_L, MOVE_Bp, MOVE_Lp, MOVE_F, MOVE_L, MOVE_B, MOVE_END },
	{ MOVE_R, MOVE_D, MOVE_Rp, MOVE_D, MOVE_R, MOVE_D, MOVE_Rp, MOVE_Bp, MOVE_R, MOVE_D, MOVE_Rp, MOVE_Dp, MOVE_Rp, MOVE_B, MOVE_R2, MOVE_Dp, MOVE_Rp, MOVE_D2, MOVE_R, MOVE_Dp, MOVE_Rp, MOVE_END },
	{ MOVE_Rp, MOVE_D, MOVE_R, MOVE_Dp, MOVE_Rp, MOVE_Bp, MOVE_Dp, MOVE_B, MOVE_R, MOVE_D, MOVE_Rp, MOVE_B, MOVE_Rp, MOVE_Bp, MOVE_R, MOVE_Dp, MOVE_R, MOVE_END },
	{ MOVE_Rp, MOVE_D, MOVE_Rp, MOVE_Dp, MOVE_Fp, MOVE_Rp, MOVE_F2, MOVE_Dp, MOVE_Fp, MOVE_D, MOVE_Fp, MOVE_R, MOVE_F, MOVE_R, MOVE_END },
	{ MOVE_B, MOVE_R, MOVE_Dp, MOVE_Rp, MOVE_Dp, MOVE_R, MOVE_D, MOVE_Rp, MOVE_Bp, MOVE_R, MOVE_D, MOVE_Rp, MOVE_Dp, MOVE_Rp, MOVE_B, MOVE_R, MOVE_Bp, MOVE_END },
	{ MOVE_R2, MOVE_L2, MOVE_U, MOVE_R2, MOVE_L2, MOVE_D2, MOVE_R2, MOVE_L2, MOVE_U, MOVE_R2, MOVE_L2, MOVE_END },
	{ MOVE_R2, MOVE_L2, MOVE_U, MOVE_R, MOVE_Lp, MOVE_B2, MOVE_Rp, MOVE_L, MOVE_U, MOVE_R2, MOVE_L2, MOVE_END },
	{ MOVE_R2, MOVE_L2, MOVE_Up, MOVE_R, MOVE_Lp, MOVE_B2, MOVE_Rp, MOVE_L, MOVE_Up, MOVE_R2, MOVE_L2, MOVE_END },
	{ MOVE_Rp, MOVE_L, MOVE_B, MOVE_R2, MOVE_L2, MOVE_F, MOVE_R2, MOVE_L2, MOVE_B, MOVE_Rp, MOVE_L, MOVE_U2, MOVE_R2, MOVE_L2, MOVE_END },
};

const char *case_names[] = {
	"0", "Aa", "Ab", "F", "Ga", "Gb", "Gc", "Gd", "Ja", "Jb", "Ra", "Rb", "T", "E", "Na", "Nb", "V", "Y", "H", "Ua", "Ub", "Z"
};

static const int num_cases = sizeof pll_cases / sizeof *pll_cases;

// Returns the rotation if correct, -1 otherwise
int check_pll(struct cube *cube, int pll_id)
{
	int arr[9] = { 0 };
	for (int i = 0; i < 9; i++) {
		int idx = 5*9 + pll_cases[pll_id][i];
		arr[i] = cube->stickers[idx];
	}

	int expected[][9] = {
		{
			0,1,2,
			3,4,5,
			6,7,8,
		},{
			6,3,0,
			7,4,1,
			8,5,2,
		},{
			8,7,6,
			5,4,3,
			2,1,0,
		},{
			2,5,8,
			1,4,7,
			0,3,6,
		}
	};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 9; j++) {
			int exp = 5*9 + expected[i][j];
			if (arr[j] != exp) { goto fail; }
		}
		return i;
		fail:
	}

	return -1;
}

int solve_pll(struct cube *cube)
{
	int rot;
	int post_rot;
	int id;
	for (rot = 0; rot < 4; rot++) {
		for (id = 0; id < num_cases; id++) {
			post_rot = check_pll(cube, id);
			if (post_rot < 0) { continue; }
			goto found;
		}
		apply_move(cube, MOVE_D);
	}

	return 0;

found:
	// printf("PLL: case %s with rotations %d/%d\n", case_names[id], rot, post_rot);

	for (int i = 0; i < rot; i++) {
		append_move(MOVE_D);
	}
	for (int i = 0; pll_algs[id][i] != MOVE_END; i++) {
		apply_move(cube, pll_algs[id][i]);
		append_move(pll_algs[id][i]);
	}
	for (int i = 0; i < post_rot; i++) {
		apply_move(cube, MOVE_Dp);
		append_move(MOVE_Dp);
	}

	return 1;
}