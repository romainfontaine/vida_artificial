#ifndef PLANT_H
#define PLANT_H

class Plant {
    std::string repr;
    LSystem l;
    Turtle t;
    Color c;
public:

    Plant(const Turtle &t, const LSystem &l, const int &n, const Color &c = {1, 1, 1}) : l(l), t(t), c(c) {
        l.generate(n, repr);
    }

    void Draw() {
        glColor3d(c.r, c.g, c.b);
        TurtleInterpreter ti(t);
        ti.Draw(repr);
    }
};


#endif /* PLANT_H */

