#pragma once

#include "cube.h"

/*
	Faces are in order of ULFBRD
	Colors are encoded as wogrby
	Returns 0 on success, -1 otherwise
*/
int scan_cube(struct cube*, const char[54]);