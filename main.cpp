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

//std::vector<Point> shape {{0,0,0},{.1,.1,0},{.2,0,0},{.1,-.1,0}};
std::vector<Point> shape{
    {0, 0, 1},
    {13, 51, 1},
    {44, 95, 1},
    {101, 134, 1},
    {164, 142, 1},
    {257, 125, 1},
    {321, 84, 1},
    {415, 154, 1},
    {397, 0, 1},
    {415, -154, 1},
    {321, -84, 1},
    {257, -125, 1},
    {164, -142, 1},
    {101, -134, 1},
    {44, -95, 1},
    {13, -51, 1},
    {0, 0, 1}
};

void drawShape(const std::vector<Point> points)
{
    glBegin(GL_LINE_LOOP);
    for (const Point &p : points)
        glVertex3d(p.x * 0.001, p.y * 0.001, p.z);
    glEnd();
    glFlush();
}

/* display function - code from:
     http://fly.cc.fer.hr/~unreal/theredbook/chapter01.html */
void renderFunction()
{
    //Turtle t(.02, 22.5);
    //Turtle t(.02, 20);
    Turtle t(.0035, 20);
    TurtleInterpreter ti(t);

    std::string out;
    //expandExp('F', {{'F',"FF-[-F+F+F]+[+F-F-F]"}}, 4, out);
    //expandExp('F', {{'F',"F[+F]F[-F][F]"}}, 5, out);
    LSystem ls('X',{
        {'F', "FF"},
        {'X', "F[+X]F[-X]+X"}
    });
    ls.generate(7, out);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glBegin(GL_LINES);
    ti.exec(out);

    glEnd();
    drawShape(shape);
    glFlush();
}

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 30, timer, 0);
}

int main(int argc, char** argv)
{
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
