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
        vx = std::min(vmax, std::max(-vmax, vx));
        vy = std::min(vmax, std::max(-vmax, vy));
        x += vx;
        y += vy;
        if (x<-1 || x > 1) {
            x = fmod(x + 3, 2) - 1;
        }
        if (y<-1 || y > 1) {
            y = fmod(y + 3, 2) - 1;
        }
        eatFood(boids);
    }

    std::pair<double, double> moveToFood(const std::vector<Boid> &boids) {
        /*double minDist = squaredTorusDistance(boids[0]);
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
            auto p = boids[minIdx].getPosition();
            return std::make_pair(p.first - x, p.second.y);

        }*/
        boids[0];
        return std::make_pair(.1, .1);

    }


};


#endif /* PREDATOR_H */

