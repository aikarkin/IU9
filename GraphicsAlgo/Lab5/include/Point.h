struct Point2i {
    int x, y;
    Point2i(int _x, int _y) : x(_x), y(_y) {};
    Point2i() : x(0), y(0) {};
    Point2i operator+(Point2i &p2);
    Point2i operator-(Point2i &p2);
    bool isNil();
};

struct Edge {
    Point2i begin, end;
    Edge(Point2i _begin, Point2i _end) : begin(_begin), end(_end) {};
    Edge();
};