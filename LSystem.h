#ifndef LSYSTEM_H
#define LSYSTEM_H

class LSystem {
    char init;
    std::map<char, std::string> rules;
public:

    LSystem(const char &init, const std::map<char, std::string> &rules) : init(init), rules(rules) {
    };

    void generate(const int n, std::string &out) const {
        std::vector<char> prev, curr;

        prev.push_back(init);

        for (unsigned int i = 0; i < n; i++) {
            for (const char &c : prev) {
                if (rules.find(c) != rules.end()) {
                    for (const char &r : rules.find(c)->second)
                        curr.push_back(r);
                } else {
                    curr.push_back(c);
                }
            }
            prev = curr;
            curr = std::vector<char>();
        }
        out = std::string(prev.size(), ' ');
        for (unsigned int i = 0; i < prev.size(); i++) {
            out[i] = prev[i];
        }
    }
};

#endif /* LSYSTEM_H */

