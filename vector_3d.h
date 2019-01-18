#ifndef VECTOR_3D_H
#define VECTOR_3D_H
#include <utility>

const double PI = 3.1415926535897932;

using namespace std;

class vector_3d
{
public:
  double i, j, k;

public:
  vector_3d();
  vector_3d(double i, double j, double k);
  void print_vector_3d() const;

  vector_3d add(vector_3d another) const;
  vector_3d subtract(vector_3d another) const;
  double dot(vector_3d another) const;
  vector_3d cross(vector_3d another) const;
  vector_3d scale(double m) const;
  double length() const;
  vector_3d normalize() const;

  double angle_2d(vector_3d another) const; // angle considering only x and y
  double angle_3d(vector_3d another) const; // angle between two vectors in degree

  vector_3d perp_2d() const; // returns a perpendicular vector ignoring z axis
  vector_3d perp_3d() const; //returns a 3d perpendicular vector

  vector_3d rotate_2d(double angle) const;                 //angle in degree
  vector_3d rotate_3d(double angle, vector_3d axis) const; //angle in degree, axis perp to vector

  pair<vector_3d, vector_3d> orthogonal_projection(vector_3d v) const;

  vector_3d reflect(vector_3d normal) const;
};

#endif