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

void Prey::update(const std::vector<Prey*> &preys,
        const std::vector<Predator*> &predators)
{
    std::pair<double, double> e;
    std::pair<double, double> s = separation(preys);
    vx += s.first*5;
    vy += s.second*5;
    if (!escapePredator(predators, e))
    {
        std::pair<double, double> c = cohesion(preys);
        std::pair<double, double> a = alignment(preys);
        std::pair<double, double> f = foodMove();
        vx += f.first*10 + c.first*.5 + a.first;
        vy += f.second*10 + c.second*.5 + a.second;
    }
    else
    {
        vx += e.first * 1000;
        vy += e.second * 1000;
    }
    if(vx == 0 && vy == 0){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> d(-1, 1);
        vx = d(gen);
        vy = d(gen);
    }
    updatePosition_NormalizeSpeed();
    eatFood();
}

bool Prey::escapePredator(const std::vector<Predator*> &predators,
        std::pair<double, double> &newDirection) const
{
    newDirection.first = 0;
    newDirection.second = 0;
    bool danger = false;
    for (const Predator* p : predators)
    {
        if (squaredTorusDistance(*p) < std::pow(vision, 2))
        {
            auto pos = p->getPosition();
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

std::pair<double, double> Prey::cohesion(const std::vector<Prey*> &preys) const
{
    if (preys.size() == 1)
    {
        return std::make_pair(0, 0);
    }
    double sx = 0, sy = 0;
    for (const Prey* b : preys)
    {
        if (b != this)
            continue;
        sx += b->x;
        sy += b->y;
    }
    sx /= preys.size() - 1;
    sy /= preys.size() - 1;

    return std::make_pair(x < 0 ? sx - x : x - sx, y < 0 ? sy - y : y - sy);
}

std::pair<double, double> Prey::alignment(const std::vector<Prey*> &preys) const
{
    if (preys.size() == 1)
    {
        return std::make_pair(0, 0);
    }
    double sx = 0, sy = 0;
    for (const Prey* b : preys)
    {
        if (b != this)
            continue;
        sx += b->vx;
        sy += b->vy;
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
    for (int i = -vision_int; i <= vision_int; i++)
    {
        for (int j = -vision_int; j <= vision_int; j++)
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