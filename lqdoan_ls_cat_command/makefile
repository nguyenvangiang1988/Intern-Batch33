CC=gcc
Obj=read.c process.c main.c 
Dep=header.h

%.o: %.c $(Dep)
	$(CC) -o $@ $<
main: $(Obj)
	$(CC) -o $@ $^ -g
.PHONY: clean
clean:
	rm -f *.o
