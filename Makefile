all:
	edje_cc mole.edc
	gcc mole.c -o mole `pkg-config --cflags --libs elementary`
