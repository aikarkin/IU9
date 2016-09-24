#pragma once
#include <Light.h>
#include <Scene.h>
#include <MathVectors.h>

void drawCube();
void drawRectangle(Vec3f, Vec3f, Vec3f, GLfloat, float);
void Ellipse(Vec2f *, size_t, Vec3f, Vec3f, float);

Vec3f bezueFunction(Vec3f, float);