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

    color operator*(float m) const {
        return color(r * m, g * m, b * m);
    }

    color operator+(const color &c) const {
        return color(r + c.r, g + c.g, b + c.b);
    }
};

inline ostream &operator<<(ostream &stream, const color &c) {
    stream << c.r << " " << c.g << " " << c.b;
    return stream;
}

#endif //OFFLINE4_COLOR_H
