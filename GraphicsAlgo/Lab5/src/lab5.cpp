#include "../include/lab5.h"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS)
        return;
    switch (key) {
        case GLFW_KEY_C:
            clippersActivated = true;
            polygon.block();
            break;
        case GLFW_KEY_H:
            if(!clippersActivated)
                polygon.createHole();
            break;
        case GLFW_KEY_ENTER:
            polygon.findIntersectionsWithClippers();
            finished = true;
            break;
        case GLFW_KEY_R:
            polygon.erase();
            clippersActivated = false;
            finished = false;
            clipperPointsCount = 0;
            break;
        default:
            break;
    }
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if(!clippersActivated) {
            polygon.addVertex(Point2i((int)xpos, (int)ypos));
        }
        else {
            if(clipperPointsCount == 0) {
                clipperBegin.x = (int) xpos, clipperBegin.y = (int) ypos;
                clipperPointsCount++;
            }
            else if(clipperPointsCount == 1) {
                clipperEnd.x = (int) xpos, clipperEnd.y = (int) ypos;
                polygon.addClipper(clipperBegin, clipperEnd);
                clipperPointsCount = 0;
            }
        }
    }
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void init_view(GLFWwindow *window, int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, (float)width,  (float)height, 0, -1.f, 15.f);
}

int main(void) {
    GLFWwindow *window;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit(EXIT_FAILURE);
    }

    int width = 640, height = 480;

    window = glfwCreateWindow(width, height, "Lab 5", NULL, NULL);

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

        draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    exit(EXIT_SUCCESS);
}


void drawInternalArea() {
    for (int i = 1; i < polygon.intersectionsCount(); i+=2) {
        drawLine(polygon.getIntersectionPoint(i - 1), polygon.getIntersectionPoint(i), GREEN);
    }

}

void drawPolygon() {
    polygon.getContour().draw();
    for (int i = 0; i < polygon.holesCount(); ++i) {
        polygon.getHole(i).draw();
    }
}

void draw() {
    for (int i = 0; i < polygon.clipperCount(); ++i) {
        drawLine(polygon.getClipper(i).begin, polygon.getClipper(i).end, BLUE);
    }
    if(finished)
        drawInternalArea();
    drawPolygon();
}






































