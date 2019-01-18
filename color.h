//
// Created by mushfiq on 1/18/19.
//

#ifndef OFFLINE4_COLOR_H
#define OFFLINE4_COLOR_H


class color
{
public:
    double r{}, g{}, b{};
    color(double r, double g, double b) : r(r), g(g), b(b)
    {
    }

    color()
    = default;
};


#endif //OFFLINE4_COLOR_H
