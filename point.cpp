#include <iostream>
#include "point.h"

point::point() {
    x = y = z = 0;
}

point::point(double x, double y, double z)
        : x(x), y(y), z(z) {
}

void point::print_point() const {
    // printf("x: %.2lf, y: %.2lf, z: %.2lf\n", x, y, z);
    printf("(x, y, z) = (%.2lf, %.2lf, %.2lf)\n", x, y, z);

}

point point::add_vector(vector_3d v) const {
    return point(x + v.i, y + v.j, z + v.k);
}

vector_3d point::gen_vector(point another) const {
    return vector_3d(another.x - x, another.y - y, another.z - z);
}


point point::operator+(vector_3d v) const {
    return point(x + v.i, y + v.j, z + v.k);
}

vector_3d point::operator-(point p) const {
    return vector_3d(x - p.x, y - p.y, z - p.z);
}

istream &operator>>(istream &stream, point &obj) {
    stream >> obj.x >> obj.y >> obj.z;
    return stream;
}

ostream &operator<<(ostream &stream, const point &obj) {
    stream << "<" << obj.x << " " << obj.y << " " << obj.z << "> ";
    return stream;
}