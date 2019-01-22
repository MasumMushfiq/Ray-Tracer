//
// Created by mushfiq on 1/22/19.
//

#ifndef OFFLINE4_CUBE_H
#define OFFLINE4_CUBE_H

#include <iostream>

#include "point.h"
#include "color.h"

struct cube {
    point bll, blr, bul, bur;
    point tll, tlr, tul, tur;
    double side{};
    color colour{};
    double ambient{}, diffuse{}, specular{}, reflection{};
    double shininess{};

    cube() = default;

    void complete_cube();
};

istream &operator>>(istream &stream, cube &s);

ostream &operator<<(ostream &stream, const cube &s);

#endif //OFFLINE4_CUBE_H
