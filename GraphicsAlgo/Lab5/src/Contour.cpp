#include "../include/Contour.h"

void drawLine(Point2i p1, Point2i p2, COLOR color) {
    if(p1.isNil() || p2.isNil())
        return;
    switch(color) {
        case WHITE:
            glColor3f(1.0, 1.0, 1.0);
            break;
        case BLACK:
            glColor3f(0.0, 0.0, 0.0);
            break;
        case RED:
            glColor3f(1.0, 1.0, 1.0);
            break;
        case GREEN:
            glColor3f(0.0, 1.0, 0.0);
            break;
        case BLUE:
            glColor3f(0.0, 0.0, 1.0);
            break;
        default:
            break;
    }

    glBegin(GL_LINES);
    glVertex2i(p1.x, p1.y);
    glVertex2i(p2.x, p2.y);
    glEnd();
}

void Contour::addVertex(Point2i vertex) {
    points.push_back(vertex);
    _size++;
}

Point2i Contour::getVertex(int index) {
    if(index < 0 || index >= points.size()) {
        std::cerr << "ERROR: can't get vertex, index out of range" << std::endl;
        return Point2i();
    }
    return points[index];
}

void Contour::draw() {
    if(points.size() < 2)
        return;
    for (int i = 1; i < points.size(); ++i) {
        drawLine(points[i - 1], points[i], WHITE);
    }
    drawLine(points.back(), points.front(), WHITE);
}