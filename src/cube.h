#pragma once

enum moves {
	MOVE_END,
	MOVE_D,
	MOVE_L,
	MOVE_F,
	MOVE_R,
	MOVE_B,
	MOVE_U,
	MOVE_Dp,
	MOVE_Lp,
	MOVE_Fp,
	MOVE_Rp,
	MOVE_Bp,
	MOVE_Up,
	MOVE_D2,
	MOVE_L2,
	MOVE_F2,
	MOVE_R2,
	MOVE_B2,
	MOVE_U2,
};

struct cube {
	unsigned char stickers[54];
};

struct cube *init_cube(void);
enum moves invert_move(enum moves);
enum moves double_move(enum moves);
void apply_algorithm(struct cube *cube, enum moves *alg);
void apply_move(struct cube *cube, enum moves move);
void print_cube(struct cube *cube);
void print_move(enum moves move);
void print_move_list(void);
void append_move(enum moves move);
struct cube *copy_cube(struct cube *cube);
int cube_equal(struct cube *c1, struct cube *c2);