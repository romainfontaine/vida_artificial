#ifndef ANIMAL_H
#define ANIMAL_H

class Animal {
private:
    std::vector<Point> shape;
    double scaleX, scaleY, perspectiveRatio;
public:

    Animal(const std::vector<Point> &shape, const double &scaleX = 1, const double &scaleY = 1, const double &perspectiveRatio = 0) :
    shape(shape), scaleX(scaleX), scaleY(scaleY), perspectiveRatio(perspectiveRatio) {

    }

    void setPerspective(const double &v) {
        this->perspectiveRatio = v;
    }

    void setScaleX(const double &scaleX) {
        this->scaleX = scaleX;
    }

    void setScaleY(const double &scaleY) {
        this->scaleY = scaleY;
    }

    void Draw(double x = 0, double y = 0) const {
        x -= 0.5 * scaleX; // Center
        glColor3d(1., 1., 1.);
        glBegin(GL_LINE_LOOP);
        for (const Point &p : shape) {
            glVertex3d(x + p.x * scaleX, y + (p.y + sign(p.y)*(exp(p.x * perspectiveRatio) - 1)) * scaleY, p.z);


        }
        glEnd();
    }

    static double sign(double d) {
        if (d > .05)
            return 1;
        if (d<-.05)
            return -1;
        return 0;
    }
};

#endif /* ANIMAL_H */

