#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "lab4.h"

using namespace std;

FrameBuffer *frameBuffer;
Polygon *polygon;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS)
        return;
    switch (key) {
        case GLFW_KEY_SPACE:
            polygon->close();
            break;
        case GLFW_KEY_F:
            polygon->fill();
            break;
        case GLFW_KEY_R:
            frameBuffer->clear();
            polygon->erase();
            break;
        default:
            break;
    }
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        bool res = polygon->addPoint(Point2i(xpos, ypos));
        if(!res) {
            polygon->erase();
            polygon->addPoint(Point2i(xpos, ypos));
        }
    }
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void init_view(GLFWwindow *window, int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, (float)width,  (float)height, 0, -1.f, 15.f);
    frameBuffer->resize(width, height);
    polygon->erase();
}

int main(void) {
    GLFWwindow *window;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit(EXIT_FAILURE);
    }

    int width = 640, height = 480;
    window = glfwCreateWindow(width, height, "Lab 4", NULL, NULL);
    frameBuffer = new FrameBuffer(width, height);
    polygon = new Polygon(frameBuffer);
    if (!window)
    {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval( 1 );

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);

    glfwSetFramebufferSizeCallback(window, init_view);
    init_view(window, width, height);

    glfwGetWindowSize(window, &width, &height);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        frameBuffer->show();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete []frameBuffer;
    delete []polygon;

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

bool Polygon::clockwise(vector<Point2i> &points) {
    int min = points[0].x, k = 0;

    for (int i = 1; i < points.size(); ++i) {
        if(points[i].x < min) {
            k = i;
            min = points[i].x;
        }
    }

    Point2i p1, p2, p3;
    if(k == 0) {
        p1 = points.back();
        p2 = points[0];
        p3 = points[1];
    }
    else if(k == points.size() - 1) {
        p1 = points[k - 1];
        p2 = points[k];
        p3 = points[0];
    }
    else {
        p1 = points[k - 1];
        p2 = points[k];
        p3 = points[k + 1];
    }

    int triangleArea2 = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    return triangleArea2 < 0;
}

