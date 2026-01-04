#include "solve.h"
#include "oll.h"
#include "pll.h"

#include <stdio.h>

int init_search(int, int);
int search(int, int stage);

int edges_solved(void);
int first_edge_solved(void) { return edges_solved() >= 1; }
int secnd_edge_solved(void) { return edges_solved() >= 2; }
int third_edge_solved(void) { return edges_solved() >= 3; }
int cross_solved(void) { return edges_solved() == 4; }

int f2l_pairs_solved(void);
int first_pair_solved(void) { return f2l_pairs_solved() >= 1; }
int secnd_pair_solved(void) { return f2l_pairs_solved() >= 2; }
int third_pair_solved(void) { return f2l_pairs_solved() >= 3; }
int fourth_pair_solved(void) { return f2l_pairs_solved() == 4; }

typedef int (*stage_t)(void);

static struct cube *cube;

stage_t main_stages[] = {
	secnd_edge_solved,
	cross_solved,
	first_pair_solved,
	secnd_pair_solved,
	third_pair_solved,
	fourth_pair_solved,
};

int main_max_depth[] = {
	5,
	5,
	7,
	7,
	7,
	5,
};

// If cross-search exceeds depth limit: assemble cross 1-by-1
stage_t alt_stages[] = {
	first_edge_solved,
	secnd_edge_solved,
	third_edge_solved,
	cross_solved,
	first_pair_solved,
	secnd_pair_solved,
	third_pair_solved,
	fourth_pair_solved,
};

int alt_max_depth[] = {
	5,
	5,
	5,
	5,
	7,
	7,
	7,
	5,
};

stage_t *stages = main_stages;
int num_stages = sizeof main_stages / sizeof *main_stages;
const int *max_depth = main_max_depth;

enum moves move_list[80] = { 0 };
int move_len = 0;

struct cube *visited_nodes[512] = { 0 };
int visited_node_len = 0;

int solve(void)
{
	if (!init_search(0, 0)) {
		stages = alt_stages;
		max_depth = alt_max_depth;
		num_stages = sizeof alt_stages / sizeof *alt_stages;
		if (init_search(0, 0)) {
		} else {
			print_cube(cube);
			printf("Failed to solve F2L\n");
			return -1;
		}
	}
	
	
	if (!solve_oll(cube)) {
		print_cube(cube);
		printf("Failed to solve OLL\n");
		return -1;
	}

	if (!solve_pll(cube)) {
		print_cube(cube);
		printf("Failed to solve PLL\n");
		return -1;
	}

	return 0;
}

void set_cube(struct cube *c)
{
	cube = c;
}

int init_search(int depth, int stage)
{
	if (!search(depth, stage)) {
		return 0;
	}

	for (int i = move_len - 1; i >= 0; i--) {
		append_move(move_list[i]);
	}

	// printf("Backtracked nodes: %d\n", visited_node_len);
	return 1;
}

int search(int depth, int stage)
{
	if (stage >= num_stages) {
		return 1;
	}

	if (depth > max_depth[stage]) {
		// printf("Depth maxed out on stage %d\n", stage);
		return 0;
	}

	// printf("Searching stage %d depth %d\n", stage, depth);
	stage_t f = stages[stage];

	if (depth == 0) {
		if (f()) {
			if (search(0, stage + 1)) {
				return 1;
			}
		}
		return search(1, stage);
	}

	enum moves stage_moves[depth];
	for (int i = 0; i < depth; i++) {
		stage_moves[i] = MOVE_D;
		apply_move(cube, MOVE_D);
	}

	while (1) {
		// Check for a success
		if (f()) {
			for (int i = 0; i < visited_node_len; i++) {
				if (cube_equal(cube, visited_nodes[i])) {
					// printf("Skipping visited node\n");
					goto next_perm;
				}
			}
			if (search(0, stage + 1)) {
				goto success;
			}
			// Backpropogate, add failed state
			if (visited_node_len >= 512) {
				printf("Visited node buffer filled\n");
				continue;
			}
			visited_nodes[visited_node_len++] = copy_cube(cube);
		}

next_perm:
		// Increment the permutation
		apply_move(cube, invert_move(stage_moves[depth - 1]));
		stage_moves[depth - 1]++;
		if (stage_moves[depth-1] == MOVE_U) {
			stage_moves[depth-1]++;
		}
		int j = depth - 1;
		for (; j >= 0; j--) {
			// "Carry" the last move(s)
			if (stage_moves[j] == MOVE_Up) {
				if (j == 0) { goto fail; }
				stage_moves[j] = MOVE_D;
				apply_move(cube, invert_move(stage_moves[j-1]));
				stage_moves[j-1]++;
				if (stage_moves[j-1] == MOVE_U) {
					stage_moves[j-1]++;
				}
			} else {
				break;
			}
		}
		for (; j < depth; j++) {
			apply_move(cube, stage_moves[j]);
		}
	}

fail:
	return search(depth + 1, stage);

success:
	for (int i = depth - 1; i >= 0; i--) {
		move_list[move_len++] = stage_moves[i];
	}

	return 1;
}

int edges_solved(void)
{
	return (cube->stickers[1] == 1)
		+ (cube->stickers[3] == 3)
		+ (cube->stickers[5] == 5)
		+ (cube->stickers[7] == 7);
}

int f2l_pairs_solved(void)
{
	if (!cross_solved()) { return 0; }

	#define CHECK(n) (cube->stickers[n] == n)
	return (CHECK(0) && CHECK(1*9+3))
		+ (CHECK(2) && CHECK(3*9+5))
		+ (CHECK(6) && CHECK(2*9+3))
		+ (CHECK(8) && CHECK(2*9+5));
}