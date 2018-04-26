/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Food.h
 * Author: romain
 *
 * Created on April 25, 2018, 11:37 AM
 */

#ifndef FOOD_H
#define FOOD_H

class Food {
    int maxCapacity = 250;
    int** foodCapacity;
    int** foodCurrent;
public:
    const int n_food_sites;

    int getCurrent(const int &x, const int &y) const {
        return foodCurrent[x][y];
    }

    int getCapacity(const int &x, const int &y) const {
        return foodCapacity[x][y];
    }
    void consume(const int &x, const int &y) {
        foodCurrent[x][y] = -300;
    }

    Food(const int &n_food_sites) : n_food_sites(n_food_sites) {
        foodCapacity = new int*[n_food_sites];
        foodCurrent = new int*[n_food_sites];
        for (int i = 0; i < n_food_sites; i++) {
            foodCapacity[i] = new int[n_food_sites];
            foodCurrent[i] = new int[n_food_sites];
            for (int j = 0; j < n_food_sites; j++) {
                foodCapacity[i][j] = 0;
                foodCurrent[i][j] = 0;
            }
        }
    }

    ~Food() {
        for (int i = 0; i < n_food_sites; i++) {
            delete[] foodCapacity[i];
            delete[] foodCurrent[i];
        }
        delete[] foodCapacity;
        delete[] foodCurrent;
    }

    void generateFood(unsigned int food_sites[][4], unsigned int n) {

        std::random_device rd{};
        std::mt19937 gen{rd()};

        for (unsigned int k = 0; k < n; k++) {
            std::normal_distribution<> d{0, (double) food_sites[k][2]};

            for (unsigned int i = 0; i < food_sites[k][3]; i++) {
                int x = (int) round(d(gen)), y = (int) round(d(gen));
                x += food_sites[k][0] + n_food_sites;
                y += food_sites[k][1] + n_food_sites;
                x %= n_food_sites;
                y %= n_food_sites;
                foodCapacity[x][y] += 4;
            }
            for (int i = 0; i < n_food_sites; i++) {
                for (int j = 0; j < n_food_sites; j++) {
                    foodCurrent[i][j] = foodCapacity[i][j];
                }
            }
        }
    }

    void regenerateFood() {
        for (int i = 0; i < n_food_sites; i++) {
            for (int j = 0; j < n_food_sites; j++) {
                foodCurrent[i][j] = std::min(foodCurrent[i][j] + 1, foodCapacity[i][j]);
            }
        }
    }

    void Draw() const {
        double x, y;
        int i, j;
        double food_site_size = 2. / n_food_sites;
        for (i = 0, x = -1.; i < n_food_sites; i++, x += food_site_size) {
            for (j = 0, y = -1.; j < n_food_sites; j++, y += food_site_size) {
                if (foodCurrent[i][j] < 1) {
                    continue;
                }
                glColor4d(46. / 255, 204. / 255, 113. / 255, .5 + ((double) foodCurrent[i][j]) / maxCapacity / 2);
                glBegin(GL_TRIANGLES);

                glVertex2d(x + food_site_size / 6, y + food_site_size / 6);
                glVertex2d(x + food_site_size * 5. / 6., y + food_site_size / 6);
                glVertex2d(x + food_site_size / 2, y + food_site_size);

                glEnd();
            }
        }
    }


};
#endif /* FOOD_H */

