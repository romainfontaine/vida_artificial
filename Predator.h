#ifndef PREDATOR_H
#define PREDATOR_H

#include "ReactionDiffusion.h"
#include "Animal.h"
#include "Prey.h"

class Predator : public Animal {
    double vision;
    static const double SQUARED_DIST_EAT;
public:

    Predator(const Animal &an, const double &vision = .7)
    : Animal(an), vision(vision) {
        react = ReactionDiffusion(0.034, 0.0618);
        react.initReact(skin_xinit, skin_yinit, skin_radius);
        genTexture();
    }

    void eatFood(std::vector<Prey*> &preys) {
        for (unsigned int i = 0; i < preys.size(); i++) {
            if (squaredTorusDistance(*preys[i]) < SQUARED_DIST_EAT) {
                foodStock += preys[i]->getFoodStock();
                preys[i]->kill();
                Food* f = preys[i]->getFood();
                auto s = preys[i]->getShape();
                delete preys[i];

                std::random_device rd;
                std::default_random_engine re(rd());
                std::uniform_real_distribution<double> unif(-1, 1);
                preys[i] = Prey::individual(f, s);
                preys[i]->setPosition(unif(re), unif(re));
                //preys.erase(preys.begin() + i);
                break;
            }
        }
    }

    static Predator* individual(const std::vector<Point> *shape) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis_vision(.5, .7);

        Animal an = Animal::individual(shape);
        return new Predator(an, dis_vision(gen));
    }

    void update(const std::vector<Prey*> &preys) {
        std::pair<double, double> c = moveToFood(preys);
        const double threshold = .0000000001;
        if (std::abs(c.first) < threshold && std::abs(c.second) < threshold &&
                std::abs(vx) < threshold && std::abs(vy) < threshold) {
            std::random_device rd;
            std::default_random_engine re(rd());
            std::uniform_real_distribution<double> unif(-10, 10);
            vx = unif(re);
            vy = unif(re);
        } else {
            vx += c.first;
            vy += c.second;
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

