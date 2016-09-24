#include <EllipticalCylinder.h>

using namespace std;

void EllipticalCylinder::generatePoints() {
	GLfloat dphi = 2 * (float)MATH_PI / (partition - 1);

	for (int i = 0; i<partition; i++) {
		points[i].x = x0 + a*cos(i*dphi);
		points[i].y = z0 + b*sin(i*dphi);
	}
}

void EllipticalCylinder::calcNormals() {
	normals[0] = normal(Vec3f(points[0].x, y0, points[0].y), Vec3f(points[1].x, y0, points[1].y), Vec3f(points[2].x, y0, points[2].y));
	/*normals[0].x *= -1;
	normals[0].y *= -1;
	normals[0].z *= -1;
*/
	for (size_t i = 1; i < partition; i++)	{
		Vec3f v1(points[i - 1].x, y0, points[i - 1].y);
		Vec3f v2(points[i - 1].x, y0 + h, points[i - 1].y);
		Vec3f v3(points[i].x, y0 + h, points[i].y);
		normals[i] = normal(v2, v1, v3);
	}

	normals[partition] = normal(Vec3f(points[partition - 1].x, y0 + h, points[partition - 1].y),
		Vec3f(points[partition - 1].x, y0, points[partition - 1].y),
		Vec3f(points[0].x, y0 + h, points[0].y));

	normals[partition + 1] = normal(Vec3f(points[0].x, y0 + h, points[0].y),
		Vec3f(points[1].x, y0 + h, points[1].y),
		Vec3f(points[2].x, y0 + h, points[2].y));

}

EllipticalCylinder::EllipticalCylinder(GLfloat _x0, GLfloat _y0, GLfloat _z0,
	GLfloat _a, GLfloat _b, GLfloat _h, size_t _partition) : 
	x0(_x0), y0(_y0), z0(_z0), h(_h), partition(_partition), a(_a), b(_b) {
		points = new Vec2f[_partition];
		normals = new Vec3f[_partition + 3];
		generatePoints();
		calcNormals();
}

EllipticalCylinder::EllipticalCylinder(GLfloat _a, GLfloat _b, GLfloat _h, size_t _partition) :
	a(_a), b(_b), h(_h), partition(_partition), x0(0), y0(-_h / 2), z0(0) {
		points = new Vec2f[_partition];
		normals = new Vec3f[_partition + 3];
		generatePoints();
		calcNormals();
}

EllipticalCylinder::EllipticalCylinder(size_t _partition) : partition(_partition), 
	x0(0), y0(-h / 2), 
	z0(0), a(1.f), b(1.5f) {
		points = new Vec2f[_partition];
		normals = new Vec3f[_partition + 3];
		generatePoints();
		calcNormals();
}

EllipticalCylinder::~EllipticalCylinder() {
	delete[]points;
	delete[]normals;
}

void EllipticalCylinder::setPartition(size_t _partition) {
	partition = _partition;
	delete[]points;
	delete[]normals;
	points = new Vec2f[partition];
	normals = new Vec3f[partition + 3];
	generatePoints();
	calcNormals();
}

size_t EllipticalCylinder::getPartition() {
	return partition;
}

void EllipticalCylinder::draw(float time) {
	//Drowing buttom side
	Ellipse(points, partition, Vec3f(x0, y0, z0), normals[0], time);

	//Drawing cylinder
	for (int i = 1; i < partition; ++i) {
		Vec3f v1(points[i - 1].x, y0, points[i - 1].y);
		Vec3f v2(points[i].x, y0, points[i].y);

		drawRectangle(v1, v2, normals[i], h, time);
	}
	//Drawing last face of cylinder
	drawRectangle(Vec3f(points[partition - 1].x, y0, points[partition - 1].y),
		Vec3f(points[0].x, y0, points[0].y), normals[partition], h, time);

	//Drawing top side
	Ellipse(points, partition, Vec3f(x0, y0 + h, z0), normals[partition + 1], time);
}

Vec3f EllipticalCylinder::getPoint(int ind) {
	if (ind > 2 * partition || ind < 0) {
		std::cerr << "Index of point out of range!" << std::endl;
		return Vec3f(0,0,0);
	}
	if (ind < partition)
		return Vec3f(points[ind].x, y0, points[ind].y);
	else if (ind == 2*partition) //points[2*partition] - buttom center
		return Vec3f(x0, y0, z0);
	else if(ind == 2*partition + 1) // points[2*parition + 1] - top center
		return Vec3f(x0, y0 + h, z0);
	else 
		return Vec3f(points[ind].x, y0 + h, points[ind].y);

}

Vec3f EllipticalCylinder::getNormal(int ind)
{
	return normals[ind];
}

