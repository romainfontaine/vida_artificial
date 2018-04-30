GENERATE_CMD = -O3 -pedantic -Wall -Wextra -Werror -lglut -lGLU -lGL -lm -std=c++11
HEADERS	    := $(wildcard *.h)
SOURCE	    := $(wildcard *.cc)
OBJ = $(SOURCE:cc=o)
	
main: $(OBJ)
	g++ $^ -o $@ $(GENERATE_CMD) 
	
Prey.o: Tools.h Animal.h Prey.h Predator.h ReactionDiffusion.h
Main.o: Tools.h Animal.h TurtleInterpreter.h LSystem.h Plant.h Prey.h Predator.h ReactionDiffusion.h


%.o: %.cc
	g++ $< -c $(GENERATE_CMD) 

	

run: main
	./main

clean:
	rm -f main *.o
