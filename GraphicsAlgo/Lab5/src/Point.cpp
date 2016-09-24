#include "../include/Point.h"

Point2i Point2i::operator+(Point2i &p2) {
    return Point2i(x + p2.x, y + p2.y);
}

Point2i Point2i::operator-(Point2i &p2) {
    return Point2i(x - p2.x, y - p2.y);
}

bool Point2i::isNil() {
    return (x==0 && y==0);
}