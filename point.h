#ifndef POINT_H
#define POINT_H

#include "vector_3d.h"

using namespace std;

class point {
public:
    double x, y, z;

public:
    point();

    point(double x, double y, double z);

    void print_point() const;

    point add_vector(vector_3d v) const;

    vector_3d gen_vector(point another) const;

    double get_distance(const point &other) const;

    point operator+(vector_3d v) const;

    vector_3d operator-(point p) const;
};

istream &operator>>(istream &stream, point &obj);

ostream &operator<<(ostream &stream, const point &obj);

#endif