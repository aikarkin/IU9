//
// Created by alex on 5/20/16.
//

#ifndef GRAPHICSALGO_LAB4_H
#define GRAPHICSALGO_LAB4_H

#endif //GRAPHICSALGO_LAB4_H
#include <algorithm>
#include <vector>

using namespace std;

enum INTER_TYPE { DEFAULT, LINE, NONE};

struct Intersection{
    int val;
    INTER_TYPE type;
    Intersection(int &_val, INTER_TYPE _type) : val(_val), type(_type) {};
    Intersection() : val(0), type(NONE) {};
};

struct Point2i {
    int x, y;
    Point2i(int _x, int _y) : x(_x), y(_y) {};
    Point2i() : x(0), y(0) {};
    Point2i& operator=(Point2i &p) {
        x = p.x;
        y = p.y;
        return *this;
    }
};


struct RGBColor3f {
    GLfloat red, green, blue;
    RGBColor3f(GLfloat _red, GLfloat _green, GLfloat _blue) : red(_red), green(_green), blue(_blue) { }
    RGBColor3f() : red(1.f), green(1.f), blue(1.f) { }
};

const RGBColor3f BLACK(0.f, 0.f, 0.f), WHITE(1.f, 1.f, 1.f);

bool operator==(const RGBColor3f& left, const RGBColor3f& right) {
    return left.red == right.red && left.green==right.green && left.blue == right.blue;
}

bool operator!=(const RGBColor3f& left, const RGBColor3f& right) {
    return !(left==right);
}

void printIntersection(Intersection &inter) {
    cout << "TYPE: ";
    switch(inter.type) {
        case DEFAULT:
            cout << "default";
            break;
        case NONE:
            cout << "none";
            break;
        case LINE:
            cout << "line";
            break;
        default:
            break;
    }
    cout << "; VALUE: " << inter.val << endl;
}

Intersection intersectionWithSegment(Point2i begin, Point2i end, int y) {
    Intersection res;
    res.val = -1;
    float y_offset = 0.5;
    if ((end.y - y - y_offset) * (y + y_offset - begin.y) > 0) {
        res.val = (int) ceil(((float)(end.x - begin.x) / (float) (end.y - begin.y)) * (y + y_offset - begin.y) +
                             (float) begin.x);
        res.type = DEFAULT;
    }
    else if(begin.y == end.y)
        res.type = LINE;
    else
        res.type = NONE;
    return res;
}

class FrameBuffer {
private:
    GLfloat *buf;
    int w, h;
public:
    FrameBuffer(int _width, int _height) : w(_width), h(_height) {
        buf = new GLfloat[3 * w * h];
    }

    void clear() {
        for (int x = 0; x < w; ++x)
            for (int y = 0; y < h; ++y)
                setPixel(Point2i(x, y), BLACK);
    }

    void read() {
        glReadPixels(0, 0, w, h, GL_RGB, GL_FLOAT, buf);
    }

    void show() {
        glDrawPixels(w, h, GL_RGB, GL_FLOAT, buf);
    }

    void setPixel(const Point2i &p, const RGBColor3f &color) {
        try {
            if(p.y > h || p.y < 0 || p.x > w || p.x < 0)
                throw std::out_of_range ("you try write out of the buffer by index: [" + to_string(p.x) + "][" + to_string(p.y)
                                         + "], while buffer size is: " + to_string(w) +"x" + to_string(h));
            buf[3 * ((h - p.y) * w + p.x)] = color.red;
            buf[3 * ((h - p.y) * w + p.x) + 1] = color.green;
            buf[3 * ((h - p.y) * w + p.x) + 2] = color.blue;
        }
        catch (const std::out_of_range& oor) {
            std::cout << "Out of Range error: " << oor.what() << '\n';
            glfwTerminate();
            exit(EXIT_FAILURE);
            return;
        }
    }

    RGBColor3f getPixel(const Point2i &p) {
        try {
            if (p.y > h || p.y < 0 || p.x > w || p.x < 0)
                throw std::out_of_range(
                        "you try get element to the buffer by index: [" + to_string(p.x) + "][" + to_string(p.y)
                        + "], while buffer size is: " + to_string(w) + "x" + to_string(h));
            return RGBColor3f(buf[3 * ((h - p.y) * w + p.x)], buf[3 * ((h - p.y) * w + p.x) + 1],
                              buf[3 * ((h - p.y) * w + p.x) + 2]);
        }
        catch (const std::out_of_range& oor) {
            std::cout << "Out of Range error: " << oor.what() << '\n';
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
    }

    int width() {
        return w;
    }

    int height() {
        return h;
    }

    void resize(int width, int height) {
        delete[] buf;
        buf = new GLfloat[3*width*height];
        w = width;
        h = height;
    }

};

int sign(int val) {
    if (val == 0)
        return 0;
    return val < 0 ? -1 : 1;
}

void drawLine(FrameBuffer *buffer, Point2i p1, Point2i p2) {
    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);
    int s1 = sign(p2.x - p1.x);
    int s2 = sign(p2.y - p1.y);
    bool delta_changed = false;
    int y = p1.y, x=p1.x;

    if(dy > dx) {
        int tmp = dx;
        dx = dy;
        dy = tmp;
        delta_changed = true;
    }

    int e = 2*dy - dx;

    for (int i = 0; i < dx; ++i) {
        buffer->setPixel(Point2i(x, y), WHITE);
        while(e >= 0) {
            if(delta_changed)
                x+=s1;
            else
                y+=s2;
            e-=2*dx;
        }
        if(delta_changed)
            y+=s2;
        else
            x+=s1;
        e+=2*dy;
    }

}


bool pointOnRightSide(Point2i p1, Point2i p2, Point2i m) {
	Point2i v1(p2.x - p1.x, p2.y - p1.y);
	Point2i v2(m.x - p1.x, m.y - p1.y);
	return v1.x * v2.y - v2.x * v1.y < 0;
}

void drawSmoothLine(FrameBuffer *buffer, Point2i p1, Point2i p2) {
    int x1 = p1.x, x2 = p2.x;
    int y1 = p1.y, y2 = p2.y;

    int x = x1, y = y1;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int s1 = sign(x2 - x1);
    int s2 = sign(y2 - y1);
    double intensity = 1.0;

    bool delta_changed = false;

    if(dy > dx) {
        int tmp = dx;
        dx = dy;
        dy = tmp;
        delta_changed = true;
    }

    int e = 2*dy - dx;
    double m;

    if(delta_changed && (s1 == s2) || (!delta_changed && (s1 != s2)))
        m = intensity * dy / (double)dx;
    else
        m = intensity*(1.0 - (double)dy / (double)dx);


    double w = intensity - m;
    double p = intensity / 2.0;
    for (int i = 0; i < dx; ++i) {
        buffer->setPixel(Point2i(x, y), RGBColor3f((float)p, (float)p, (float)p));
        if(p - w < 0.000001f)
            p+=m;
        else
            p-=w;
		if(e > 0.000001f) {
            if (delta_changed)
                x += s1;
            else
                y += s2;
            e-=2*dx;
        }
        if(delta_changed) 
            y+=s2;
        else
            x+=s1; 
        e+=2*dy;
//        buffer->setPixel(Point2i(x, y), RGBColor3f((float)p, (float)p, (float)p));
    }
}

void drawSmoothLine1(FrameBuffer *buffer, Point2i p1, Point2i p2) {
    int x = p1.x;
    int y = p1.y;
    int dx = abs(p2.x - x);
    int dy = abs(p2.y - y);
    int sx = sign(p2.x - x);
    int	sy = sign(p2.y - y);
    int I = 1;
    double e = 0.5;
    double m = (double)(I*dy)/(double)dx;
    bool delta_changed = false;

    if(dy > dx) {
        std::swap(dx, dy);
        m = (double)(I*dx)/(double)dy;
        delta_changed = true;
    }

    double w = I - m;

    while(x!=p2.x || y!=p2.y) {
        cout << "pixel: (" << x << ", " << y << ") e=" << e << endl;
        buffer->setPixel(Point2i(x, y), RGBColor3f(e, e, e));

        x+=sx;
        if(p2.y < p1.y && p2.x < p1.x) {
            if(w - e > 0.00001)
                e+=m;
            else {
                y+=sx;
                e-=w;
            }
        }
        else if( (p2.y < p1.y && p2.x > p1.x)
                 || (p2.y > p1.y && p2.x < p1.x) ) {
            if(e - m > 0.00001)
                e-=m;
            else {
                y-=sx;
                e+=w;
            }
        }
        else {
            if (w - m > 0.00001)
                e+=m;
            else {
                y+=sx;
                e-=w;
            }
        }
    }
}

class Polygon {
private:
    vector<Point2i> points;
    FrameBuffer *frameBuffer;
    bool closed;
    double I;
public:
    Polygon(FrameBuffer *_frameBuffer) : frameBuffer(_frameBuffer), I(1) {
        closed=false;
    };

    Polygon(FrameBuffer *_frameBuffer, double intensity) : frameBuffer(_frameBuffer), I(intensity) {
        closed=false;
    }

    bool addPoint(Point2i p) {
        if (!closed) {
            points.push_back(p);
            if (points.size() > 1) {
				int n = (int)points.size();
				// bool side = pointOnRightSide(points[n - 2], points[n - 1], points[0]);
                bool side = false;
                if(points.size() > 2)
                    side = pointOnRightSide(points[n - 2], points[n - 1], points[n - 3]);
                drawSmoothLine(frameBuffer, points[points.size() - 2], points[points.size() - 1]);
				// drawLine(frameBuffer, points[points.size() - 2], points[points.size() - 1]);
			}
            return true;
        }
        return false;
    }

    void close() {
        closed=true;
		int n = (int)points.size();
		bool side = pointOnRightSide(points[n - 1], points[0], points[1]);
        drawSmoothLine(frameBuffer, points.back(), points[0]);
		//drawLine(frameBuffer, points.back(), points[0]);
    }

    void erase() {
        points.clear();
        closed=false;
    }

    bool clockwise(vector<Point2i> &);

    void fill() {
        if(!closed)
            return;
        int y_min = points[0].y, y_max = y_min;
        int height = frameBuffer->height(), width = frameBuffer->width();

        for (int i = 0; i < points.size(); ++i) {
            if(points[i].y < y_min)
                y_min = points[i].y;
            else if(points[i].y > y_max)
                y_max = points[i].y;
        }

        vector<Intersection> pointsList[y_max - y_min + 2];
        for (int i = 0; i < points.size(); ++i) {
            Point2i p1(points[i].x, points[i].y),
                    p2(points[(i + 1) % points.size()].x, points[(i + 1) % points.size()].y);
            Intersection x_inter;

            for (int y = min(p1.y, p2.y), p_ind = 0; y <= max(p1.y, p2.y); ++y) {
                p_ind = y - y_min;
                x_inter = intersectionWithSegment(p1, p2, y);
                if(x_inter.type == LINE) {
                    pointsList[p_ind].emplace_back(p1.x, LINE);
                    pointsList[p_ind].emplace_back(p2.x, LINE);
                }
                else if(x_inter.type == DEFAULT) {
                    pointsList[p_ind].push_back(x_inter);
                }
            }
        }

        int lst_dist = 0, lst_p = 0;
        for (int i = 0; i < points.size(); ++i) {
            if(points[i].x*points[i].x + points[i].y*points[i].y > lst_dist) {
                lst_dist = points[i].x * points[i].x + points[i].y * points[i].y;
                lst_p = i;
            }
        }
        int prev = lst_p - 1, next = (lst_p + 1)%(int)points.size();
        if(prev < 0)
            prev = (int)points.size() - 1;

        // redraw path in negative direction(clockwise)
        frameBuffer->clear();

        if(!clockwise(points)) {
            for (int i = 1; i < points.size(); ++i)
                drawSmoothLine(frameBuffer, points[i - 1], points[i]);
            drawSmoothLine(frameBuffer, points.back(), points[0]);
        }
        else {
            for (int i = 1; i < points.size(); ++i)
                drawSmoothLine(frameBuffer, points[i], points[i - 1]);
            drawSmoothLine(frameBuffer, points[0], points.back());
        }

		// filling polygon
        for (int y = y_min, i, p_ind; y < y_max; ++y) {
            i=0;
            p_ind = y - y_min;
            sort(pointsList[p_ind].begin(), pointsList[p_ind].end(), [](Intersection a, Intersection b) {
                return a.val < b.val;
            });

            while(i + 1 < pointsList[y-y_min].size()) {
                if(pointsList[p_ind][i].type != NONE) {
                    for (int x = pointsList[p_ind][i].val; x < pointsList[p_ind][i + 1].val; ++x) {
                        if (frameBuffer->getPixel(Point2i(x, y)) == BLACK)
                            frameBuffer->setPixel(Point2i(x, y), WHITE);
                    }
                }
                i+=2;
            }
        }
    }
};
