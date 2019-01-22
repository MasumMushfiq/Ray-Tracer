//
// Created by mushfiq on 1/18/19.
//

#include "pyramid.h"

void pyramid::complete_pyramid() {
    upper_left = lower_right = upper_right = top = lower_left;
    upper_left.y += width;
    lower_right.x += width;
    upper_right.x += width;
    upper_right.y += width;

    top.x += width / 2.0;
    top.y += width / 2.0;
    top.z += height;
}

istream &operator>>(istream &stream, pyramid &s) {
    stream >> s.lower_left;
    stream >> s.width >> s.height;
    stream >> s.colour.r >> s.colour.g >> s.colour.b;
    stream >> s.ambient >> s.diffuse >> s.specular >> s.reflection;
    stream >> s.shininess;
    return stream;
}

ostream &operator<<(ostream &stream, const pyramid &p) {
    stream << p.upper_left << p.upper_right << endl;
    stream << p.lower_left << p.lower_right << endl;
    stream << p.top << endl;
    return stream;
}
