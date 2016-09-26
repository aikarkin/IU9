#include <Drawing.h>
#include <iostream>

extern Scene scene;

void drawCube() {
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		//glColor3f(1.0f, 0.5f, 0.0f);
		setMaterial(scene.materials[0]);
		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);
		glVertex3f(1.0, -1.0, 1.0);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, -1.0);
		//glColor3f(0.0f, 1.0f, 0.0f);
		setMaterial(scene.materials[1]);

		glVertex3f(1.0, 1.0, -1.0);
		glVertex3f(1.0, -1.0, -1.0);
		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(-1.0, 1.0, -1.0);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(-1.0, 0.0, 0.0);
		setMaterial(scene.materials[2]);

		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(-1.0, 1.0, -1.0);
		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(-1.0, -1.0, 1.0);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(1.0, 0.0, 0.0);
		//glColor3f(1.0, 0.0, 0.0);
		setMaterial(scene.materials[3]);
		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(1.0, -1.0, 1.0);
		glVertex3f(1.0, -1.0, -1.0);
		glVertex3f(1.0, 1.0, -1.0);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0);
		//glColor3f(0.0, 0.0, 1.0);
		setMaterial(scene.materials[4]);

		glVertex3f(-1.0, 1.0, -1.0);
		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(1.0, 1.0, -1.0);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(0.0, -1.0, 0.0);
		//glColor3f(1.0, 0.0, 1.0);
		setMaterial(scene.materials[5]);

		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(1.0, -1.0, -1.0);
		glVertex3f(1.0, -1.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);
	glEnd();
}


void drawRectangle(Vec3f p1, Vec3f p2, Vec2f *texels, Vec3f N, GLfloat h, float time) {
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
		setMaterial(scene.materials[5]);

		glTexCoord2f(texels[0].x, texels[0].y);
		glVertex3fv(&v1.x);

		glTexCoord2f(texels[1].x, texels[1].y);
		glVertex3fv(&v2.x);

		glTexCoord2f(texels[3].x, texels[3].y);
		glVertex3fv(&v3.x);

		glTexCoord2f(texels[2].x, texels[2].y);
		glVertex3fv(&v4.x);
	glEnd();
}

void drawEllipse(Vec2f *points, Vec2f *texels, size_t count, Vec3f O, Vec3f N, float time) {
	Vec3f O_(O);
	if(time != 0) {
		O_ = bezueFunction(O, time);
		//N = bezueFunction(N, time);
	}
	glBegin(GL_TRIANGLE_FAN);
		glNormal3f(N.x, N.y, N.z);
		setMaterial(scene.materials[1]);
		glTexCoord2f(texels[0].x, texels[0].y);
		glVertex3f(O_.x, O_.y, O_.z);
		for (int i = 0; i < count; i++) {
			Vec3f curPoint(points[i].x, O.y, points[i].y);
			if(time!=0)
				curPoint = bezueFunction(curPoint, time);
			//cout << "(" << texels[i].x << "; " << texels[i].y << ")" << endl;
			glTexCoord2f(texels[i + 1].x, texels[i + 1].y);
			glVertex3fv(&curPoint.x);
		}

		Vec3f curPoint(points[0].x, O.y, points[0].y);
		if (time != 0)
			curPoint = bezueFunction(curPoint, time);
		//cout << "(" << texels[i].x << "; " << texels[i].y << ")" << endl;
		glTexCoord2f(texels[count].x, texels[count].y);
		glVertex3fv(&curPoint.x);
	glEnd();
}

Vec3f bezueFunction(Vec3f p0, float t)
{
	if (p0.y == -1) {
		return p0;
	}

	Vec3f p, p1(p0.x, 3.0, p0.z), p2(p0.x, -1, p0.z);
	p.x = (1 - t)*(1 - t)*p0.x + 2 * t*(1 - t)*p1.x + t*t*p2.x;
	p.y = (1 - t)*(1 - t)*p0.y + 2 * t*(1 - t)*p1.y + t*t*p2.y;
	p.z = (1 - t)*(1 - t)*p0.z + 2 * t*(1 - t)*p1.z + t*t*p2.z;
	return p;
}
