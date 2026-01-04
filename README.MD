# Rubik's Solver
5-gen solver for 3x3x3 Rubik's cubes

## About
This is a program that solves Rubik's cubes using only five of the six sides. The top face (white) is not rotated

## Building
1. Clone or download the repository
2. Run `make` to build, `make run` to run with a random scramble
3. The binary is created at `/bin/solve[.exe]`

## Usage
 * `solve` -- Solve a random scramble
 * `solve --superflip` -- Solve a superflip position
 * `solve wbwowrwgw owobo...` -- `[TODO]` Solve a given position
	* Arguments are grouped by faces in order of `ULFRBD` (Singmaster notation)
	* Colors within each face are ordered by reading order (left-to-right, top-to-bottom)

## Implementation Details
This solver uses a multi-stage approach based on CFOP to find a solution. The first stages use a breadth-first search to solve the first two cross edges, full cross, first, second, third, and last pair. If a given stage (i.e. last pair) cannot be solved within a certain amount of moves, it will backtrack (i.e. resolve the third pair). The algorithm will then ensure it is in a different state and attempt to re-solve the previously failed stage. If it is unable to solve the cross within a certain number of moves, it will instead solve each edge of the cross individually.

The final two stages utilize standard[*](#footnote-algorithms) OLL and PLL algorithms. Instead of transforming the last algorithms (such as with `y` and `y'` moves), it uses a conjugate (`D`) to orient the last layer.

<a name="footnote-algorithms"></a>*Some algorithms are modified to not use the opposite (`U`) face.

## Sources
* https://jperm.net/algs/oll
* https://jperm.net/algs/pll
* https://www.cubeskills.com/uploads/pdf/tutorials/pll-algorithms.pdf (For G-Perms)
* https://www.stylewarning.com/posts/brute-force-rubiks-cube/