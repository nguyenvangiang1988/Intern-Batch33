Compiler=g++
Obj=ex0.o ex1.o ex2_1.o ex2_2.o ex3_1.o ex3_2.o ex3_3.o ex6_1.o ex6_2.o ex6_3.o ex6_4.o ex7_1.o ex7_2.o ex7_3.o ex7_4.o main.o
Deps=header.h

%.o: %.cpp $(Deps)
	$(Compiler) -c -o $@ $< 

main: $(Obj)
	$(Compiler) -o $@ $^ 
 
.PHONY: clean
clean:
	rm -f *.o
