#ifndef ANIMAL_H
#define ANIMAL_H
#include <thread>
#include <atomic>
#include <memory>
#include "ReactionDiffusion.h"

class Animal {
protected:
    const std::vector<Point> *shape;
    double x, y, vx, vy, vmax, xscale, yscale, perspective;
    int foodStock, metabolism, age, agelimit;
    int skin_xinit, skin_yinit, skin_radius;
    ReactionDiffusion react;
    std::shared_ptr<std::thread> t;
    std::shared_ptr<std::atomic<bool>> done;
    std::shared_ptr<std::atomic<bool>> stop;
public:

    Animal(const std::vector<Point> *shape, const double &x = 0, const double &y = 0,
            const int &foodStock = 200, const int &metabolism = 1,
            const int &agelimit = 1000, const double &vmax = .005,
            const double &xscale = .15, const double&yscale = .15, const double &perspective = 0.,
            const int &skin_xinit = 20, const int &skin_yinit = 20, const int &skin_radius = 2)
    : shape(shape), x(x), y(y), vx(0), vy(0), vmax(vmax), xscale(xscale), yscale(yscale),
    perspective(perspective), foodStock(foodStock), metabolism(metabolism), age(0), agelimit(agelimit),
    skin_xinit(skin_xinit), skin_yinit(skin_yinit), skin_radius(skin_radius) {
        done = std::shared_ptr < std::atomic<bool>>(new std::atomic<bool>(false));
        stop = std::shared_ptr < std::atomic<bool>>(new std::atomic<bool>(false));

    }

    static Animal individual(const std::vector<Point> *shape) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis_meta(1, 3);
        std::normal_distribution<> dis_age(1000, 100);
        std::uniform_real_distribution<> dis_speed(.001, .006);
        std::uniform_real_distribution<> dis_scale(.075, .12);
        std::uniform_real_distribution<> dis_persp(-.15, .15);
        std::normal_distribution<> dis_skin_pos(0, ReactionDiffusion::size);
        return Animal(shape, 0, 0, 200, dis_meta(gen), dis_age(gen), dis_speed(gen), dis_scale(gen),
                dis_scale(gen), dis_persp(gen), dis_skin_pos(gen), dis_skin_pos(gen));
    }

    const std::vector<Point> * getShape() const {
        return shape;
    }

    void kill() {
        *stop = true;
        t->join();
    }
protected:

    void genTexture() {
        t = std::shared_ptr<std::thread>(new std::thread(&Animal::gen, this));
    }

    void gen() {

        for (int i = 0; i < 5000 && !*stop; i++)
            react.iterate();
        if (!*stop)
            react.generateUCharArray();
        *done = true;
    }

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

        if (*done) {
            const unsigned char* texDat = react.toUCharArray();
            int tw = react.getSize(), th = react.getSize();
            // Source: https://stackoverflow.com/a/24266568/4384857
            //upload to GPU texture
            GLuint tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tw, th, 0, GL_RGB, GL_UNSIGNED_BYTE, texDat);
            glBindTexture(GL_TEXTURE_2D, 0);

            //clear and draw quad with texture (could be in display callback)
            glBindTexture(GL_TEXTURE_2D, tex);
            glEnable(GL_TEXTURE_2D);
            float h = .05, w = .05;
            glColor4f(1.0, 1.0, 1.0, 1.0);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2f(x + w / 2, y - h / 2);
            glTexCoord2f(0, 1);
            glVertex2f(x + w / 2, y + h - h / 2);
            glTexCoord2f(1, 1);
            glVertex2f(x + w + w / 2, y + h - h / 2);
            glTexCoord2f(1, 0);
            glVertex2f(x + w + w / 2, y - h / 2);
            glEnd();
            /*
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_POLYGON);
            for (const Point &p : *shape) {
                glVertex2f(x + p.x * xscale, y + (p.y + sign(p.y)*(exp(p.x * perspective) - 1)) * yscale);
                glTexCoord2f(p.x, .5 + p.y);
            }
            glEnd();
             */

            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            glDeleteTextures(1, &tex);

        }
        glColor3d(1., 1., 1.);
        glBegin(GL_LINE_LOOP);
        for (const Point &p : *shape)
            glVertex3d(x + p.x * xscale, y + (p.y + sign(p.y)*(exp(p.x * perspective) - 1)) * yscale, p.z);
        glEnd();
        int timeRemaining = age/(float)agelimit*100;
        displayText(x, y-.075, 1, 1, 1, std::to_string(timeRemaining)+"%");
        int ttl = foodStock/(float)metabolism;
        displayText(x+.75*yscale, y-.075, 1, 1, 1, std::to_string(ttl));
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

