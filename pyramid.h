//
// Created by mushfiq on 1/18/19.
//

#ifndef OFFLINE4_PYRAMID_H
#define OFFLINE4_PYRAMID_H

#include <fstream>
#include "point.h"
#include "color.h"

struct pyramid {
    point left_lower{}, left_upper{}, right_lower{}, right_upper{}, top{};
    double width{}, height{};

    color colour{};
    double ambient{}, diffuse{}, specular{}, reflection{};
    double shininess{};

    pyramid() = default;

    void complete_pyramid();
};

istream& operator>>(istream &stream, pyramid &s);
ostream &operator<<(ostream &stream, const pyramid &s);


#endif //OFFLINE4_PYRAMID_H
