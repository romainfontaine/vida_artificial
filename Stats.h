/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Stats.h
 * Author: romain
 *
 * Created on May 18, 2018, 2:07 PM
 */

#ifndef STATS_H
#define STATS_H

#include <algorithm>

class Stats {
    std::vector<float> values;
    std::string name;
    unsigned int n_decimals;
public:

    explicit Stats(std::string name, unsigned int n_decimals = 2) : name(name),
    n_decimals(n_decimals) {

    }

    void add(float f) {
        values.push_back(f);
    }

    void Draw(float x = -.9, float y = .9) {
        float scale = .4;
        glColor3d(1., 1., 1.);
        glBegin(GL_LINE_STRIP);
        glVertex2f(x + 0. * scale, y + 0. * scale);
        glVertex2f(x + 0. * scale, y - 1. * scale);
        glVertex2f(x + 1. * scale, y - 1. * scale);
        glEnd();

        if (values.size() <= 1)
            return;
        float max = values[0], min = values[0];
        for (unsigned int i = 1; i < values.size(); i++) {
            max = std::max(max, values[i]);
            min = std::min(min, values[i]);
        }
        displayText(x - .07, y, 1, 1, 1, toString(max, n_decimals));
        displayText(x - .07, y - 1 * scale, 1, 1, 1, toString(min, n_decimals));
        displayText(x + 1 * scale, y - 1.1 * scale, 1, 1, 1, std::to_string(values.size()));
        displayText(x, y - 1.1 * scale, 1, 1, 1, "0");
        displayText(x + .33 * scale, y - 1.1 * scale, 1, 1, 1, name);
        max *= 1.1;
        min *= .9;


        glBegin(GL_LINE_STRIP);
        for (unsigned int i = 0; i < values.size(); i++) {
            glVertex2f(x + (0. + 1. / values.size() * (float) i) * scale, y + (-1. + (values[i] - min) / (max - min)) * scale);
        }
        glEnd();
    }

};


#endif /* STATS_H */

