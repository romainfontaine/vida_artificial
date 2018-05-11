#ifndef ANIMAL_H
#define ANIMAL_H
#include <thread>
#include <atomic>
#include <memory>
#include "ReactionDiffusion.h"
#include "Tools.h"
#include "Food.h"

class Animal {
protected:
    static const double SQUARED_DIST_SEPARATION;
    static const int INIT_FOOD_AMOUNT;
    const std::vector<Point> *shape;
    double x, y, vx, vy, vmax, xscale, yscale, perspective, vision;
    int foodStock, metabolism, age, agelimit;
    int skin_xinit, skin_yinit, skin_radius;
    bool sex;
    ReactionDiffusion react;
    std::shared_ptr<std::thread> t;
    std::shared_ptr<std::atomic<bool>> done;
    std::shared_ptr<std::atomic<bool>> stop;
    std::map<Animal*, bool> can_reproduce;
public:

    bool compareSkin(Animal* a) {
        if (!done || !a->done)
            return false;
        if (can_reproduce.find(a) != can_reproduce.end()) {
            return can_reproduce[a];
        }
        double diff = 0;
        for (int i = 0; i < ReactionDiffusion::size; i++) {
            for (int j = 0; j < ReactionDiffusion::size; j++) {
                diff += std::abs(react.getV(i, j) - a->react.getV(i, j));
            }
        }
        std::cout << diff << std::endl;
        bool v = diff < 200.;
        return (can_reproduce[a] = v);
    }


    static bool debug;
    static bool debug_fov;
    static bool big_textures;

    Animal(const std::vector<Point> *shape, const double &x = 0, const double &y = 0,
            const int &foodStock = INIT_FOOD_AMOUNT, const int &metabolism = 1,
            const int &agelimit = 1000, const double &vmax = .005,
            const double &xscale = .15, const double&yscale = .15, const double &perspective = 0.,
            const double &vision = .7,
            const int &skin_xinit = 20, const int &skin_yinit = 20, const int &skin_radius = 2,
            const bool &sex = true)
    : shape(shape), x(x), y(y), vx(0), vy(0), vmax(clamp(vmax, .001, .006)),
    xscale(clamp(xscale, .075, .12)), yscale(clamp(yscale, .075, .12)),
    perspective(clamp(perspective, -.15, .15)), vision(clamp(vision, .5, .7)),
    foodStock(foodStock), metabolism(clamp(metabolism, 1, 2)), age(0),
    agelimit(clamp(agelimit, 1500, 2500)),
    skin_xinit(clamp(skin_xinit, 0, ReactionDiffusion::size)), skin_yinit(clamp(skin_yinit, 0, ReactionDiffusion::size)),
    skin_radius(clamp(skin_radius, 1, 2)), sex(sex),
    done(new std::atomic<bool>(false)), stop(new std::atomic<bool>(false)) {
    }

    friend std::ostream& operator<<(std::ostream& s, const Animal& b) {
        return s << "Animal" << // " | pos=" << b.x << "," << b.y <<
                " vmax=" << b.vmax << " xscale=" << b.xscale << " yscale=" <<
                b.yscale << " perspective=" << b.perspective <<
                " foodStock=" << b.foodStock << " metabolism=" << b.metabolism <<
                " age=" << b.age << " agelimit=" << b.agelimit << " vision=" <<
                b.vision << " skin_xinit=" << b.skin_xinit;
    }

    static Animal individual(const std::vector<Point> *shape) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis_meta(1, 2);
        std::uniform_int_distribution<> dis_bool(0, 1);
        std::normal_distribution<> dis_age(2000, 300);
        std::uniform_real_distribution<> dis_speed(.001, .006);
        std::uniform_real_distribution<> dis_scale(.075, .12);
        std::uniform_real_distribution<> dis_persp(-.15, .15);
        std::normal_distribution<> dis_skin_pos(0, ReactionDiffusion::size);
        std::uniform_real_distribution<> dis_vision(.5, .7);
        return Animal(shape, 0, 0, INIT_FOOD_AMOUNT, dis_meta(gen), dis_age(gen), dis_speed(gen), dis_scale(gen),
                dis_scale(gen), dis_persp(gen), dis_vision(gen), dis_skin_pos(gen), dis_skin_pos(gen), dis_meta(gen), dis_bool(gen) == 1);
    }

    template<class T>
    void reproduce(std::vector<T*> &preys) {
        const int minTTLToReproduce = 500;
        const int reproduceCost = 170;
        const int min_reprod_age = 200;
        if (foodStock / metabolism < minTTLToReproduce || age < min_reprod_age)
            return;
        for (T *p : preys) {
            if (sex != p->sex && p->age >= min_reprod_age &&
                    p->foodStock / p->metabolism >= minTTLToReproduce &&
                    squaredTorusDistance(*p) <= vision * vision && compareSkin(p)) {
                preys.push_back((T*) ((T*)this)->T::crossoverMutation(*p));
                double nx = (p->x - x) / 2 + p->x;
                double ny = (p->y - y) / 2 + p->y;
                preys.back()->setPosition(nx, ny);
                std::cout << *preys.back() << std::endl;
                foodStock -= reproduceCost;
                p->foodStock -= reproduceCost;
            }
        }
    }

    Animal crossoverMutation(const Animal &o) const {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> mut(0, 1);
        std::uniform_real_distribution<> dis_crossover(0, 1);
        std::uniform_int_distribution<> dis_bool(0, 1);

        float b_meta = dis_crossover(gen),
                b_age = dis_crossover(gen),
                b_speed = dis_crossover(gen),
                b_xscale = dis_crossover(gen),
                b_yscale = dis_crossover(gen),
                b_persp = dis_crossover(gen),
                b_vision = dis_crossover(gen),
                b_skinx = dis_crossover(gen),
                b_skiny = dis_crossover(gen),
                b_skinr = dis_crossover(gen);

        return Animal(shape, 0, 0, INIT_FOOD_AMOUNT,
                b_meta * metabolism + (1 - b_meta) * o.metabolism + mut(gen),
                b_age * agelimit + (1 - b_age) * o.agelimit + mut(gen),
                b_speed * vmax + (1 - b_speed) * o.vmax + mut(gen)*.1,
                b_xscale * xscale + (1 - b_xscale) * o.xscale + mut(gen)*.1,
                b_yscale * yscale + (1 - b_yscale) * o.yscale + mut(gen)*.1,
                b_persp * perspective + (1 - b_persp) * o.perspective + mut(gen)*.1,
                b_vision * vision + (1 - b_vision) * o.vision + mut(gen)*.1,
                b_skinx * (float) skin_xinit + (1 - b_skinx) * (float) o.skin_xinit + mut(gen)*.1,
                b_skiny * (float) skin_yinit + (1 - b_skiny) * (float) o.skin_yinit + mut(gen)*.1,
                b_skinr * skin_radius + (1 - b_skinr) * o.skin_radius + mut(gen)*.1,
                dis_bool(gen) == 1);
    }

    const std::vector<Point> * getShape() const {
        return shape;
    }

    void kill(Food *f = NULL) {
        *stop = true;
        if (f != NULL) {
            int x = (this->x + 1) / 2. * f->n_food_sites, y = (this->y + 1) / 2. * f->n_food_sites;
            std::cout << x << " " << y << std::endl;
            for (int i = 0; i < foodStock; i++)
                f->addFood(x, y);
        }
        t->join();
    }

    void setVision(const double &v) {
        vision = v;
    }
protected:

    template<class T>
    std::pair<double, double> separation(const std::vector<T*> &predators) const {
        double sx = 0, sy = 0;
        for (const T* b : predators) {
            if (b == this)
                continue;
            if (squaredTorusDistance(*b) < T::SQUARED_DIST_SEPARATION) {
                sx -= b->x - x;
                sy -= b->y - y;
            }
        }
        return std::make_pair(sx, sy);
    }

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
    virtual ~Animal() = default;

    void setPosition(const double &x, const double &y) {
        this->x = x;
        this->y = y;
    }

    std::pair<double, double> getPosition() const {
        return std::make_pair(x, y);
    }

    virtual void draw() const {
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
            float h = big_textures ? .15 : .05, w = big_textures ? .15 : .05;
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
            glVertex3d(x + p.x * xscale, y + (p.y + sign(p.y) * expm1(p.x * perspective)) * yscale, p.z);
        glEnd();
        if (Animal::debug) {
            int timeRemaining = age / (float) agelimit * 100;
            displayText(x, y - .075, 1, 1, 1, sex ? "M" : "F");
            displayText(x + .3 * yscale, y - .075, 1, timeRemaining >= 85 ? 0 : 1, timeRemaining >= 85 ? 0 : 1, std::to_string(timeRemaining) + "%");
            int ttl = foodStock / (float) metabolism;
            displayText(x + .8 * yscale, y - .075, 1, ttl <= 100 ? 0 : 1, ttl <= 100 ? 0 : 1, std::to_string(ttl));
            //displayText(x, y - .1, 1, 1,1, std::to_string(metabolism) + " "+std::to_string(foodStock));
        }
        if (Animal::debug_fov) {
            drawCircle(this->x, y, vision);
        }
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

