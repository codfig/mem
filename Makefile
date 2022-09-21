
mem: mem.c
	gcc -o mem $$(pkg-config --cflags --libs cairo)  mem.c

pdf: mem
	./mem

