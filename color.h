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

inline istream &operator>>(istream &stream, color &c) {
    stream >> c.r >> c.g >> c.b;
    if(c.r > 1.01) c.r /= 255.0f;
    if(c.g > 1.01) c.g /= 255.0f;
    if(c.b > 1.01) c.b /= 255.0f;
    return stream;
}

inline ostream &operator<<(ostream &stream, const color &c) {
    stream << c.r << " " << c.g << " " << c.b;
    return stream;
}

#endif //OFFLINE4_COLOR_H
