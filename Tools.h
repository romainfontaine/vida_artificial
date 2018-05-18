#ifndef TOOLS_H
#define TOOLS_H

#include "GL/freeglut.h"
#include "GL/gl.h"


#include <string>

struct Point {
    double x, y, z;

    Point(double x, double y, double z) : x(x), y(y), z(z) {
    };
};

struct Color {
    double r, g, b;

    Color(double r, double g, double b) : r(r), g(g), b(b) {
    };
};

inline int positive_modulo(int i, int n) {
    return (i % n + n) % n;
}

inline void displayText(const float &x, const float &y, const float &r,
        const float &g, const float &b, const std::string &string) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (unsigned int i = 0; i < string.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}

inline void drawCircle(const float &x, const float &y, const float &r,
        const float &red = 1, const float &green = 1, const float&blue = 1) {
    glColor3f(red, green, blue);
    glBegin(GL_LINE_LOOP);
    for (double i = 0; i < 2 * M_PI; i += M_PI / 12)
        glVertex2f(x + cos(i) * r, y + sin(i) * r);
    glEnd();
}

template<class T>
inline T clamp(const T &a,const T &min, const T &max){
    return std::max(min, std::min(a, max));
}

#endif /* TOOLS_H */

