#ifndef PREY_H
#define PREY_H

#include "Animal.h"
#include "Food.h"
#include "ReactionDiffusion.h"

class Predator;

class Prey : public Animal {
    // Source for basic boid rules: http://www.kfish.org/boids/pseudocode.html
    static int ID_COUNT;
    static const double SQUARED_DIST_SEPARATION;
protected:
    int id, vision_int;
    Food* food;
public:

    Food* getFood() {
        return food;
    }

    Prey(Food* f, const Animal &an)
    : Animal(an),
    id(ID_COUNT++), vision_int(vision * 50), food(f) {
        react = ReactionDiffusion(0.0545, 0.062);
        react.initReact(skin_xinit, skin_yinit, skin_radius);
        genTexture();
    }

    friend std::ostream& operator<<(std::ostream& s, const Prey& b) {
        return s << "Prey #" << b.id << " | pos={" << b.x << "," << b.y <<
                "} vmax=" << b.vmax << " xscale=" << b.xscale << " yscale=" <<
                b.yscale << " perspective=" << b.perspective << " id=" << b.id <<
                " foodStock=" << b.foodStock << " metabolism=" << b.metabolism <<
                " age=" << b.age << " agelimit=" << b.agelimit << " vision=" <<
                b.vision;
    }

    static Prey* individual(Food* f, const std::vector<Point> *shape) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis_vision_c(.1, .25);
        Animal an = Animal::individual(shape);
        an.setVision(dis_vision_c(gen));
        return new Prey(f, an);
    }

    void update(const std::vector<Prey*> &preys,
            const std::vector<Predator*> &predators);

    unsigned int getFoodStock() {
        return foodStock;
    }

private:

    bool escapePredator(const std::vector<Predator*> &predators,
            std::pair<double, double> &newDirection) const;

    void eatFood();

    std::pair<double, double> cohesion(const std::vector<Prey*> &preys) const;

    std::pair<double, double> separation(const std::vector<Prey*> &preys) const;

    std::pair<double, double> alignment(const std::vector<Prey*> &preys) const;

    std::pair<double, double> foodMove() const;
};

#endif /* PREY_H */

