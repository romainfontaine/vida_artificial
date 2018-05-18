GENERATE_CMD = -O3 -lpthread -pedantic -Wall -Wextra -Werror -lglut -lGLU -lGL -lm -std=c++17
HEADERS	    := $(wildcard *.h)
SOURCE	    := $(wildcard *.cc)
OBJ = $(SOURCE:cc=o)
COMPILER = g++-6
	
main: $(OBJ)
	$(COMPILER) $^ -o $@ $(GENERATE_CMD) 
	
Prey.o: Food.h Tools.h Animal.h Prey.h Predator.h ReactionDiffusion.h
Main.o: Stats.h Food.h Tools.h Animal.h TurtleInterpreter.h LSystem.h Plant.h Prey.h Predator.h ReactionDiffusion.h


%.o: %.cc
	$(COMPILER) $< -c $(GENERATE_CMD) 

	

run: main
	./main

clean:
	rm -f main *.o
