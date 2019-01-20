//
// Created by mushfiq on 1/18/19.
//

#ifndef OFFLINE4_COLOR_H
#define OFFLINE4_COLOR_H

#include <iostream>

class color {
public:
    float r{}, g{}, b{};

    color(float r, float g, float b) : r(r), g(g), b(b) {
    }

    color()
    = default;
};

inline ostream &operator<<(ostream &stream, const color &c) {
    stream << c.r << " " << c.g << " " << c.b;
    return stream;
}

#endif //OFFLINE4_COLOR_H
