//
// Created by mushfiq on 1/20/19.
//

#include "spotlight.h"

spotlight::spotlight(point pos, double fo, point la, double angle) :
        light_source(pos, fo), looking_at(la), cutoff_angle(angle) {
    direction = (looking_at - pos).get_direction_vector();
}
