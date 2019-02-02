//
// Created by mushfiq on 1/22/19.
//

#include "cube.h"

void cube::complete_cube() {
    bul = bur = blr = bll;

    bul.y += side;

    blr.x += side;

    bur.x += side;
    bur.y += side;

    tll = bll;
    tll.z += side;

    tul = bul;
    tul.z += side;

    tur = bur;
    tur.z += side;

    tlr = blr;
    tlr.z += side;
}

istream &operator>>(istream &stream, cube &s) {
    stream >> s.bll;
    stream >> s.side;
    stream >> s.colour;
    stream >> s.ambient >> s.diffuse >> s.specular >> s.reflection;
    stream >> s.shininess;
    return stream;
}

ostream &operator<<(ostream &stream, const cube &c) {
    stream << c.tul << c.tur << endl;
    stream << c.tll << c.tlr << endl;
    cout << endl;
    stream << c.bul << c.bur << endl;
    stream << c.bll << c.blr << endl;
    return stream;
}