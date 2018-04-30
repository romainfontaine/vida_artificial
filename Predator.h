#ifndef PREDATOR_H
#define PREDATOR_H

class Predator : public Animal {
    double vision;
public:

    Predator(const Animal &an, const double &vision = .7)
    : Animal(an), vision(vision) {
    }

    void eatFood(const std::vector<Boid> &boids) {
        boids[0];
    }

    void update(const std::vector<Boid> &boids) {
        std::pair<double, double> c = moveToFood(boids);

        vx += c.first;
        vy += c.second;
        updatePosition_NormalizeSpeed();
        eatFood(boids);
    }

    std::pair<double, double> moveToFood(const std::vector<Boid> &boids) {
        if (boids.size() == 0) {
            return std::make_pair(.0, .0);
        }
        double minDist = squaredTorusDistance(boids[0]);
        int minIdx = 0;
        for (unsigned int i = 1; i < boids.size(); i++) {
            double d = squaredTorusDistance(boids[i]);
            if (d <= minDist) {
                minDist = d;
                minIdx = i;
            }
        }
        if (minDist > vision)
            return std::make_pair(.0, .0);
        else {
            std::pair<double, double> p = boids[minIdx].getPosition();
            double dx = p.first - x;
            double dy = p.second - y;
            return std::make_pair(dx>1?dx-2:dx, dy>1?dy-2:dy);
        }
    }


};


#endif /* PREDATOR_H */

