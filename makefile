GENERATE_CMD = -O3 -pedantic -Wall -Wextra -Werror -lglut -lGLU -lGL -lm -std=c++11
HEADERS	    := $(wildcard *.h)
SOURCE	    := $(wildcard *.cc)
OBJ = $(SOURCE:cc=o)
	
main: $(OBJ)
	g++ $^ -o $@ $(GENERATE_CMD) 
	
Boid.o: Tools.h Boid.h Predator.h
Main.o: Tools.h TurtleInterpreter.h LSystem.h Plant.h Boid.h Predator.h ReactionDiffusion.h


%.o: %.cc
	g++ $< -c $(GENERATE_CMD) 

	

run: main
	./main

clean:
	rm -f main *.o
