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
public:
    const int n_food_sites;
private:
    const int maxCapacity = 20;
    int** foodCapacity;
    int** foodCurrent;
    int** foodSeason;
    int currentSeason = 0;
    int seasonsNbr = 0;
    int seasonFrames = 0;
    const int seasonDuration = 300;

    Food(const Food &f) = delete;

    void setFullCapacity() {
        for (int i = 0; i < n_food_sites; i++) {
            for (int j = 0; j < n_food_sites; j++) {
                if (currentSeason == foodSeason[i][j])
                    foodCurrent[i][j] = foodCapacity[i][j];
            }
        }
    }

    void addSand(const int &x, const int &y, const int &k) {
        if (x < 0 || y < 0 || x >= n_food_sites || y >= n_food_sites)
            return;
        foodSeason[x][y] = k;
        foodCapacity[x][y]++;
        if (foodCapacity[x][y] >= 4) {
            foodCapacity[x][y] = 0;
            addSand(x + 1, y, k);
            addSand(x - 1, y, k);
            addSand(x, y + 1, k);
            addSand(x, y - 1, k);
        }
    }

public:

    void addFood(const int &x, const int &y) {
        if (x < 0 || y < 0 || x >= n_food_sites || y >= n_food_sites)
            return;
        foodCurrent[x][y]++;
        if (foodCurrent[x][y] >= 4) {
            foodCurrent[x][y] = 0;
            addFood(x + 1, y);
            addFood(x - 1, y);
            addFood(x, y + 1);
            addFood(x, y - 1);
        }
    }

    int getCurrent(const int &x, const int &y) const {
        return foodCurrent[x][y];
    }

    int getCapacity(const int &x, const int &y) const {
        return foodCapacity[x][y];
    }

    void consume(const int &x, const int &y) {
        foodCurrent[x][y] = -300;
    }

    explicit Food(const int &n_food_sites) : n_food_sites(n_food_sites) {
        foodCapacity = new int*[n_food_sites];
        foodCurrent = new int*[n_food_sites];
        foodSeason = new int*[n_food_sites];
        for (int i = 0; i < n_food_sites; i++) {
            foodCapacity[i] = new int[n_food_sites];
            foodCurrent[i] = new int[n_food_sites];
            foodSeason[i] = new int[n_food_sites];
            for (int j = 0; j < n_food_sites; j++) {
                foodCapacity[i][j] = 0;
                foodCurrent[i][j] = 0;
                foodSeason[i][j] = -1;
            }
        }
    }

    ~Food() {
        for (int i = 0; i < n_food_sites; i++) {
            delete[] foodCapacity[i];
            delete[] foodCurrent[i];
            delete[] foodSeason[i];
        }
        delete[] foodCapacity;
        delete[] foodCurrent;
        delete[] foodSeason;
    }

    void generateFoodSandpile(unsigned int food_sites[][3], unsigned int n) {
        seasonsNbr = n;
        for (unsigned int k = 0; k < n; k++) {
            for (unsigned int l = 0; l < food_sites[k][2]; l++) {
                addSand(food_sites[k][0], food_sites[k][1], k);
            }
        }
        for (int i = 0; i < n_food_sites; i++) {
            for (int j = 0; j < n_food_sites; j++) {
                foodCapacity[i][j] *= maxCapacity / 4;
            }
        }
        setFullCapacity();
    }

    void regenerateFood() {
        if (seasonFrames++>seasonDuration) {
            seasonFrames = 0;
            currentSeason++;
            currentSeason %= seasonsNbr;
        }
        for (int i = 0; i < n_food_sites; i++) {
            for (int j = 0; j < n_food_sites; j++) {
                if (currentSeason == foodSeason[i][j]) {
                    foodCurrent[i][j] = std::min(foodCurrent[i][j] + 1, foodCapacity[i][j]);
                } else if (foodSeason[i][j] != -1 && foodCurrent[i][j] < 0) {
                    foodCurrent[i][j] = -30;
                }
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

                if (currentSeason == foodSeason[i][j])
                    glColor4d(46. / 255, 204. / 255, 113. / 255, .3 + ((double) foodCurrent[i][j]) / maxCapacity / 3. * 2.);
                else
                    glColor4d(245. / 255, 215. / 255, 110. / 255, .3 + ((double) foodCurrent[i][j]) / maxCapacity / 3. * 2.);

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

