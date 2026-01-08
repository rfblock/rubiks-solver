#include "scan.h"

#include <stdlib.h>
#include <stdio.h>

// Centers point to themselves
// Edges point to the adjacent sticker
// Corners point to the next adjacent sticker moving clockwise
int adjacent[54] = {
	1*9+0, 4*9+1, 4*9+0,   1*9+1, 0*9+4, 3*9+1,   2*9+0, 2*9+1, 3*9+0,
	4*9+2, 0*9+3, 0*9+6,   4*9+5, 1*9+4, 2*9+3,   5*9+6, 5*9+3, 2*9+6,
	1*9+2, 0*9+7, 0*9+8,   1*9+5, 2*9+4, 3*9+3,   5*9+0, 5*9+1, 3*9+6,
	2*9+2, 0*9+5, 0*9+2,   2*9+5, 3*9+4, 4*9+3,   5*9+2, 5*9+5, 4*9+6,
	3*9+2, 0*9+1, 0*9+0,   3*9+5, 4*9+4, 1*9+3,   5*9+8, 5*9+7, 1*9+6,
	1*9+8, 2*9+7, 2*9+8,   1*9+7, 5*9+4, 3*9+7,   4*9+8, 4*9+7, 3*9+8,
};

int scan_corner(const char stickers[54], int i);
int scan_edge(const char stickers[54], int i);

int identify_corner(int c1, int c2, int c3);
int identify_edge(int c1, int c2);

static int get_color(char c);

int scan_cube(struct cube *cube, const char stickers[54])
{
	if (cube == NULL) {
		printf("Unable to scan cube (cube NULL)\n");
		return -1;
	}

	for (int i = 0; i < 54; i++) {
		int id = -1;

		switch (i % 9) {
		case 0:
		case 2:
		case 6:
		case 8:
			id = scan_corner(stickers, i);
			break;
		case 1:
		case 3:
		case 5:
		case 7:
			id = scan_edge(stickers, i);
			break;
		default:
			continue;
		}
		if (id < 0) {
			continue;
		}

		cube->stickers[i] = id;
	}

	return 0;
}

int scan_corner(const char stickers[54], int i)
{
	int j = adjacent[i];
	int k = adjacent[j];

	char s1 = stickers[i];
	char s2 = stickers[j];
	char s3 = stickers[k];

	int c1 = get_color(s1);
	int c2 = get_color(s2);
	int c3 = get_color(s3);

	return identify_corner(c1, c2, c3);
}

int scan_edge(const char stickers[54], int i)
{
	int j = adjacent[i];
	char s1 = stickers[i];
	char s2 = stickers[j];

	int c1 = get_color(s1);
	int c2 = get_color(s2);

	return identify_edge(c1, c2);
}

int identify_corner(int c1, int c2, int c3)
{
	int corner_indices[] = { 0, 2, 6, 8 };
	for (int i = 0; i < 4; i++) {
		int i2 = c2*9 + corner_indices[i];
		int i3 = adjacent[i2];
		int i1 = adjacent[i3];

		int col1 = i1 / 9;
		int col3 = i3 / 9;

		if (c1 == col1 && c3 == col3) {
			return i1;
		}
	}

	printf("Unable to identify corner %d %d %d\n", c1, c2, c3);
	return -1;
}

int identify_edge(int c1, int c2)
{
	int edge_indices[] = { 1, 3, 5, 7 };
	for (int i = 0; i < 4; i++) {
		int idx = c2*9 + edge_indices[i];
		int adj_idx = adjacent[idx];
		int adj_col = adj_idx / 9;
		if (c1 == adj_col) {
			return adj_idx;
		}
	}

	printf("Unable to identify edge: %d %d\n", c1, c2);
	return -1;
}

int get_color(char c)
{
	switch (c) {
		case 'W': return 0;
		case 'O': return 1;
		case 'G': return 2;
		case 'R': return 3;
		case 'B': return 4;
		case 'Y': return 5;
		default:
	}
	printf("Unable to scan color, %c\n", c);
	return -1;
}