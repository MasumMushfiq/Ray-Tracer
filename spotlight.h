//
// Created by mushfiq on 1/20/19.
//

#ifndef OFFLINE4_SPOTLIGHT_H
#define OFFLINE4_SPOTLIGHT_H


#include "light_source.h"

struct spotlight : public light_source {
    point looking_at;
    double cutoff_angle;   // angle in degree

    vector_3d direction;

    spotlight() = default;

    spotlight(point pos, double fo, point la, double angle);
};


#endif //OFFLINE4_SPOTLIGHT_H
