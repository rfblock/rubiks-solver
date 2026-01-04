with open('superflip.txt') as f:
	for line in f.readlines():
		up = 'U'
		left = 'L'
		front = 'F'
		right = 'R'
		back = 'B'
		down = 'D'

		slice_subs = {
			"M": "R L' x'".split(' '),
			"E": "U D' y'".split(' '),
			"S": "F B' z".split(' '),

			"M2": "R2 L2 x2".split(' '),
			"E2": "U2 D2 y2".split(' '),
			"S2": "F2 B2 z2".split(' '),

			"M'": "R' L x".split(' '),
			"E'": "U' D y".split(' '),
			"S'": "F' B z'".split(' '),
		}

		wide_subs = {
			"u": "D y".split(' '),
			"l": "R x'".split(' '),
			"f": "B z".split(' '),
			"r": "L x".split(' '),
			"b": "F z'".split(' '),
			"d": "U y'".split(' '),

			"u'": "D' y'".split(' '),
			"l'": "R' x".split(' '),
			"f'": "B' z'".split(' '),
			"r'": "L' x'".split(' '),
			"b'": "F' z".split(' '),
			"d'": "U' y".split(' '),
			
			"u2": "D2 y2".split(' '),
			"l2": "R2 x2".split(' '),
			"f2": "B2 z2".split(' '),
			"r2": "L2 x2".split(' '),
			"b2": "F2 z2".split(' '),
			"d2": "U2 y2".split(' '),
		}

		def rot_x():
			global up, front, down, back
			temp = up
			up = front
			front = down
			down = back
			back = temp

		def rot_y():
			global front, right, back, left
			temp = front
			front = right
			right = back
			back = left
			left = temp

		def rot_z():
			global up, left, down, right
			temp = up
			up = left
			left = down
			down = right
			right = temp

		moves = ["x2"] + line.strip().split(' ')
		new_moves = []
		for move in moves:
			if move in slice_subs:
				new_moves += slice_subs[move]
			elif move in wide_subs:
				new_moves += wide_subs[move]
			else:
				new_moves.append(move)
		
		moves = new_moves[:]
		new_moves = []
		for move in moves:
			table = {
				'U': up,
				'L': left,
				'F': front,
				'R': right,
				'B': back,
				'D': down,
			}
			if move[0] in table:
				new_moves.append(table[move[0]] + move[1:])
				continue
			deg = 1
			if len(move) == 2:
				if '2' in move:
					deg = 2
				else:
					deg = 3
			for _ in range(deg):
				if 'x' in move: rot_x()
				if 'y' in move: rot_y()
				if 'z' in move: rot_z()
		
		for i, m in enumerate(new_moves):
			m = m.replace("'", 'p')
			new_moves[i] = 'MOVE_' + m
		new_moves.append('MOVE_END')
		print('{', ', '.join(new_moves), '},')