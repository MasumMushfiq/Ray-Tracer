#include <iostream>
#include <cmath>
#include "vector_3d.h"

using namespace std;

vector_3d::vector_3d() {
    i = 0;
    j = 0;
    k = 0;
}

vector_3d::vector_3d(double i, double j, double k)
        : i(i), j(j), k(k) {
}

void vector_3d::print_vector_3d() const {
    // printf("i: %.2lf, j: %.2lf, k: %.2lf\n", i, j, k);
    printf("%.2lfi + %.2lfj + %.2lfk\n", i, j, k);
}

vector_3d vector_3d::add(vector_3d another) const {
    double x = i + another.i;
    double y = j + another.j;
    double z = k + another.k;
    return vector_3d(x, y, z);
}

vector_3d vector_3d::subtract(vector_3d another) const {
    double x = i - another.i;
    double y = j - another.j;
    double z = k - another.k;
    return vector_3d(x, y, z);
}

vector_3d vector_3d::scale(double m) const {
    double x = i * m;
    double y = j * m;
    double z = k * m;
    return vector_3d(x, y, z);
}

double vector_3d::dot(vector_3d another) const {
    double x = i * another.i;
    double y = j * another.j;
    double z = k * another.k;
    return x + y + z;
}

vector_3d vector_3d::cross(vector_3d another) const {
    double x = j * another.k - k * another.j;
    double y = k * another.i - i * another.k;
    double z = i * another.j - j * another.i;

    return vector_3d(x, y, z);
}

double vector_3d::length() const {
    return sqrt(i * i + j * j + k * k);
}

vector_3d vector_3d::normalize() const {
    double l = length();
    double x = i / l;
    double y = j / l;
    double z = k / l;

    return vector_3d(x, y, z);
}

double vector_3d::angle_2d(vector_3d another) const {
    double a = i * another.i + j * another.j;
    double l1 = sqrt(i * i + j * j);
    double l2 = sqrt(another.i * another.i + another.j * another.j);
    return acos(a / (l1 * l2)) * 180 / PI;
}

double vector_3d::angle_3d(vector_3d another) const {
    vector_3d a = normalize();
    vector_3d b = another.normalize();
    double dot_val = a.dot(b);
    double radian_angle = acos(dot_val);
    return radian_angle * 180 / PI;
}

vector_3d vector_3d::perp_2d() const {
    return vector_3d(-j, i, k);
}

vector_3d vector_3d::perp_3d() const {
    /* 
        angle between u and v is 60 degree.
        So no line can be parallel to both of these lines.
    */
    vector_3d u(1, 1, 0);
    vector_3d v(0, 1, 1);
    if (angle_3d(u) < 1.0) {
        return cross(v);
    }
    return cross(u);
}

vector_3d vector_3d::rotate_2d(double angle_degree) const {
    double angle = angle_degree / 180 * PI;
    vector_3d perp = perp_2d();
    vector_3d h_comp = scale(cos(angle));
    vector_3d v_comp = perp.scale(sin(angle));
    return h_comp.add(v_comp);
}

vector_3d vector_3d::rotate_3d(double angle_degree, vector_3d axis) const {
    double angle = angle_degree / 180 * PI;
    vector_3d perp = axis.normalize().cross(*this);
    vector_3d h_comp = scale(cos(angle));
    vector_3d v_comp = perp.scale(sin(angle));
    return h_comp.add(v_comp);
}

pair<vector_3d, vector_3d> vector_3d::orthogonal_projection(vector_3d v) const {
    vector_3d v_unit = v.normalize();
    double h_length = dot(v_unit);
    vector_3d h_comp = v_unit.scale(h_length);
    vector_3d v_comp = subtract(h_comp);
    return {h_comp, v_comp};
}

vector_3d vector_3d::reflect(vector_3d normal) const {
    vector_3d n_unit = normal.normalize();
    double val = dot(n_unit);
    vector_3d u = n_unit.scale(2 * val);
    return subtract(u);
}

vector_3d vector_3d::operator+(const vector_3d &another) const {
    double x = i + another.i;
    double y = j + another.j;
    double z = k + another.k;
    return vector_3d(x, y, z);
}

vector_3d vector_3d::operator-(const vector_3d &another) const {
    double x = i - another.i;
    double y = j - another.j;
    double z = k - another.k;
    return vector_3d(x, y, z);
}

vector_3d vector_3d::operator*(double m) const {
    double x = i * m;
    double y = j * m;
    double z = k * m;
    return vector_3d(x, y, z);
}