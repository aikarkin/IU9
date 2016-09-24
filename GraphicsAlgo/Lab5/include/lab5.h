#include <GL/glut.h>
#include <cmath>
#include "Polygon.h"

bool clippersActivated = false;
bool finished = false;
int clipperPointsCount = 0;
Polygon polygon;
Point2i clipperBegin, clipperEnd;

void drawInternalArea();
void drawLine(Point2i, Point2i, COLOR);
void drawPolygon();
void draw();
