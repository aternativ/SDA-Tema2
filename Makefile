build: search_index

search_index: main.o functions.o list.o word.o heap.o multitree.o
	gcc -g main.o functions.o heap.o list.o multitree.o word.o -o search_index

main.o: main.c word.h functions.h system.h
	gcc -g -c main.c -o main.o

functions.o: functions.c system.h list.h word.h helper.h multitree.h heap.h 
	gcc -g -c functions.c -o functions.o

list.o: list.c list.h helper.h
	gcc -g -c list.c -o list.o

word.o: word.c word.h system.h
	gcc -g -c word.c -o word.o

heap.o: heap.c heap.h helper.h
	gcc -g -c heap.c -o heap.o

multitree.o: multitree.c list.h helper.h multitree.h word.h
	gcc -g -c multitree.c -o multitree.o

clean:
	rm *.o search_index
