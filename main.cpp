#include "GL/freeglut.h"
#include "GL/gl.h"
#include <cmath>
#include <vector>
#include <map>
#include <iostream>

#include "TurtleInterpreter.h"
#include "LSystem.h"

struct Point
{
    double x, y, z;

    Point(double x, double y, double z) : x(x), y(y), z(z)
    {
    };
};

class Animal
{
private:
    std::vector<Point> shape;
    double scaleX, scaleY;
public:

    Animal(const std::vector<Point> &shape, const double &scaleX = 1, const double &scaleY = 1) :
    shape(shape), scaleX(scaleX), scaleY(scaleY)
    {

    }

    void setScaleX(const double &scaleX)
    {
        this->scaleX = scaleX;
    }

    void setScaleY(const double &scaleY)
    {
        this->scaleY = scaleY;
    }

    void Draw(double x = 0, double y = 0)
    {
        glBegin(GL_LINE_LOOP);
        for (const Point &p : shape)
            glVertex3d(x + p.x * scaleX, y + p.y * scaleY, p.z);
        glEnd();
    }
};

class Plant
{
    std::string repr;
    LSystem l;
    Turtle t;
public:

    Plant(const Turtle &t, const LSystem &l, const int &n) : l(l), t(t)
    {
        l.generate(n, repr);
    }

    void Draw()
    {
        TurtleInterpreter ti(t);
        ti.Draw(repr);
    }
};


std::vector<Point>shape{
    {0.5, 0, 1},
    {0.4686746988, 0.1655844156, 1},
    {0.3939759036, 0.3084415584, 1},
    {0.256626506, 0.4350649351, 1},
    {0.1048192771, 0.461038961, 1},
    {-0.1192771084, 0.4058441558, 1},
    {-0.2734939759, 0.2727272727, 1},
    {-0.5, 0.5, 1},
    {-0.456626506, 0, 1},
    {-0.5, -0.5, 1},
    {-0.2734939759, -0.2727272727, 1},
    {-0.1192771084, -0.4058441558, 1},
    {0.1048192771, -0.461038961, 1},
    {0.256626506, -0.4350649351, 1},
    {0.3939759036, -0.3084415584, 1},
    {0.4686746988, -0.1655844156, 1},
    {0.5, 0, 1}
};

std::vector<Point> shape2{
    {-0.4981884058, 0.2106598985, 1},
    {-0.3713768116, 0.0837563452, 1},
    {-0.0489130435, 0.2664974619, 1},
    {-0.0307971014, 0.3527918782, 1},
    {-0.027173913, 0.4492385787, 1},
    {-0.0253623188, 0.5, 1},
    {0.0905797101, 0.3020304569, 1},
    {0.1956521739, 0.2868020305, 1},
    {0.2844202899, 0.2461928934, 1},
    {0.4130434783, 0.1294416244, 1},
    {0.4673913043, 0.0329949239, 1},
    {0.5, -0.0634517766, 1},
    {0.5, -0.1192893401, 1},
    {0.4710144928, -0.1954314721, 1},
    {0.4239130435, -0.2512690355, 1},
    {0.3731884058, -0.2969543147, 1},
    {0.2608695652, -0.3426395939, 1},
    {0.2355072464, -0.4898477157, 1},
    {0.2264492754, -0.5, 1},
    {0.2173913043, -0.5, 1},
    {0.2065217391, -0.4746192893, 1},
    {0.1884057971, -0.3934010152, 1},
    {0.1902173913, -0.3578680203, 1},
    {-0.009057971, -0.3274111675, 1},
    {-0.0307971014, -0.3730964467, 1},
    {-0.0579710145, -0.4137055838, 1},
    {-0.0778985507, -0.4137055838, 1},
    {-0.0815217391, -0.3934010152, 1},
    {-0.0797101449, -0.3578680203, 1},
    {-0.0724637681, -0.307106599, 1},
    {-0.1721014493, -0.2766497462, 1},
    {-0.2282608696, -0.3527918782, 1},
    {-0.2445652174, -0.2563451777, 1},
    {-0.3043478261, -0.2157360406, 1},
    {-0.3423913043, -0.2055837563, 1},
    {-0.4420289855, -0.4340101523, 1},
    {-0.4583333333, -0.4441624365, 1},
    {-0.4655797101, -0.4289340102, 1},
    {-0.4673913043, -0.3883248731, 1},
    {-0.4565217391, -0.2258883249, 1},
    {-0.4583333333, -0.1091370558, 1},
    {-0.4692028986, 0.0126903553, 1},
    {-0.4800724638, 0.0939086294, 1},
    {-0.5, 0.2055837563, 1}
};

bool XInc = true, YInc = true;
const double mini = .5, maxi = 1.1, step = .05;
double currX = mini, currY = mini;

Plant p1(Turtle(.0035, 20, -.5, -1, .5, .1), LSystem('X',{
    {'F', "FF"},
    {'X', "F[+X]F[-X]+X"}
}), 7);

Plant p2(Turtle(.02, 20, .5, -1, .5, .2), LSystem('F',{
    {'F', "FF-[-F+F+F]+[+F-F-F]"}
}), 4);
Animal a1(shape, currX, currY);
Animal a2(shape2, currX, currY);

std::vector<double> xs;
std::vector<double> ys;
int i = 0;

/* display function - code from:
     http://fly.cc.fer.hr/~unreal/theredbook/chapter01.html */
void renderFunction()
{
    a1.setScaleX(xs[i]);
    a1.setScaleY(ys[i]);
    a2.setScaleX(xs[i]);
    a2.setScaleY(ys[i]);
    i++;
    i%=xs.size();


    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);



    p1.Draw();
    p2.Draw();

    a1.Draw(.5, .5);
    a2.Draw(-.5, .5);
    glFlush();
}

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 30, timer, 0);
}

int main(int argc, char** argv)
{
    for (double x = mini; x < maxi; x += step)
        for (double y = mini; y < maxi; y += step)
        {
            xs.push_back(x);
            ys.push_back(y);
        }
    for (double x = maxi; x > mini; x -= step)
        for (double y = maxi; y > mini; y -= step)
        {
            xs.push_back(x);
            ys.push_back(y);
        }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1200, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ALife");
    glutDisplayFunc(renderFunction);
    timer(0);
    glutMainLoop();

    return 0;
}
