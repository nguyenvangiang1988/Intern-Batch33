CC=gcc
Obj=readFile.c mulMat.c
Dep=readFile.h

%.o: %.c $(Dep)
	$(CC) -o -pthread $@ $<
main: $(Obj)
	$(CC) -o $@ $^ -g -pthread
.PHONY: clean
clean:
	rm -f *.o
