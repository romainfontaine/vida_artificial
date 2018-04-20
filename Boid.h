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
public:

    Boid(const double &x = 0, const double &y = 0, const double &vx = 0, const double &vy = 0)
    : x(x), y(y), vx(vx), vy(vy), id(ID_COUNT++) {
    }

    void update(const std::vector<Boid> &boids, const Animal &animal, const double &vmax = .005) {
        std::pair<double, double> c = cohesion(boids);
        std::pair<double, double> s = separation(boids);
        std::pair<double, double> a = alignment(boids);
        vx += c.first + s.first + a.first;
        vy += c.second + s.second + a.second;
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
    }

private:

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
};
int Boid::ID_COUNT = 0;

#endif /* BOID_H */

