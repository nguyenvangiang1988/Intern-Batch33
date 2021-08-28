CC=gcc
Obj=LocGen.c
Dep=LocGenHeader.h

%.o: %.c $(Dep)
	$(CC) -o -pthread $@ $<
locgen: $(Obj)
	$(CC) -o $@ $^ -g -pthread
.PHONY: clean
clean:
	rm -f *.o
