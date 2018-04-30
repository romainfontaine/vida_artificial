#include "GL/freeglut.h"
#include "GL/gl.h"
#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include <random>

#include "Tools.h"
#include "Prey.h"
#include "Predator.h"

void Prey::update(const std::vector<Prey> &preys,
        const std::vector<Predator> &predators)
{
    std::pair<double, double> e;
    std::pair<double, double> s = separation(preys);
    vx += e.first * 1000 + s.first;
    vy += e.second * 1000 + s.second;
    if (!escapePredator(predators, e))
    {
        std::pair<double, double> c = cohesion(preys);
        std::pair<double, double> a = alignment(preys);
        std::pair<double, double> f = foodMove();
        vx += f.first + c.first + a.first;
        vy += f.second + c.second + a.second;
    }
    updatePosition_NormalizeSpeed();
    eatFood();
}

bool Prey::escapePredator(const std::vector<Predator> &predators,
        std::pair<double, double> &newDirection) const
{
    newDirection.first = 0;
    newDirection.second = 0;
    bool danger = false;
    for (const Predator &p : predators)
    {
        if (squaredTorusDistance(p) < 0.05)
        {
            auto pos = p.getPosition();
            newDirection.first += x - pos.first;
            newDirection.second += y - pos.second;
            danger = true;
        }
    }

    return danger;
}

void Prey::eatFood()
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

std::pair<double, double> Prey::cohesion(const std::vector<Prey> &preys) const
{
    double sx = 0, sy = 0;
    for (const Prey &b : preys)
    {
        if (b.id == id)
            continue;
        sx += b.x;
        sy += b.y;
    }
    sx /= preys.size() - 1;
    sy /= preys.size() - 1;

    return std::make_pair(sx / 100, sy / 100);
}

std::pair<double, double> Prey::separation(const std::vector<Prey> &preys) const
{
    double sx = 0, sy = 0;
    for (const Prey &b : preys)
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

std::pair<double, double> Prey::alignment(const std::vector<Prey> &preys) const
{
    double sx = 0, sy = 0;
    for (const Prey &b : preys)
    {
        if (b.id == id)
            continue;
        sx += b.vx;
        sy += b.vy;
    }
    sx /= preys.size() - 1;
    sy /= preys.size() - 1;
    return std::make_pair((sx - vx) / 8, (sy - vy) / 8);
}

std::pair<double, double> Prey::foodMove() const
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