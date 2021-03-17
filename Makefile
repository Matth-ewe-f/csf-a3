# Matthew Flynn and Andrew Suh
# CSF Assignment #3

CFLAGS = -std=c++11 -Wall -Wextra --pedantic
CPP_SRCS = main.cpp Cache.cpp CacheBlock.cpp

csim : main.o Cache.o CacheBlock.o
	g++ -o $@ main.o Cache.o CacheBlock.o

main.o : main.cpp main.h
	g++ $(CFLAGS) -c main.cpp

Cache.o : Cache.cpp Cache.h
	g++ $(CFLAGS) -c Cache.cpp

CacheBlock.o : CacheBlock.cpp CacheBlock.h
	g++ $(CFLAGS) -c CacheBlock.cpp

clean :
	rm -f *.o csim depend.mak

depend.mak :
	touch $@

depend :
	gcc -M $(CFLAGS) $(CPP_SRCS) >> depend.mak

include depend.mak