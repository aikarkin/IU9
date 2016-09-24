#include <MathVectors.h>
#include <iostream>

const Vec2f Vec2f::operator+(const Vec2f &v) const
{
	return Vec2f(x + v.x, y + v.y);
}

const Vec2f Vec2f::operator-(const Vec2f &v) const
{
	return Vec2f(x - v.x, y - v.y);
}

float Vec2f::length()
{
	return sqrt(x * x + y * y);
}

const Vec3f Vec3f::operator+(const Vec3f &v) const
{
	return Vec3f(x + v.x, y + v.y, z + v.z);
}

const Vec3f Vec3f::operator-(const Vec3f &v) const
{
	return Vec3f(x - v.x, y - v.y, z - v.z);
}

const Vec3f Vec3f::operator*(const Vec3f &v) const
{
	return Vec3f(y * v.z - z * v.y, v.x * z - x * v.z, x * v.y - y * v.x);
}

const Vec3f Vec3f::operator/(float d) const
{
	return Vec3f(x / d, y / d, z / d);
}

bool Vec3f::operator==(const Vec3f &other)
{
	return this->x==other.x && this->y==y && this->z==z;
}


float Vec3f::length()
{
	return sqrt(x * x + y * y + z * z);
}

Vec3f normal(Vec3f v1, Vec3f v2, Vec3f v3) {
	Vec3f c = (v3 - v1) * (v2 - v1);
	float c_len = abs((v1.x - v3.x)*(v2.y - v3.y) - (v2.x - v3.x)*(v1.y - v3.y));
	//std::cout << "c_len: " << c_len << std::endl;
	Vec3f res = c;
	return res;
}