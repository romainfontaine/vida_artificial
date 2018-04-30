#ifndef ANIMAL_H
#define ANIMAL_H

class Animal {
protected:
    const std::vector<Point> *shape;
    double x, y, vx, vy, vmax, xscale, yscale, perspective;
    int foodStock, metabolism, age, agelimit;
public:

    Animal(const std::vector<Point> *shape, const double &x = 0, const double &y = 0,
            const int &foodStock = 200, const int &metabolism = 1,
            const int &agelimit = 1000, const double &vmax = .005,
            const double &xscale = .15, const double&yscale = .15, const double &perspective = 0.)
    : shape(shape), x(x), y(y), vx(0), vy(0), vmax(vmax), xscale(xscale), yscale(yscale),
    perspective(perspective), foodStock(foodStock), metabolism(metabolism), age(0), agelimit(agelimit) {

    }
protected:

    double squaredTorusDistance(const Animal &a) const {
        return squaredTorusDistance(2, x, y, a.x, a.y);
    }

    static double squaredTorusDistance(const double &size, const double &fx, const double &fy, const double &nx, const double &ny) {
        // Source: https://stackoverflow.com/q/2123947/4384857
        double a = std::min(std::abs(fx - nx), size - std::abs(fx - nx));
        double x = std::min(std::abs(fy - ny), size - std::abs(fy - ny));
        return a * a + x * x;
    }

    void updatePosition_NormalizeSpeed() {
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
    }


public:

    void setPosition(const double &x, const double &y) {
        this->x = x;
        this->y = y;
    }

    std::pair<double, double> getPosition() const {
        return std::make_pair(x, y);
    }

    void draw() const {
        const double x = this->x - 0.5 * xscale; // Center
        glColor3d(1., 1., 1.);
        glBegin(GL_LINE_LOOP);
        for (const Point &p : *shape)
            glVertex3d(x + p.x * xscale, y + (p.y + sign(p.y)*(exp(p.x * perspective) - 1)) * yscale, p.z);
        glEnd();
    }

    bool consumeEnergy() {
        return ((foodStock -= metabolism) > 0) && (age++<agelimit);
    }
private:

    static double sign(double d) {
        if (d > .05)
            return 1;
        if (d<-.05)
            return -1;
        return 0;
    }
};

#endif /* ANIMAL_H */

