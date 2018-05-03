/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReactionDiffusion.h
 * Author: romain
 *
 * Created on April 30, 2018, 12:52 PM
 */

#ifndef REACTIONDIFFUSION_H
#define REACTIONDIFFUSION_H

class ReactionDiffusion {
public:
    static const int size = 40;
private:
    float u[2][size][size] = {};
    float v[2][size][size] = {};
    unsigned char texture[size * size * 3] = {};
    int current = 0;

    float F, k;

    inline float laplacian(const int &i, const int &j, float m[size][size]) {
        return -1 * m[i][j] +
                // Horizontal & vertical
                .2 * (m[(i + 1) % size][j] +
                m[(i - 1 + size) % size][j] +
                m[i][(j + 1) % size] +
                m[i][(j - 1 + size) % size]) +
                // Diagonals
                .05 * (m[(i + 1) % size][(j + 1) % size] +
                m[(i - 1 + size) % size][(j + 1) % size] +
                m[(i - 1 + size) % size][(j - 1 + size) % size] +
                m[(i + 1) % size][(j - 1 + size) % size]);
    }

public:

    ReactionDiffusion(float F = 0.0545, float k = 0.062) : F(F), k(k) {

    }

    int getSize() const {
        return this->size;
    }

    void initReact(int x = 0, int y = 20, int r = 2) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                u[0][i][j] = 1;
            }
        }
        for (int i = -r; i < r; i++) {
            for (int j = -r; j < r; j++) {
                int nx = x + i, ny = y + j;
                if ((x - nx)*(x - nx)+(y - ny)*(y - ny) <= r * r) {
                    v[0][positive_modulo(nx, size)][positive_modulo(ny, size)] = 1;
                }
            }
        }
    }

    void iterate(const float &t = 1) {
        current = 1 - current;
        float Du = 0.2097, Dv = .105;

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                int other = 1 - current;

                float du = Du * laplacian(i, j, u[other]) - u[other][i][j] * v[other][i][j] * v[other][i][j] + F * (1 - u[other][i][j]);
                float dv = Dv * laplacian(i, j, v[other]) + u[other][i][j] * v[other][i][j] * v[other][i][j]-(F + k) * v[other][i][j];

                u[current][i][j] = std::max(0.f, u[other][i][j] + du * t);
                v[current][i][j] = std::max(0.f, v[other][i][j] + dv * t);
            }
        }
    }

    void generateUCharArray() {
        double colors[][3] = {
            {0.867, 0.867, 0.},
            {0., .8, .8},
            {0., 0., .8},
            {0., 0., .27}
        };

        double thresholds[] = {
            .1, .4, 1.
        };

        for (int i = 0; i < size * size * 3; i += 3) {
            int x = i / 3 / size;
            int y = (i / 3) % size;
            double val = v[current][x][y];

            for (unsigned int k = 0; k<sizeof (thresholds) / sizeof (double); k++) {
                if (val < thresholds[k]) {
                    val /= thresholds[k];
                    for (int m = 0; m < 3; m++) {
                        texture[i + m] = (colors[k][m] * (1 - val) + colors[k + 1][m] * val) * 255.;
                    }
                    break;
                }
            }
        }
    }

    const unsigned char* toUCharArray() const {
        return texture;
    }


};


#endif /* REACTIONDIFFUSION_H */

