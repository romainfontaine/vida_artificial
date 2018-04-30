#include "GL/freeglut.h"
#include "GL/gl.h"
#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include <random>

#include "Tools.h"
#include "Boid.h"
#include "Predator.h"

void Boid::update(const std::vector<Boid> &boids,
        const std::vector<Predator> &predators)
{
    std::pair<double, double> e;
    if (escapePredator(predators, e))
    {
        vx += e.first;
        vy += e.second;
    }
    else
    {
        std::pair<double, double> c = cohesion(boids);
        std::pair<double, double> s = separation(boids);
        std::pair<double, double> a = alignment(boids);
        std::pair<double, double> f = foodMove();

        vx += c.first + s.first + a.first + f.first;
        vy += c.second + s.second + a.second + f.second;
    }
    updatePosition_NormalizeSpeed();
    eatFood();
}

bool Boid::escapePredator(const std::vector<Predator> &predators,
        std::pair<double, double> &newDirection) const
{
    newDirection.first = 0;
    newDirection.second = 0;
    int c = 0;
    for (const Predator &p : predators)
    {
        if (squaredTorusDistance(p) < 0.03)
        {
            auto pos = p.getPosition();
            auto d = Animal::diff(2.,x, pos.first, y, pos.second);
            newDirection.first += d.first;
            newDirection.second += d.second;
            c++;
        }
    }

    return c != 0;
}

void Boid::eatFood()
{
    int xgrid = (x + 1) / 2 * food->n_food_sites;
    int ygrid = (y + 1) / 2 * food->n_food_sites;
    const int food_radius = 3;
    for (int i = -food_radius; i <= food_radius; i++)
    {
        for (int j = -food_radius; j <= food_radius; j++)
        {
            int x = positive_modulo(xgrid + i, food->n_food_sites);
            int y = positive_modulo(ygrid + j, food->n_food_sites);
            if (food->getCurrent(x, y) > 0)
            {
                foodStock += food->getCurrent(x, y);
                food->consume(x, y);
            }
        }
    }
}

std::pair<double, double> Boid::cohesion(const std::vector<Boid> &boids) const
{
    double sx = 0, sy = 0;
    for (const Boid &b : boids)
    {
        if (b.id == id)
            continue;
        sx += b.x;
        sy += b.y;
    }
    sx /= boids.size() - 1;
    sy /= boids.size() - 1;

    return std::make_pair(sx / 100, sy / 100);
}

std::pair<double, double> Boid::separation(const std::vector<Boid> &boids) const
{
    double sx = 0, sy = 0;
    for (const Boid &b : boids)
    {
        if (b.id == id)
            continue;
        if (squaredTorusDistance(b) < SQUARED_DIST_SEPARATION)
        {
            sx -= b.x - x;
            sy -= b.y - y;
        }
    }
    return std::make_pair(sx, sy);
}

std::pair<double, double> Boid::alignment(const std::vector<Boid> &boids) const
{
    double sx = 0, sy = 0;
    for (const Boid &b : boids)
    {
        if (b.id == id)
            continue;
        sx += b.vx;
        sy += b.vy;
    }
    sx /= boids.size() - 1;
    sy /= boids.size() - 1;
    return std::make_pair((sx - vx) / 8, (sy - vy) / 8);
}

std::pair<double, double> Boid::foodMove() const
{
    int xgrid = (x + 1) / 2 * food->n_food_sites;
    int ygrid = (y + 1) / 2 * food->n_food_sites;
    int maxF = -1, dirX = 0, dirY = 0, minD = 0;
    for (int i = -vision; i <= vision; i++)
    {
        for (int j = -vision; j <= vision; j++)
        {
            int x = positive_modulo(xgrid + i, food->n_food_sites);
            int y = positive_modulo(ygrid + j, food->n_food_sites);
            int d = squaredTorusDistance(food->n_food_sites, xgrid, ygrid, x, y);
            if (food->getCurrent(x, y) > maxF || (food->getCurrent(x, y) == maxF && d < minD))
            {
                minD = d;
                maxF = food->getCurrent(x, y);
                dirX = i < 0 ? -1 : (i > 0 ? 1 : 0);
                dirY = j < 0 ? -1 : (j > 0 ? 1 : 0);
            }
        }
    }
    if ((dirX == 0 && dirY == 0) || maxF <= 0)
    {
        return std::make_pair(0, 0);
    }
    else
    {
        double norm = sqrt((dirX * dirX) + (dirY * dirY));
        double dx = (double) dirX / norm;
        double dy = (double) dirY / norm;
        const double food_fact = .07;
        return std::make_pair(dx*food_fact, dy * food_fact);
    }
}