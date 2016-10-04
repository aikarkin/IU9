#include <EllipticalCylinder.h>
#include <Texture.h>


using namespace std;

extern Texture *tex1, *tex2;

void EllipticalCylinder::pointsAlloc()
{
	cap.pixels = new Vec3f[partition + 3];
	cap.texels = new Vec2f[partition + 3];
	base.pixels = new Vec3f[partition + 3];
	base.texels = new Vec2f[partition + 3];
	
	for (int i = 0; i <= partition; i++) {
		walls.emplace_back();
		walls[i].pixels = new Vec3f[2*partition];
		walls[i].texels = new Vec2f[2*partition];
	}
}

void EllipticalCylinder::pointsFree()
{
	delete[] base.pixels;
	delete[] base.texels;
	delete[] cap.pixels;
	delete[] cap.texels;
	for (int i = 0; i < walls.size(); i++) {
		delete[] walls[i].pixels;
		delete[] walls[i].texels;
	}
	walls.clear();
}

void EllipticalCylinder::initPoints() {
	float dphi = 2 * (float)MATH_PI / (partition - 1);
	float phi;
	float dh = h / (partition - 2);
	float ds = 1.0f / partition;
	float dt = 1.0f / (partition - 2);
	float aTex = 0.5 * a / b; 
	float bTex = 0.5;

	if (a > b) {
		aTex = 0.5;
		bTex = 0.5 * b / a;
	}

	// base calc-s
	base.texels[0] = Vec2f(aTex, bTex);
	base.pixels[0] = Vec3f(x0, y0, z0);

	for (int i = 1; i < partition; i++) {
		phi = (float)MATH_PI + (i - 1)*dphi;
		base.pixels[i] = Vec3f(x0 + a * cos(phi - (float)MATH_PI), y0, z0 + b * sin(phi - (float)MATH_PI));
		base.texels[i] = Vec2f(aTex + aTex * cos(phi), bTex + bTex * sin(phi));
	}

	phi = (float)MATH_PI + (partition - 1)*dphi;
	base.pixels[partition] = Vec3f(x0 + a * cos(phi - (float)MATH_PI), y0, z0 + b * sin(phi - (float)MATH_PI));
	base.texels[partition] = Vec2f(aTex + aTex * cos(phi), bTex + bTex * sin(phi));
	base.normalVec = normal(base.pixels[1], base.pixels[0], base.pixels[2]);

	// walls calc-s
	float x1, z1, x2, z2;
	int k = 0;
	for (int i = 0; i < partition; i++) {
		k = (i + 1) % partition;

		x1 = x0 + a * cos(i * dphi);
		z1 = z0 + b * sin(i * dphi);
		x2 = x0 + a * cos(k * dphi);
		z2 = z0 + b * sin(k * dphi);

		for (int j = 0; j < 2*(partition - 1); j+=2) {
			walls[i].pixels[j] = Vec3f(x1, y0 + j / 2  * dh, z1);
			walls[i].pixels[j + 1] = Vec3f(x2, y0 + j / 2 * dh, z2);

			walls[i].texels[j] = Vec2f(i * ds, j / 2 * dt);
			walls[i].texels[j + 1] = Vec2f(k * ds, j / 2 * dt);
		}
		walls[i].normalVec = normal(walls[i].pixels[0], walls[i].pixels[1], walls[i].pixels[2]);
	}	

	// cap calc-s
	cap.texels[0] = Vec2f(aTex, bTex);
	cap.pixels[0] = Vec3f(x0, y0 + h, z0);

	for (int i = 1; i < partition; i++) {
		phi = (float)MATH_PI + (i - 1)*dphi;
		cap.pixels[i] = Vec3f(x0 + a * cos(phi - (float)MATH_PI), y0 + h, z0 + b * sin(phi - (float)MATH_PI));
		cap.texels[i] = Vec2f(aTex + aTex * cos(phi), bTex + bTex * sin(phi));
	}
	phi = (float)MATH_PI + (partition - 1)*dphi;
	cap.pixels[partition] = Vec3f(x0 + a * cos(phi - (float)MATH_PI), y0 + h, z0 + b * sin(phi - (float)MATH_PI));
	cap.texels[partition] = Vec2f(aTex + aTex * cos(phi), bTex + bTex * sin(phi));
	cap.normalVec = normal(base.pixels[1], base.pixels[2], base.pixels[0]);
}


EllipticalCylinder::EllipticalCylinder(GLfloat _x0, GLfloat _y0, GLfloat _z0,
	GLfloat _a, GLfloat _b, GLfloat _h, size_t _partition) : 
	x0(_x0), y0(_y0), z0(_z0), h(_h), partition(_partition), a(_a), b(_b) {		
		pointsAlloc();
		initPoints();
}

EllipticalCylinder::EllipticalCylinder(GLfloat _a, GLfloat _b, GLfloat _h, size_t _partition) :
	a(_a), b(_b), h(_h), partition(_partition), x0(0), y0(-_h / 2), z0(0) {
		pointsAlloc();
		initPoints();
}


EllipticalCylinder::EllipticalCylinder(size_t _partition) : partition(_partition), 
	x0(0), y0(-h / 2), 
	z0(0), a(1.f), b(1.5f) {
		pointsAlloc();
		initPoints();
}

EllipticalCylinder::~EllipticalCylinder() {
	pointsFree();
}

void EllipticalCylinder::setPartition(size_t _partition) {
	partition = _partition;
	pointsFree();
	pointsAlloc();
	initPoints();
}

size_t EllipticalCylinder::getPartition() {
	return partition;
}


void EllipticalCylinder::draw() {
	tex1->bind();
	drawEllipseO(base.pixels, base.texels, base.normalVec, partition + 1);

	tex2->bind();
	for (int i = 0; i < walls.size(); i++)
		drawWallO(walls[i].pixels, walls[i].texels, walls[i].normalVec, partition);

	tex1->bind();
	drawEllipseO(cap.pixels, cap.texels, cap.normalVec, partition + 1);
}

void EllipticalCylinder::draw(float time) {
	tex1->bind();
	drawEllipseO(base.pixels, base.texels, base.normalVec, partition + 1, time);

	tex2->bind();
	for (int i = 0; i < walls.size(); i++)
		drawWallO(walls[i].pixels, walls[i].texels, walls[i].normalVec, partition, time);


	tex1->bind();
	drawEllipseO(cap.pixels, cap.texels, cap.normalVec, partition + 1, time);
}

void EllipticalCylinder::draw(float time, bool optimize_animation) {
	if (optimize_animation)
		draw(time);
	else {
		tex1->bind();
		drawEllipse(base.pixels, base.texels, base.normalVec, partition + 1, time);

		tex2->bind();
		for (int i = 0; i < walls.size(); i++)
			drawWall(walls[i].pixels, walls[i].texels, walls[i].normalVec, partition, time);


		tex1->bind();
		drawEllipse(cap.pixels, cap.texels, cap.normalVec, partition + 1, time);
	}
}

void EllipticalCylinder::draw(bool optimize_drawing)
{
	if (optimize_drawing)
		draw();
	else {
		tex1->bind();
		drawEllipse(base.pixels, base.texels, base.normalVec, partition + 1);

		tex2->bind();
		for (int i = 0; i < walls.size(); i++)
			drawWall(walls[i].pixels, walls[i].texels, walls[i].normalVec, partition);

		tex1->bind();
		drawEllipse(cap.pixels, cap.texels, cap.normalVec, partition + 1);
	}
}

void EllipticalCylinder::createDisplayList(bool optimize_drawing)
{
	dListId = glGenLists(1);
	glNewList(dListId, GL_COMPILE);
	draw(optimize_drawing);
	glEndList();
}

void EllipticalCylinder::callDisplayList()
{
	glCallList(dListId);
}
