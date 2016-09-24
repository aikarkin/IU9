#include <vector>
#include <iostream>
#include <GLFW/glfw3.h>
#include "Point.h"

enum COLOR { WHITE, BLACK, RED, GREEN, BLUE };

class Contour {
private:
    std::vector<Point2i> points;
    std::size_t _size;
public:
    std::size_t size() { return _size; };
    void addVertex(Point2i vertex);
    void draw();
    Point2i getVertex(int index);
};


void drawLine(Point2i p1, Point2i p2, COLOR color);
