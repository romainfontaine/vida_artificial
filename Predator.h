#ifndef PREDATOR_H
#define PREDATOR_H

#include "ReactionDiffusion.h"
#include "Animal.h"
#include "Prey.h"

class Predator : public Animal {
    static const double SQUARED_DIST_EAT;
public:

    explicit Predator(const Animal &an)
    : Animal(an) {
        react = ReactionDiffusion(0.034, 0.0618);
        react.initReact(skin_xinit, skin_yinit, skin_radius);
        genTexture();
    }

    void eatFood(std::vector<Prey*> &preys) {
        for (unsigned int i = 0; i < preys.size() && foodStock < FOOD_CAPACITY; i++) {
            if (squaredTorusDistance(*preys[i]) < SQUARED_DIST_EAT) {
                foodStock += preys[i]->getFoodStock();
                preys[i]->kill();
                //Food* f = preys[i]->getFood();
                //auto s = preys[i]->getShape();
                delete preys[i];

                preys.erase(preys.begin() + i);
                /*std::random_device rd;
                std::default_random_engine re(rd());
                std::uniform_real_distribution<double> unif(-1, 1);
                preys[i] = Prey::individual(f, s);
                preys[i]->setPosition(unif(re), unif(re));
                //preys.erase(preys.begin() + i);
                 */
                break;
            }
        }
    }

    Predator* crossoverMutation(const Predator &o) const {
        Animal a = Animal::crossoverMutation(o);
        return new Predator(a);
    }

    static Predator* individual(const std::vector<Point> *shape) {
        return new Predator(Animal::individual(shape));
    }

    void update(const std::vector<Prey*> &preys, const std::vector<Predator*> &preds) {
        std::pair<double, double> c = foodStock / (float) metabolism <= 300 ? moveToFood(preys) : std::pair<double, double>{0, 0};
        const double threshold = .0000000001;
        if (std::abs(c.first) < threshold && std::abs(c.second) < threshold &&
                std::abs(vx) < threshold && std::abs(vy) < threshold) {
            std::random_device rd;
            std::default_random_engine re(rd());
            std::uniform_real_distribution<double> unif(-10, 10);
            vx = unif(re);
            vy = unif(re);
        } else {
            auto s = separation(preds);
            vx += c.first + s.first;
            vy += c.second + s.second;
        }
        updatePosition_NormalizeSpeed();
    }

    std::pair<double, double> moveToFood(const std::vector<Prey*> &preys) {
        if (preys.size() == 0) {
            return std::make_pair(.0, .0);
        }
        double minDist = squaredTorusDistance(*preys[0]);
        int minIdx = 0;
        for (unsigned int i = 1; i < preys.size(); i++) {
            double d = squaredTorusDistance(*preys[i]);
            if (d <= minDist) {
                minDist = d;
                minIdx = i;
            }
        }
        if (minDist > vision)
            return std::make_pair(.0, .0);
        else {
            std::pair<double, double> p = preys[minIdx]->getPosition();
            double dx = p.first - x;
            double dy = p.second - y;
            return std::make_pair(dx > 1 ? dx - 2 : dx, dy > 1 ? dy - 2 : dy);
        }
    }
};


#endif /* PREDATOR_H */

