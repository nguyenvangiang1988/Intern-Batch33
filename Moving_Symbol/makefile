CC=gcc
Obj=myConio.c movingSymbol.c
Dep=myConio.h

%.o: %.c $(Dep)
	$(CC) -o -pthread $@ $<
main: $(Obj)
	$(CC) -o $@ $^ -g -pthread
.PHONY: clean
clean:
	rm -f *.o
