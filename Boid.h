#ifndef BOID_H
#define BOID_H

#include "Animal.h"
#include "Food.h"

class Predator;

class Boid : public Animal {
    // Source for basic boid rules: http://www.kfish.org/boids/pseudocode.html
    static int ID_COUNT;
    static const double SQUARED_DIST_SEPARATION;
protected:
    int id, vision;
    Food* food;
public:

    Boid(Food* f, const Animal &an, const int &vision = 10)
    : Animal(an),
    id(ID_COUNT++), vision(vision), food(f) {
    }

    friend std::ostream& operator<<(std::ostream& s, const Boid& b) {
        return s << "Boid #" << b.id << " | pos={" << b.x << "," << b.y <<
                "} vmax=" << b.vmax << " xscale=" << b.xscale << " yscale=" <<
                b.yscale << " perspective=" << b.perspective << " id=" << b.id <<
                " foodStock=" << b.foodStock << " metabolism=" << b.metabolism <<
                " age=" << b.age << " agelimit=" << b.agelimit << " vision=" <<
                b.vision;
    }

    static Boid individual(Food* f, const std::vector<Point> *shape) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis_vision(5, 15);

        Animal an = Animal::individual(shape);
        return Boid(f, an, dis_vision(gen));
    }

    void update(const std::vector<Boid> &boids,
            const std::vector<Predator> &predators);


private:

    bool escapePredator(const std::vector<Predator> &predators,
            std::pair<double, double> &newDirection) const;

    void eatFood();

    std::pair<double, double> cohesion(const std::vector<Boid> &boids) const;

    std::pair<double, double> separation(const std::vector<Boid> &boids) const;

    std::pair<double, double> alignment(const std::vector<Boid> &boids) const;

    std::pair<double, double> foodMove() const;
};

#endif /* BOID_H */

