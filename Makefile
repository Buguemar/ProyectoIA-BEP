flags = -Wno-deprecated -g -ggdb 
comp = g++
prog  = IA

#El problema
f_in = InstanciasBEP/InstanceBEP-1-4-2-4.txt
f_out = InstanciasBEPOut/InstanceBEP-1-4-2-4.txt

comp:$(prog)

exe:$(prog)
	#valgrind --leak-check=full --show-reachable=yes 
	./$(prog) $(f_in) $(f_out) 

$(prog): forward.o
	$(comp) forward.o -o $(prog) $(flags)

main.o: forward.cpp includes.h forward.h
	$(comp) -c forward.cpp $(flags)

clean:
	rm -f *~
	rm -f *.o
	rm -f *.txt
	rm -f *.ps
	rm -f $(prog)
	rm -f core*

