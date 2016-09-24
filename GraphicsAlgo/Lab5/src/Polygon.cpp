//
// Created by alex on 16.09.16.
//

#include "../include/Polygon.h"

void Polygon::findIntersectionsWithClippers() {
    for(Edge clipper:clippers) {
        pClipperBegin = clipper.begin;
        pClipperEnd = clipper.end;
        interParams.clear();
        for (int j = 0; j < contours.size(); ++j) {
            for (int k = 1; k < contours[j].size(); ++k) {
                addInterParam(contours[j].getVertex(k - 1), contours[j].getVertex(k), pClipperBegin, pClipperEnd);
            }
            addInterParam(contours[j].getVertex((int) (contours[j].size() - 1)), contours[j].getVertex(0),
                          pClipperBegin, pClipperEnd);
        }

        if (insidePolygon(pClipperBegin))
            interParams.push_back(0);
        if (insidePolygon(pClipperEnd))
            interParams.push_back(1);

        sort(interParams.begin(), interParams.end());

        for (int i = 0; i < interParams.size(); ++i) {
            float x = pClipperBegin.x + (pClipperEnd.x - pClipperBegin.x) * interParams[i],
                    y = pClipperBegin.y + (pClipperEnd.y - pClipperBegin.y) * interParams[i];
            intersectionPoints.emplace_back((int) x, (int) y);
        }
    }
}

void Polygon::createHole() {
    if(polygonFormed) {
        return;
    };
    contours.push_back(Contour());
    _holesCount++;
}

void Polygon::addInterParam(Point2i v1, Point2i v2, Point2i p1, Point2i p2) {
    if(intersected(v1, v2, p1, p2)) {
        float t = intersectionParam(v1, v2, p1, p2);
        interParams.push_back(t);
    }
}

Point2i Polygon::getIntersectionPoint(int index) {
    return intersectionPoints[index];
}

bool Polygon::insidePolygon(Point2i p) {
    Point2i v(640, p.y);
    bool res = false;
    for (int i = 0; i < contours.size(); ++i) {
        for (int j = 1; j < contours[i].size(); ++j) {
            if(intersected(contours[i].getVertex(j - 1), contours[i].getVertex(j), p, v))
                res = !res;
        }
        if(intersected(contours[i].getVertex((int)(contours[i].size() - 1)), contours[i].getVertex(0), p, v))
            res = !res;
    }
    return res;
}

Contour &Polygon::getHole(int ind) {
    return contours[ind + 1];
}

int Polygon::holesCount() {
    return _holesCount;
}

Contour &Polygon::getContour() {
    return contours.front();
}

void Polygon::addVertex(Point2i vertex) {
    if(polygonFormed)
        return;

    contours.back().addVertex(vertex);
}

Polygon::Polygon() : polygonFormed(false), _holesCount(0) {
    contours.emplace_back();
}

void Polygon::erase() {
    contours.clear();
    _holesCount = 0;
    pClipperBegin.x = 0, pClipperEnd.y = 0;
    interParams.clear();
    intersectionPoints.clear();
    polygonFormed = false;
    clippers.clear();
    contours.emplace_back();
}

void Polygon::block() {
    polygonFormed = true;
}

void Polygon::addClipper(Point2i _begin, Point2i _end) {
    clippers.emplace_back(_begin, _end);
}

int Polygon::clipperCount() {
    return (int)clippers.size();
}

Edge Polygon::getClipper(int ind) {
    return clippers[ind];
};

bool intersected(Point2i v1, Point2i v2, Point2i p1, Point2i p2) {
    int s1 = side(v1, v2, p1),
            s2 = side(v1, v2, p2);

    if(s1 == 0 && s2 == 0)
        return false;

    int s3 = side(p1, p2, v1),
            s4 = side(p1, p2, v2);

    return ((s1*s2<=0) & (s3*s4<=0));
}

float intersectionParam(Point2i v1, Point2i v2, Point2i p1, Point2i p2) {
    Point2i N(v2.y - v1.y, v1.x - v2.x);
    return (-1)*(float)scalar(p1-v2, N)/(float)scalar(p2 - p1, N);
}

int scalar(Point2i vector1, Point2i vector2) {
    return vector1.x * vector2.x + vector1.y * vector2.y;
}

int sign(int val) {
    if (val == 0)
        return 0;
    return val < 0 ? -1 : 1;
}

int side(Point2i p1, Point2i p2, Point2i m) {
    Point2i N(p2.y - p1.y, p1.x - p2.x); /* нормаль к [P1, P2] */
    return sign(scalar(m - p1, N));
}
