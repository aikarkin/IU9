#pragma once
#include <MathVectors.h>

struct Face {
	Vec3f *pixels;
	Vec2f *texels;
	Vec3f normalVec;
};

void drawCube();
void drawCubeO();

void drawWall(Vec3f*, Vec2f*, Vec3f, size_t);
void drawWallO(Vec3f*, Vec2f*, Vec3f, size_t);

void drawWall(Vec3f*, Vec2f*, Vec3f, size_t, float);
void drawWallO(Vec3f*, Vec2f*, Vec3f, size_t, float);

void drawEllipse(Vec3f*, Vec2f*, Vec3f, size_t);
void drawEllipseO(Vec3f*, Vec2f*, Vec3f, size_t);

void drawEllipse(Vec3f*, Vec2f*, Vec3f, size_t, float);
void drawEllipseO(Vec3f*, Vec2f*, Vec3f, size_t, float);

Vec3f bezueFunction(Vec3f, float);

void createCubeDList();
void callCubeDList();
