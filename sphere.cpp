//
// Created by mushfiq on 1/18/19.
//

#include <fstream>
#include "sphere.h"


istream &operator>>(istream &stream, sphere &s) {
    stream >> s.center;
    stream >> s.radius;
    stream >> s.colour.r >> s.colour.g >> s.colour.b;
    stream >> s.ambient >> s.diffuse >> s.specular >> s.reflection;
    stream >> s.shininess;
    return stream;
}

ostream &operator<<(ostream &stream, const sphere &s) {
    stream << s.center.x << " " << s.center.y << " " << s.center.z << endl;
    return stream;
}