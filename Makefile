main: src/main.c src/operands.c
	cc -o bin/minesweeper src/main.c src/operands.c

test: main
	./bin/minesweeper