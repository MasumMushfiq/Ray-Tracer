//
// Created by mushfiq on 1/22/19.
//

#ifndef OFFLINE4_CAMERA_H
#define OFFLINE4_CAMERA_H

#include "point.h"

struct camera {
    point pos;
    vector_3d u, r, l;

    const int CAMERA_POSITION_MOVEMENT = 5; // unit co-ordinate change
    const int CAMERA_ANGLE_MOVEMENT = 5;   // degree to rotate

    camera() {
        init_camera();
    }

    void init_camera() {
        // looking from back
        pos = point(20, -150, 50);

        l = vector_3d(0, 1, 0);
        u = vector_3d(0, 0, 1);
        r = vector_3d(1, 0, 0);

        //looking from front
        /*pos = {20, 350, 50};

        l = {0, -1, 0};
        u = {0, 0, 1};
        r = {-1, 0, 0};*/
    }

    void move_forward() { pos = pos + (l * CAMERA_POSITION_MOVEMENT); }

    void move_backward() { pos = pos + (l * (-1) * CAMERA_POSITION_MOVEMENT); }

    void move_right() { pos = pos + (r * CAMERA_POSITION_MOVEMENT); }

    void move_left() { pos = pos + (r * (-1) * CAMERA_POSITION_MOVEMENT); }

    void move_up() { pos = pos + (u * CAMERA_POSITION_MOVEMENT); }

    void move_down() { pos = pos + (u * (-1) * CAMERA_POSITION_MOVEMENT); }

    void look_left() {
        r = r.rotate_3d(CAMERA_ANGLE_MOVEMENT, u);
        l = l.rotate_3d(CAMERA_ANGLE_MOVEMENT, u);
    }

    void look_right() {
        r = r.rotate_3d(-CAMERA_ANGLE_MOVEMENT, u);
        l = l.rotate_3d(-CAMERA_ANGLE_MOVEMENT, u);
    }

    void look_up() {
        u = u.rotate_3d(CAMERA_ANGLE_MOVEMENT, r);
        l = l.rotate_3d(CAMERA_ANGLE_MOVEMENT, r);
    }

    void look_down() {
        u = u.rotate_3d(-CAMERA_ANGLE_MOVEMENT, r);
        l = l.rotate_3d(-CAMERA_ANGLE_MOVEMENT, r);
    }

    void tilt_clockwise() {
        u = u.rotate_3d(-CAMERA_ANGLE_MOVEMENT, l);
        r = r.rotate_3d(-CAMERA_ANGLE_MOVEMENT, l);
    }

    void tilt_counterclockwise() {
        u = u.rotate_3d(CAMERA_ANGLE_MOVEMENT, l);
        r = r.rotate_3d(CAMERA_ANGLE_MOVEMENT, l);
    }

    void reset() { init_camera(); }

};
#endif //OFFLINE4_CAMERA_H
