#ifndef TURTLE_H
#define TURTLE_H

#include <random>

class Turtle {
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;
    
    double x;
    double y;
    double a;
    double d;
    double delta;

    void forward() {
        double noise = distribution(generator)*0.5;
        this->x = this->x + (this->d + noise) * cos(this->a);
        this->y = this->y + (this->d + noise) * sin(this->a);
    }

public:
    // Delta in degrees
    Turtle(const double &d = 1, const double &delta = 90, const double x = 0, const double y = 0) : x(x), y(y), a(M_PI / 2), d(d), delta(delta*M_PI / 180), distribution(0, 0.0005) {
    };

    void f() {
        this->forward();
    }

    void F() {
        glVertex2d(this->x, this->y);
        this->forward();
        glVertex2d(this->x, this->y);
    }

    void plus() {
        this->a += this->delta + distribution(generator)*25;
    }

    void minus() {
        this->a -= this->delta + distribution(generator)*25;
    }
};

#endif /* TURTLE_H */

