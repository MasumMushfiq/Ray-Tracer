//
// Created by mushfiq on 1/18/19.
//

#ifndef OFFLINE4_PYRAMID_H
#define OFFLINE4_PYRAMID_H

#include <fstream>
#include "point.h"
#include "color.h"

struct pyramid {
    point lower_left{}, upper_left{}, lower_right{}, upper_right{}, top{};
    double width{}, height{};

    color colour{};
    double ambient{}, diffuse{}, specular{}, reflection{};
    double shininess{};

    pyramid() = default;

    void complete_pyramid();
};

istream &operator>>(istream &stream, pyramid &s);

ostream &operator<<(ostream &stream, const pyramid &s);


#endif //OFFLINE4_PYRAMID_H
