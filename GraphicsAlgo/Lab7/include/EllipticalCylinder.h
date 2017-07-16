#pragma once
#include <Drawing.h>
#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>

#define MATH_PI 3.141593

class EllipticalCylinder {
private:
	std::vector<Face> walls;
	Face base, cap;
	GLuint dListId;

	GLfloat x0, y0, z0, h, a, b;
	size_t partition;
	void pointsAlloc();
	void pointsFree();
	void initPoints();
public:
	EllipticalCylinder(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, size_t);
	EllipticalCylinder(GLfloat, GLfloat, GLfloat, size_t);
	EllipticalCylinder(size_t);

	void setPartition(size_t);
	size_t getPartition();

	void draw(float time);
	void draw();

	void draw(float time, bool optimize_animation);
	void draw(bool optimize_drawing);

	void createDisplayList(bool optimize_drawing);
	void callDisplayList();

	~EllipticalCylinder();
};
