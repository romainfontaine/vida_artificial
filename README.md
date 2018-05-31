# Artificial Life - Final project
## Introduction
This project is part of the Artificial Life course at the [National University of Colombia](http://unal.edu.co/), taught by [Jonatan Gómez](http://dis.unal.edu.co/~jgomezpe/).

This project has been implemented with C++ and OpenGL. The environment is toroidal, and populated with two spieces of fish, which are predators and preys. Preys gather energy by eating food growing close to plants. Predators gather energy by eating preys. Both species have properties, determined by their genetic code. These properties are the metabolism (energy-consumption rate), vision range, maximum speed, among others. They also have the ability to reproduce, leading to open-ended evolution.

## Plants
Three types of plants are present in the environment, which are drawn thanks to Lindenmayer Systems. Some are deterministic and some are stochastic. An illusion of movement is obtained by adding small variations (noise) in the angle and length of each section.

## Food Generation
The food is generated with the Sandpile Model, inside a grid of size 200*200. The capacity of each cell is defined at the beginning. There are three seasons, and the food grows only in one place at a time, given the current season. When an animal dies of age, it leaves its ressources in the form of a sandpile as well.

## Animals
### Shape
The shape of the animals is given by a list of points, defining their outline.
Non-lineal transformations are then applied to these points according to the genetic code of the animal.

### Skin
The skins of the animals are generated with a Reaction-Diffusion model. The texture has a shape of 40*40 pixels, and 5000 iterations are needed for convergence. The initial pattern is determined by the genetic code of the animal. In order for the simulation to be smoother, the skins are generated in separate threads.

### Genetic Code
The genetic code of the animals is composed of:
- Vision range
- Maximum velocity
- Non-lineal transformation parameters
- Skin-generation parameters
- Metabolism
- Maximum age
- Gender

### Preys' Behaviors
The behaviors of the preys is an extension of the Boid Algorithm, with its three basic components:
- Cohension
- Alignment
- Separation
Moreover, preys have a tendency to move towards food and to escape predators located in their vision range. When they come in contact with food sites, they eat it, increasing their ressources and emptying the food sides.

### Predators' Behaviors
The predators move towards the preys in their range of vision if they are hungry, and move in a random way otherwise. When they eat a prey, the prey's ressources are added to their own.

### Reproduction
The attributes are continuous values; crossover is performed by doing a linear combination of the parents attributes with a random variable in the interval [0;1]. Mutations occur for each attribute given a Normal law of probability.

The following restrictions apply for reproduction:
- The parents' skins must be sufficiently similar for them to reproduce.
- The parents must be old enough to be able to reproduce.
- The parents must be in each others range of vision
- The parents must have enough ressources to produce an offspring and transmit ressources. Both parents contribute to this cost proportionally to their current ressources.

## Emerging behaviors
- Migration of the animals according to the season
- Open-ended (co-)evolution through natural selection. The fittest individuals survive longer and reproduce more, therefore allowing to increase the maximum speed, the vision range and the maximum age through generations. The metabolism also tends to decrease with time.
- Sub-species can appear because of the skin similarity required for reproduction.
- Peaks and crashes in the population size often occur, species may go extinct.

