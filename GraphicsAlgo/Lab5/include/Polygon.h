#include "Contour.h"
#include <algorithm>
using namespace std;

class Polygon {
private:
    int _holesCount;
    bool polygonFormed;
    Point2i pClipperBegin, pClipperEnd;
    vector<Edge> clippers;
    vector<float> interParams;
    vector<Point2i> intersectionPoints;
    vector<Contour> contours;

    void addInterParam(Point2i v1, Point2i v2, Point2i p1, Point2i p2);
public:
    Polygon();
    bool insidePolygon(Point2i p);
    void createHole();
    void addVertex(Point2i vertex);
    Edge getClipper(int ind);
    void addClipper(Point2i _begin, Point2i _end);
    void findIntersectionsWithClippers();
    int holesCount();
    int clipperCount();
    int intersectionsCount() { return (int)intersectionPoints.size(); };
    void block();
    void erase();
    Point2i getIntersectionPoint(int index);
    Contour &getHole(int ind);
    Contour &getContour();
};

int sign(int);
int scalar(Point2i, Point2i);
int side(Point2i, Point2i, Point2i);
bool intersected(Point2i, Point2i, Point2i, Point2i);
float intersectionParam(Point2i, Point2i, Point2i, Point2i);

