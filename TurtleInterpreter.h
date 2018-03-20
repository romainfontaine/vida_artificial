#ifndef TURTLEINTERPRETER_H
#define TURTLEINTERPRETER_H


#include "Turtle.h"

class TurtleInterpreter {
public:
    std::vector<Turtle> ts;

    TurtleInterpreter(const Turtle &t) {
        ts.push_back(t);
    };

    void exec(const std::string &s) {
        for (unsigned int i = 0; i < s.length(); i++) {
            switch (s[i]) {
                case 'f':
                    ts.back().f();
                    break;
                case 'F':
                    ts.back().F();
                    break;
                case '+':
                    ts.back().plus();
                    break;
                case '-':
                    ts.back().minus();
                    break;
                case '[':
                    ts.push_back(ts.back());
                    break;
                case ']':
                    ts.pop_back();
                    if (ts.size() == 0) {
                        std::cerr << "TurtleInterpreter Error - more pops than pushes." << std::endl;
                        exit(1);
                    }
                    break;
            }
        }
    }
};

#endif /* TURTLEINTERPRETER_H */

