#include <Light.h>
#include <Drawing.h>
#include <Scene.h>
#include <iostream>

extern Scene scene;

void drawCube() {
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		glColor3f(1.0f, 0.5f, 0.0f);

		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);
		glVertex3f(1.0, -1.0, 1.0);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, -1.0);
		glColor3f(0.0f, 1.0f, 0.0f);

		glVertex3f(1.0, 1.0, -1.0);
		glVertex3f(1.0, -1.0, -1.0);
		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(-1.0, 1.0, -1.0);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(-1.0, 0.0, 0.0);
		glColor3f(0.0f, 1.0f, 03.f);

		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(-1.0, 1.0, -1.0);
		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(-1.0, -1.0, 1.0);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(1.0, 0.0, 0.0);
		glColor3f(1.0, 0.0, 0.0);

		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(1.0, -1.0, 1.0);
		glVertex3f(1.0, -1.0, -1.0);
		glVertex3f(1.0, 1.0, -1.0);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0);
		glColor3f(0.0, 0.0, 1.0);

		glVertex3f(-1.0, 1.0, -1.0);
		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(1.0, 1.0, -1.0);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(0.0, -1.0, 0.0);
		glColor3f(1.0, 0.0, 1.0);

		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(1.0, -1.0, -1.0);
		glVertex3f(1.0, -1.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);
	glEnd();
}


void drawWall(Vec3f *pixels, Vec2f *texels, Vec3f normal, int partition, float time) {
	glBegin(GL_QUAD_STRIP);
	glNormal3fv(&normal.x);
	Vec3f p1, p2;
	for (int i = 0; i < 2*(partition - 1); i+=2)
	{
		p1 = bezueFunction(pixels[i], time);
		p2 = bezueFunction(pixels[i + 1], time);

		glTexCoord2fv(&texels[i].x);
		glVertex3fv(&p1.x);

		glTexCoord2fv(&texels[i + 1].x);
		glVertex3fv(&p2.x);
	}
	glEnd();
}

void drawWall2(Vec3f *pixels, Vec2f *texels, Vec3f normal, int partition, float time)
{
	Vec3f p1, p2, p3, p4;
	glBegin(GL_QUAD_STRIP);
		glNormal3fv(&normal.x);
		p1 = bezueFunction(pixels[0], time);
		p2 = bezueFunction(pixels[1], time);
		p3 = bezueFunction(pixels[2 * partition - 4], time);
		p4 = bezueFunction(pixels[2 * partition - 3], time);

		glTexCoord2fv(&texels[0].x);
		glVertex3fv(&p1.x);
		glTexCoord2fv(&texels[1].x);
		glVertex3fv(&p2.x);
		glTexCoord2fv(&texels[2 * partition - 4].x);
		glVertex3fv(&p3.x);
		glTexCoord2fv(&texels[2 * partition - 3].x);
		glVertex3fv(&p4.x);
	glEnd();
}


/*void drawRectangle2(Vec3f p1, Vec3f p2, Vec2f *texels, Vec3f N, float h, float time, int k) {
	Vec3f v1(p1), v4(p2), v2(p1.x, p1.y + h, p1.z), v3(p2.x, p2.y + h, p2.z);
	if(time!=0) {
		//N = bezueFunction(N, time);
		v1 = bezueFunction(p1, time);
		v2 = bezueFunction(Vec3f(p1.x, p1.y + h, p1.z), time);
		v3 = bezueFunction(Vec3f(p2.x, p2.y + h, p2.z), time);
		v4 = bezueFunction(Vec3f(p2.x, p2.y, p2.z), time);
	}
	glBegin(GL_QUADS);
		glNormal3f(N.x, N.y, N.z);
		glColor3f(1.0, 0.3, 0.0);

		glTexCoord2f(texels[0].x, texels[0].y);
		glVertex3fv(&v1.x);

		glTexCoord2f(texels[1].x, texels[1].y);
		glVertex3fv(&v2.x);

		glTexCoord2f(texels[3].x, texels[3].y);
		glVertex3fv(&v3.x);

		glTexCoord2f(texels[2].x, texels[2].y);
		glVertex3fv(&v4.x);
	glEnd();
}*/

void drawEllipse(Vec3f *points, Vec2f *texels, size_t count, Vec3f centr, Vec3f normal, float time) {
	Vec3f O_(bezueFunction(centr, time));
	Vec3f curPoint;
	glBegin(GL_TRIANGLE_FAN);
		glNormal3f(normal.x, normal.y, normal.z);
		glTexCoord2f(texels[0].x, texels[0].y);
		glVertex3f(O_.x, O_.y, O_.z);
		for (int i = 0; i < count; i++) {
			curPoint = bezueFunction(points[i], time);
			glTexCoord2fv(&texels[i + 1].x);	
			glVertex3fv(&curPoint.x);
		}

		curPoint = bezueFunction(points[0], time);
		glTexCoord2fv(&texels[count].x);
		glVertex3fv(&curPoint.x);
	glEnd();
}

Vec3f bezueFunction(Vec3f p0, float t)
{
	if (t == 0 || p0.y == -1) {
		return p0;
	}

	Vec3f p, p1(p0.x, 3.0, p0.z), p2(p0.x, -1, p0.z);
	p.x = (1 - t)*(1 - t)*p0.x + 2 * t*(1 - t)*p1.x + t*t*p2.x;
	p.y = (1 - t)*(1 - t)*p0.y + 2 * t*(1 - t)*p1.y + t*t*p2.y;
	p.z = (1 - t)*(1 - t)*p0.z + 2 * t*(1 - t)*p1.z + t*t*p2.z;
	return p;
}
