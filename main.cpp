#include "GL/freeglut.h"
#include "GL/gl.h"
#include <cmath>
#include <vector>
#include <map>
#include <iostream>

struct Point
{
    double x, y, z;

    Point(double x, double y, double z) : x(x), y(y), z(z)
    {
    };
};

struct Color
{
    double r, g, b;

    Color(double r, double g, double b) : r(r), g(g), b(b)
    {
    };
};

inline int positive_modulo(int i, int n)
{
    return (i % n + n) % n;
}

#include "TurtleInterpreter.h"
#include "LSystem.h"
#include "Animal.h"
#include "Plant.h"


std::vector<Point> fish1{
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

std::vector<Point> fish2{
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

const double mini = .1, maxi = .2, step = .01;

std::vector<double> ps;
std::vector<double> xs;
std::vector<double> ys;
int i = 0;



Plant p1(Turtle(.0035, 20, -.5, -1, 1.5, .05), LSystem('X',{
    {'F',
        {"FF"}},
    {'X',
        {"F[+X]F[-X]+X"}}
}), 7, Color(43. / 255, 112. / 255, 46. / 255));

Plant p2(Turtle(.02, 20, .5, -1, .3, .1), LSystem('F',{
    {'F',
        {"FF-[-F+F+F]+[+F-F-F]"}}
}), 4, Color(43. / 255, 112. / 255, 46. / 255));


Plant p3(Turtle(.02, 20, 0, -1, 1, .1), LSystem('F',{
    {'F',
        {"F[+F]F[-F]F", "F[+F]F", "F[-F]F"}}
}), 5, Color(96. / 255, 140. / 255, 93. / 255));
Animal a1(fish1, mini, mini);
Animal a2(fish2, mini, mini);

#include "Food.h"
Food* food = new Food(100);
#include "Boid.h"
std::vector<Boid> preys;

// X, Y, STD, QTY
unsigned int food_sites[][4] = {
    {25, 15, 3, 5000},
    {50, 50, 5, 20000},
    {80, 35, 4, 5000},
};

void renderFunction()
{
    a1.setScaleX(xs[i]);
    a1.setScaleY(ys[i]);
    a1.setPerspective(ps[i]);
    a2.setScaleX(xs[i]);
    a2.setScaleY(ys[i]);
    a2.setPerspective(ps[i]);
    i++;
    i %= xs.size();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    p1.Draw();
    p2.Draw();
    p3.Draw();

    a1.Draw(.5, .5);

    for (unsigned int i = 0; i < preys.size(); i++)
    {
        preys[i].update(preys, a2);
        if (!preys[i].consumeEnergy())
        {
            preys.erase(preys.begin() + i);
        }
    }
    food->regenerateFood();

    food->Draw();

    glFlush();
}

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 30, timer, 0);
}

int main(int argc, char** argv)
{
    for (double x = mini; x < maxi; x += step * 15)
    {
        for (double y = mini; y < maxi; y += step * 15)
        {
            for (double p = -.15; p <= .15; p += .0075)
            {
                ps.push_back(p);
                xs.push_back(x);
                ys.push_back(y);
            }
        }
    }

    food->generateFood(food_sites, sizeof (food_sites) / sizeof (int) / 4);
    std::random_device rd;
    std::default_random_engine re(rd());

    std::uniform_real_distribution<double> unif(-1, 1);
    for (int i = 0; i < 10; i++)
    {
        preys.push_back(Boid(food, unif(re), unif(re)));
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1500, 900);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ALife");
    glutDisplayFunc(renderFunction);
    glEnable(GL_BLEND); // Enable transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    timer(0);
    glutMainLoop();
    delete food;
    return 0;
}
