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
    double x, y, vx, vy;
    int id;
    int foodStock = 100;
    int metabolism = 1;
    int age = 0;
    int agelimit = 1000;
public:

    void operator=(const Boid &b) {
        x = b.x;
        y = b.y;
        vx = b.vx;
        vy = b.vy;
        id = b.id;
        foodStock = b.foodStock;
        metabolism = b.metabolism;
        age = b.age;
        agelimit = b.agelimit;
    }

    Boid(const double &x = 0, const double &y = 0, const double &vx = 0, const double &vy = 0)
    : x(x), y(y), vx(vx), vy(vy), id(ID_COUNT++) {
    }

    void update(const std::vector<Boid> &boids, const Animal &animal, const double &vmax = .005) {
        std::pair<double, double> c = cohesion(boids);
        std::pair<double, double> s = separation(boids);
        std::pair<double, double> a = alignment(boids);
        std::pair<double, double> f = food();

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
        animal.Draw(x, y);
        eatFood();
    }

    bool consumeEnergy() {
        return ((foodStock -= metabolism) > 0) && (age++<agelimit);
    }

private:

    void eatFood() {
        int xgrid = (x + 1) / 2 * n_food_sites;
        int ygrid = (y + 1) / 2 * n_food_sites;


        for (int i = -2; i <= 2; i++) {
            for (int j = -2; j <= 2; j++) {
                int x = (xgrid + i) % n_food_sites;
                int y = (ygrid + j) % n_food_sites;
                if (foodCurrent[x][y] > 0) {
                    foodStock += foodCurrent[x][y];
                    foodCurrent[x][y] = -300;
                }

            }
        }
    }

    double squaredTorusDistance(const Boid &b) const {
        // Source: https://stackoverflow.com/q/2123947/4384857
        double a = std::min(std::abs(x - b.x), 2. - std::abs(x - b.x));
        double x = std::min(std::abs(y - b.y), 2. - std::abs(y - b.y));
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

    std::pair<double, double> food() const {
        int xgrid = (x + 1) / 2 * n_food_sites;
        int ygrid = (y + 1) / 2 * n_food_sites;

        int maxF = -1, dirX = 0, dirY = 0;
        for (int i = -n_food_sites / 10; i <= n_food_sites / 10; i++) {
            for (int j = -n_food_sites / 10; j <= n_food_sites / 10; j++) {
                int x = (xgrid + i) % n_food_sites;
                int y = (ygrid + j) % n_food_sites;
                if (foodCurrent[x][y] > maxF) {
                    maxF = foodCurrent[x][y];
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

