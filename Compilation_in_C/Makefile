exe: src/tree.o src/stack.o
	gcc $(CFLAGS) src/tree.o src/stack.o -o exe
src/tree.o: src/tree.c include/tree.h include/stack.h
	gcc $(CFLAGS) -c -I include/ src/tree.c -o src/tree.o
src/stack.o: src/stack.c include/stack.h include/tree.h
	gcc $(CFLAGS) -c -I include/ src/stack.c -o src/stack.o
clean:
	rm src/tree.o src/stack.o exe

