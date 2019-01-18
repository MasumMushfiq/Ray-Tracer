//
// Created by mushfiq on 1/18/19.
//

#ifndef OFFLINE4_SPHERE_H
#define OFFLINE4_SPHERE_H


#include "point.h"
#include "color.h"

struct sphere {
    point center{};
    double radius{};
    color colour{};
    double ambient{}, diffuse{}, specular{}, reflection{};
    double shininess{};

    sphere() = default;
};

istream& operator>>(istream &stream, sphere &s);
ostream &operator<<(ostream &stream, const sphere &s);
#endif //OFFLINE4_SPHERE_H
