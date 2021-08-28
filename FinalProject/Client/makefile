CC=gcc
Obj=Client.c
Dep=ClientHeader.h

%.o: %.c $(Dep)
	$(CC) -o -pthread $@ $<
client: $(Obj)
	$(CC) -o $@ $^ -g -pthread
.PHONY: clean
clean:
	rm -f *.o
