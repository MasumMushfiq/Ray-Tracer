//
// Created by mushfiq on 1/20/19.
//

#ifndef OFFLINE4_LIGHT_SOURCE_H
#define OFFLINE4_LIGHT_SOURCE_H

#include <iostream>
#include "point.h"

struct light_source {
    point position;
    double fall_off;

    light_source() = default;

    light_source(point p, double fo);
};


#endif //OFFLINE4_LIGHT_SOURCE_H
