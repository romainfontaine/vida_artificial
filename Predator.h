#ifndef PREDATOR_H
#define PREDATOR_H

class Predator : public Animal {
    double vision;
    static const double SQUARED_DIST_EAT;
public:

    Predator(const Animal &an, const double &vision = .7)
    : Animal(an), vision(vision) {
    }

    void eatFood(std::vector<Boid> &boids) {
        for (unsigned int i = 0; i < boids.size(); i++) {
            if (squaredTorusDistance(boids[i]) < SQUARED_DIST_EAT) {
                boids.erase(boids.begin() + i);
                break;
            }
        }
    }

    void update(const std::vector<Boid> &boids) {
        std::pair<double, double> c = moveToFood(boids);
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
            return std::make_pair(dx > 1 ? dx - 2 : dx, dy > 1 ? dy - 2 : dy);
        }
    }
};

const double Predator::SQUARED_DIST_EAT = 0.01;

#endif /* PREDATOR_H */

