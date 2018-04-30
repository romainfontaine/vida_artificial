#include "GL/freeglut.h"
#include "GL/gl.h"
#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include <random>

#include "Tools.h"
#include "TurtleInterpreter.h"
#include "LSystem.h"
#include "Plant.h"
#include "Boid.h"
#include "Predator.h"

int Boid::ID_COUNT = 0;
const double Boid::SQUARED_DIST_SEPARATION = 0.015;
const double Predator::SQUARED_DIST_EAT = 0.01;

static std::vector<Point> fish1{
    {0.0625, -0.0692307692, 1},
    {0.0, 0.1846153846, 1},
    {0.125, 0.1461538462, 1},
    {0.17578125, -0.0076923077, 1},
    {0.30859375, 0.2384615385, 1},
    {0.31640625, 0.4923076923, 1},
    {0.61328125, 0.5, 1},
    {0.734375, 0.3538461538, 1},
    {0.875, 0.1846153846, 1},
    {1.0, -0.0923076923, 1},
    {0.90625, -0.2769230769, 1},
    {0.77734375, -0.4153846154, 1},
    {0.640625, -0.4692307692, 1},
    {0.50390625, -0.4461538462, 1},
    {0.4375, -0.4153846154, 1},
    {0.3046875, -0.5, 1},
    {0.3203125, -0.3538461538, 1},
    {0.171875, -0.2076923077, 1},
    {0.125, -0.3230769231, 1},
    {0.00390625, -0.4, 1},
    {0.0625, -0.1076923077, 1}
};

static std::vector<Point> fish2{
    {0.0, 0.2055837563, 1},
    {0.12862318839999998, 0.0837563452, 1},
    {0.45108695649999997, 0.2664974619, 1},
    {0.4692028986, 0.3527918782, 1},
    {0.472826087, 0.4492385787, 1},
    {0.4746376812, 0.5, 1},
    {0.5905797101, 0.3020304569, 1},
    {0.6956521739, 0.2868020305, 1},
    {0.7844202899, 0.2461928934, 1},
    {0.9130434783, 0.1294416244, 1},
    {1.0, -0.1192893401, 1},
    {0.9710144928, -0.1954314721, 1},
    {0.9239130435, -0.2512690355, 1},
    {0.8731884058, -0.2969543147, 1},
    {0.7608695651999999, -0.3426395939, 1},
    {0.7355072464, -0.4898477157, 1},
    {0.7264492754, -0.5, 1},
    {0.7173913043, -0.5, 1},
    {0.7065217391, -0.4746192893, 1},
    {0.6884057970999999, -0.3934010152, 1},
    {0.6902173913, -0.3578680203, 1},
    {0.490942029, -0.3274111675, 1},
    {0.4692028986, -0.3730964467, 1},
    {0.4420289855, -0.4137055838, 1},
    {0.4221014493, -0.4137055838, 1},
    {0.4184782609, -0.3934010152, 1},
    {0.4202898551, -0.3578680203, 1},
    {0.4275362319, -0.307106599, 1},
    {0.3278985507, -0.2766497462, 1},
    {0.2717391304, -0.3527918782, 1},
    {0.25543478259999997, -0.2563451777, 1},
    {0.1956521739, -0.2157360406, 1},
    {0.15760869570000002, -0.2055837563, 1},
    {0.05797101449999997, -0.4340101523, 1},
    {0.04166666670000002, -0.4441624365, 1},
    {0.03442028990000001, -0.4289340102, 1},
    {0.03260869570000002, -0.3883248731, 1},
    {0.04347826090000001, -0.2258883249, 1},
    {0.019927536199999984, 0.0939086294, 1},
    {0.0, 0.2055837563, 1}
};

static Plant p1(Turtle(.0035, 20, -.5, -1, 1.5, .05), LSystem('X',{
    {'F',
        {"FF"}},
    {'X',
        {"F[+X]F[-X]+X"}}
}), 7, Color(43. / 255, 112. / 255, 46. / 255));

static Plant p2(Turtle(.02, 20, .5, -1, .3, .1), LSystem('F',{
    {'F',
        {"FF-[-F+F+F]+[+F-F-F]"}}
}), 4, Color(43. / 255, 112. / 255, 46. / 255));


static Plant p3(Turtle(.02, 20, 0, -1, 1, .1), LSystem('F',{
    {'F',
        {"F[+F]F[-F]F", "F[+F]F", "F[-F]F"}}
}), 5, Color(96. / 255, 140. / 255, 93. / 255));

#include "Food.h"
static Food food(100);
#include "Animal.h"
#include "Boid.h"
#include "Predator.h"
static std::vector<Boid> preys;
static std::vector<Predator> predators;

void texture(unsigned char* texDat, unsigned int tw, unsigned int th,
        double x = 0, double y = 0, double w = 1, double h = 1)
{
    // Source: https://stackoverflow.com/a/24266568/4384857
    //upload to GPU texture
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tw, th, 0, GL_RGB, GL_UNSIGNED_BYTE, texDat);
    glBindTexture(GL_TEXTURE_2D, 0);

    //clear and draw quad with texture (could be in display callback)
    glBindTexture(GL_TEXTURE_2D, tex);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0);
    glVertex2f(x, y);
    glTexCoord2i(0, 1);
    glVertex2f(x, y + h);
    glTexCoord2i(1, 1);
    glVertex2f(x + w, y + h);
    glTexCoord2i(1, 0);
    glVertex2f(x + w, y);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

static const int size = 64;
static float u[2][size][size] = {};
static float v[2][size][size] = {};

void initReact()
{
    std::random_device r;
    std::mt19937 g(r());
    std::uniform_real_distribution<> d(0., 1.);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            u[0][i][j] = 1;
            //if (i > 20 && i < 30 && j > 20 && j < 30)
            //    v[0][i][j] = 1;
        }
    }


    std::normal_distribution<double> distribution(0, 15);
    std::uniform_int_distribution<> integ(0, size);
    for (int l = 0; l < 1; l++)
    {
        int x = integ(g), y = integ(g);
        std::cout << x << " " << y << std::endl;
        for (int k = 0; k < 1000; k++)
        {
            int nx = x + (int) distribution(g), ny = y + (int) distribution(g);
            if (nx < 0 || nx >= size || ny < 0 || ny >= size)
                continue;
            v[0][nx][ny] = d(g); //1;
        }

    }
    /*std::uniform_int_distribution<> i(size, size);
    for(int k = 0; k<10;k++){
        v[0][i(g)][i(g)]=1;
    }*/

}
static int current = 0;

inline float laplacian(const int &i, const int &j, float m[size][size])
{
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
            m[(i + 1) % size][(j - 1) % size]);
}

void iterate(const float &t = 1)
{
    current = 1 - current;
    float Du = 0.2097, Dv = .105, F = 0.021, k = 0.06;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int other = 1 - current;

            float du = Du * laplacian(i, j, u[other]) * u[other][i][j] - u[other][i][j] * v[other][i][j] * v[other][i][j] + F * (1 - u[other][i][j]);
            float dv = Dv * laplacian(i, j, v[other]) * v[other][i][j] + u[other][i][j] * v[other][i][j] * v[other][i][j]-(F + k) * v[other][i][j];

            u[current][i][j] = std::max(0.f, u[other][i][j] + du * t);
            v[current][i][j] = std::max(0.f, v[other][i][j] + dv * t);
        }
    }
}

void renderFunction()
{

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    p1.Draw();
    p2.Draw();
    p3.Draw();

    std::random_device rd;
    std::default_random_engine re(rd());
    std::uniform_real_distribution<double> unif(-1, 1);
    for (unsigned int i = 0; i < preys.size(); i++)
    {
        preys[i].update(preys, predators);
        if (!preys[i].consumeEnergy())
        {
            std::cout << preys[i] << std::endl;
            preys[i] = Boid::individual(&food, &fish2);
            preys[i].setPosition(unif(re), unif(re));
        }
        preys[i].draw();
    }
    for (unsigned int i = 0; i < predators.size(); i++)
    {
        predators[i].update(preys);
        predators[i].eatFood(preys);
        if (!predators[i].consumeEnergy())
        {
            predators[i] = Predator::individual(&fish1);
            predators[i].setPosition(unif(re), unif(re));
        }
        predators[i].draw();
    }
    food.regenerateFood();

    food.Draw();

    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < 2; i++)
        iterate();

    unsigned char texDat[size * size * 3];
    for (int i = 0; i < size * size * 3; i += 3)
    {
        texDat[i] = u[current][i / 3 / size][(i / 3) % size]*255;
        texDat[i + 1] = u[current][i / 3 / size][(i / 3) % size]*255;
        texDat[i + 2] = 0;
    }

    texture(texDat, size, size, -5, -5, 1, 1);


    glFlush();
}

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 30, timer, 0);
}

int main(int argc, char** argv)
{
    initReact();

    // X, Y, QTY
    unsigned int food_sites_sp[][3] = {
        {25, 15, 1000},
        {50, 50, 2000},
        {80, 35, 1000},
    };
    food.generateFoodSandpile(food_sites_sp, sizeof (food_sites_sp) / sizeof (int) / 3);


    std::random_device rd;
    std::default_random_engine re(rd());

    std::uniform_real_distribution<double> unif(-1, 1);
    for (int i = 0; i < 20; i++)
    {
        preys.push_back(Boid::individual(&food, &fish2));
        preys.back().setPosition(unif(re), unif(re));
    }
    for (int i = 0; i < 5; i++)
    {
        predators.push_back(Predator::individual(&fish1));
        predators.back().setPosition(unif(re), unif(re));
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("ALife");
    glutDisplayFunc(renderFunction);
    glEnable(GL_BLEND); // Enable transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    timer(0);
    glutMainLoop();
    return 0;
}