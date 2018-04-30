#ifndef TURTLE_H
#define TURTLE_H

class Turtle {
    double d;
    double delta;
    double x;
    double y;
    double a;
    double noise_angle;
    double noise_distance;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;

    void forward() {
        double noise = distribution(generator) * noise_distance;
        this->x = this->x + (this->d + noise) * cos(this->a);
        this->y = this->y + (this->d + noise) * sin(this->a);
    }

public:
    Turtle& operator=(const Turtle& other);

    Turtle(const Turtle &t) {
        x = t.x;
        y = t.y;
        a = t.a;
        d = t.d;
        delta = t.delta;
        noise_angle = t.noise_angle;
        noise_distance = t.noise_distance;
        generator = std::default_random_engine(std::random_device()());
        distribution = t.distribution;

    }

    // Angles in degrees

    Turtle(const double &d = 1, const double &delta = 90,
            const double &x = 0, const double &y = 0,
            const double &noise_angle = 0, const double &noise_distance = 0)
    : d(d), delta(delta*M_PI / 180), x(x), y(y), a(M_PI / 2),
    noise_angle(noise_angle*M_PI / 180),
    noise_distance(noise_distance*M_PI / 180), distribution(0, .1) {
        generator = std::default_random_engine(std::random_device()());
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
        this->a += this->delta + distribution(generator) * noise_angle;
    }

    void minus() {
        this->a -= this->delta + distribution(generator) * noise_angle;
    }
};

#endif /* TURTLE_H */

