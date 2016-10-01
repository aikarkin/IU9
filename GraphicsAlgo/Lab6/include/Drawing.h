#pragma once
#include <MathVectors.h>

void drawCube();
void drawWall(Vec3f*, Vec2f*, Vec3f, int, float);
void drawWall2(Vec3f*, Vec2f*, Vec3f, int, float);
void drawEllipse(Vec3f*, Vec2f*, size_t, Vec3f, Vec3f, float);

Vec3f bezueFunction(Vec3f, float);