#ifndef TOOLS_H
#define TOOLS_H

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


#endif /* TOOLS_H */

