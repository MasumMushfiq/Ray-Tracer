//
// Created by mushfiq on 1/18/19.
//

#include "pyramid.h"

void pyramid::complete_pyramid() {
    left_upper = right_lower = right_upper = top = left_lower;
    left_upper.y += width;
    right_lower.x += width;
    right_upper.x += width;
    right_upper.y += width;

    top.x += width / 2.0;
    top.y += width / 2.0;
    top.z += height;
}

istream &operator>>(istream &stream, pyramid&s) {
    stream >> s.left_lower;
    stream >> s.width >> s.height;
    stream >> s.colour.r >> s.colour.g >> s.colour.b;
    stream >> s.ambient >> s.diffuse >> s.specular >> s.reflection;
    stream >> s.shininess;
    return stream;
}

ostream &operator<<(ostream &stream, const pyramid &p){
    stream << p.left_upper << p.right_upper;
    stream << p.left_lower << p.right_lower;
    stream << p.top;
    }
