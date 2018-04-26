/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Boid.h
 * Author: romain
 *
 * Created on April 20, 2018, 11:36 AM
 */

#ifndef BOID_H
#define BOID_H

#include "Animal.h"

class Boid {
    // Source for basic boid rules: http://www.kfish.org/boids/pseudocode.html
    static int ID_COUNT;
    const double SQUARED_DIST_SEPARATION = 0.025;
protected:
    double x, y, vx, vy, vmax, xscale, yscale, perspective;
    int id, foodStock, metabolism, age, agelimit, vision;
    Food* food;
    Animal* animal;
public:

    void operator=(const Boid &b) {
        x = b.x;
        y = b.y;
        vx = b.vx;
        vy = b.vy;
        vmax = b.vmax;
        xscale = b.xscale;
        yscale = b.yscale;
        perspective = b.perspective;
        id = b.id;
        foodStock = b.foodStock;
        metabolism = b.metabolism;
        age = b.age;
        agelimit = b.agelimit;
        vision = b.vision;
        food = food;
        animal = animal;
    }

    Boid(Food* f, Animal* a, const double &x = 0, const double &y = 0,
            const int &foodStock = 200, const int &metabolism = 1,
            const int &agelimit = 1000, const int &vision = 10, const double &vmax = .005,
            const double &xscale = .15, const double&yscale = .15, const double &perspective = 0.)
    : x(x), y(y), vx(0), vy(0), vmax(vmax), xscale(xscale), yscale(yscale),
    perspective(perspective), id(ID_COUNT++), foodStock(foodStock),
    metabolism(metabolism), age(0), agelimit(agelimit), vision(vision), food(f), animal(a) {
    }

    friend std::ostream& operator<<(std::ostream& s, const Boid& b) {
        return s << "Boid #" << b.id << " | pos={" << b.x << "," << b.y << 
                "} vmax=" << b.vmax << " xscale=" << b.xscale << " yscale=" <<
                b.yscale << " perspective=" << b.perspective << " id=" << b.id <<
                " foodStock=" << b.foodStock << " metabolism=" << b.metabolism <<
                " age=" << b.age << " agelimit=" << b.agelimit << " vision=" <<
                b.vision;
    }

    static Boid individual(Food* f, Animal* a) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis_meta(1, 3);
        std::normal_distribution<> dis_age(1000, 100);
        std::uniform_int_distribution<> dis_vision(5, 15);
        std::uniform_real_distribution<> dis_speed(.001, .006);
        std::uniform_real_distribution<> dis_scale(.075, .12);
        std::uniform_real_distribution<> dis_persp(-.15, .15);

        return Boid(f, a, 0, 0, 200, dis_meta(gen), dis_age(gen),
                dis_vision(gen), dis_speed(gen), dis_scale(gen),
                dis_scale(gen), dis_persp(gen));
    }

    void setPosition(const double &x, const double &y) {
        this->x = x;
        this->y = y;
    }

    void update(const std::vector<Boid> &boids) {
        std::pair<double, double> c = cohesion(boids);
        std::pair<double, double> s = separation(boids);
        std::pair<double, double> a = alignment(boids);
        std::pair<double, double> f = foodMove();

        vx += c.first + s.first + a.first + f.first;
        vy += c.second + s.second + a.second + f.second;
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
        eatFood();
    }

    void draw() {
        animal->setPerspective(perspective);
        animal->setScaleX(xscale);
        animal->setScaleY(yscale);
        animal->Draw(x, y);
    }

    bool consumeEnergy() {
        return ((foodStock -= metabolism) > 0) && (age++<agelimit);
    }

private:

    void eatFood() {
        int xgrid = (x + 1) / 2 * food->n_food_sites;
        int ygrid = (y + 1) / 2 * food->n_food_sites;

        for (int i = -2; i <= 2; i++) {
            for (int j = -2; j <= 2; j++) {
                int x = positive_modulo(xgrid + i, food->n_food_sites);
                int y = positive_modulo(ygrid + j, food->n_food_sites);
                if (food->getCurrent(x, y) > 0) {
                    foodStock += food->getCurrent(x, y);
                    food->consume(x, y);
                }
            }
        }
    }

    double squaredTorusDistance(const Boid &b) const {
        return squaredTorusDistance(2, x, y, b.x, b.y);
    }

    static double squaredTorusDistance(const double &size, const double &fx, const double &fy, const double &nx, const double &ny) {
        // Source: https://stackoverflow.com/q/2123947/4384857
        double a = std::min(std::abs(fx - nx), size - std::abs(fx - nx));
        double x = std::min(std::abs(fy - ny), size - std::abs(fy - ny));
        return a * a + x * x;
    }

    std::pair<double, double> cohesion(const std::vector<Boid> &boids) const {
        double sx = 0, sy = 0;
        for (const Boid &b : boids) {
            if (b.id == id)
                continue;
            sx += b.x;
            sy += b.y;
        }
        sx /= boids.size() - 1;
        sy /= boids.size() - 1;

        return std::make_pair(sx / 100, sy / 100);
    }

    std::pair<double, double> separation(const std::vector<Boid> &boids) const {
        double sx = 0, sy = 0;
        for (const Boid &b : boids) {
            if (b.id == id)
                continue;
            if (squaredTorusDistance(b) < SQUARED_DIST_SEPARATION) {
                sx -= b.x - x;
                sy -= b.y - y;
            }
        }
        return std::make_pair(sx, sy);
    }

    std::pair<double, double> alignment(const std::vector<Boid> &boids) const {
        double sx = 0, sy = 0;
        for (const Boid &b : boids) {
            if (b.id == id)
                continue;
            sx += b.vx;
            sy += b.vy;
        }
        sx /= boids.size() - 1;
        sy /= boids.size() - 1;
        return std::make_pair((sx - vx) / 8, (sy - vy) / 8);
    }

    std::pair<double, double> foodMove() const {
        int xgrid = (x + 1) / 2 * food->n_food_sites;
        int ygrid = (y + 1) / 2 * food->n_food_sites;
        int maxF = -1, dirX = 0, dirY = 0, minD = 0;
        for (int i = -vision; i <= vision; i++) {
            for (int j = -vision; j <= vision; j++) {
                int x = positive_modulo(xgrid + i, food->n_food_sites);
                int y = positive_modulo(ygrid + j, food->n_food_sites);
                int d = squaredTorusDistance(food->n_food_sites, xgrid, ygrid, x, y);
                if (food->getCurrent(x, y) > maxF || (food->getCurrent(x, y) == maxF && d < minD)) {
                    minD = d;
                    maxF = food->getCurrent(x, y);
                    dirX = i < 0 ? -1 : (i > 0 ? 1 : 0);
                    dirY = j < 0 ? -1 : (j > 0 ? 1 : 0);
                }
            }
        }
        if ((dirX == 0 && dirY == 0) || maxF <= 0) {
            return std::make_pair(0, 0);
        } else {
            double norm = sqrt((dirX * dirX) + (dirY * dirY));
            double dx = (double) dirX / norm;
            double dy = (double) dirY / norm;
            const double food_fact = .1;
            return std::make_pair(dx*food_fact, dy * food_fact);
        }
    }
};
int Boid::ID_COUNT = 0;

#endif /* BOID_H */

