raycaster : raycaster.o scene.o
	cc -o raycaster raycaster.o scene.o -Wall -lm -g
	rm raycaster.o scene.o

raycaster.o : raycaster.c scene.h
	cc -c raycaster.c

scene.o : scene.c scene.h
	cc -c scene.c
