#pragma once
#include <Drawing.h>
#include <iostream>
#include <GLFW/glfw3.h>

#define MATH_PI 3.141593

class EllipticalCylinder {
private:
	Vec2f *points;
	Vec3f *normals;
	Vec2f *wallTexels;
	Vec2f *capTexels;
	GLfloat x0, y0, z0, h, a, b;
	size_t partition;

	void generatePoints();
	void generateTexels();
	void calcNormals();
public:
	EllipticalCylinder(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, size_t);
	EllipticalCylinder(GLfloat, GLfloat, GLfloat, size_t);
	EllipticalCylinder(size_t);

	void setPartition(size_t);
	Vec3f getPoint(int ind);
	Vec3f getNormal(int ind);
	size_t getPartition();
	void draw(float time);

	~EllipticalCylinder();
};
