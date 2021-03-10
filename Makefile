# Matthew Flynn and Andrew Suh
# CSF Assignment #3

CFLAGS = -std=c++11 -Wall -Wextra --pedantic

csim : main.o
	g++ -o $@ main.o

main.o : main.cpp main.h
	g++ $(CFLAGS) -c main.cpp

clean :
	rm -f *.o csim