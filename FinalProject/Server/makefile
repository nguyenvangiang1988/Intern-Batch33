CC=gcc
Obj=VTS.c
Dep=VTSHeader.h

%.o: %.c $(Dep)
	$(CC) -o -pthread $@ $<
server: $(Obj)
	$(CC) -o $@ $^ -g -pthread
.PHONY: clean
clean:
	rm -f *.o
