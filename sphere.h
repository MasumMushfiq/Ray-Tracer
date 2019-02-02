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

    // normal unit vector at point p where p is on the sphere
    vector_3d get_normal_at_point(const point &p) const;
    bool is_inside(const point &p) const;
};

istream &operator>>(istream &stream, sphere &s);

ostream &operator<<(ostream &stream, const sphere &s);

#endif //OFFLINE4_SPHERE_H
