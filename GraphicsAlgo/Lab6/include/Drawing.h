#pragma once
#include <MathVectors.h>

void drawCube();
void drawRectangle(Vec3f, Vec3f, Vec2f*, Vec3f, float, float);
void drawEllipse(Vec2f*, Vec2f*, size_t, Vec3f, Vec3f, float);

Vec3f bezueFunction(Vec3f, float);