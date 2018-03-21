#ifndef ANIMAL_H
#define ANIMAL_H

class Animal {
private:
    std::vector<Point> shape;
    double scaleX, scaleY;
public:

    Animal(const std::vector<Point> &shape, const double &scaleX = 1, const double &scaleY = 1) :
    shape(shape), scaleX(scaleX), scaleY(scaleY) {

    }

    void setScaleX(const double &scaleX) {
        this->scaleX = scaleX;
    }

    void setScaleY(const double &scaleY) {
        this->scaleY = scaleY;
    }

    void Draw(double x = 0, double y = 0) {
        glColor3d(1., 1., 1.);
        glBegin(GL_LINE_LOOP);
        for (const Point &p : shape)
            glVertex3d(x + p.x * scaleX, y + p.y * scaleY, p.z);
        glEnd();
    }
};

#endif /* ANIMAL_H */

