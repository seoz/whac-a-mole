all:
	edje_cc sample.edc
	gcc sample.c -o sample `pkg-config --cflags --libs elementary`
