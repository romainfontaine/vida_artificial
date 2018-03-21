headers	    := $(wildcard *.h)

main: main.cpp $(headers)
	g++ main.cpp -lglut -lGLU -lGL -lm -std=c++11 -o main

run: main
	./main

clean:
	rm -f main
