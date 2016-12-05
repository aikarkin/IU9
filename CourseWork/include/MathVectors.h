#pragma once
#include<cmath>

struct Vec2f {
	float x, y;
	Vec2f() : x(0), y(0) {};
	Vec2f(float _x, float _y) : x(_x), y(_y) {};
	const Vec2f operator+(const Vec2f&) const;
	const Vec2f operator-(const Vec2f&) const;
	float length();
};


struct Vec3f {
	float x, y, z;
	Vec3f() : x(0), y(0), z(0) {};
	Vec3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
	Vec3f(double _x, double _y, double _z) : x((float)_x), y((float)_y), z((float)_z) {};
	Vec3f(const Vec3f& vec) : x(vec.x), y(vec.y), z(vec.z) {};
	const Vec3f operator+(const Vec3f&) const;
	const Vec3f operator-(const Vec3f&) const;
	const Vec3f operator*(const Vec3f&) const;
	const Vec3f operator/(float d) const;
	bool operator==(const Vec3f&);

	float length();
};


Vec3f normal(Vec3f, Vec3f, Vec3f);