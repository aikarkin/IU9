#include <Light.h>
#include <Drawing.h>
#include <Scene.h>
#include <iostream>

extern Scene scene;
GLuint cubeID;

float cubePoints[][6] = {
				/*  normal coordiantes		vertexes coordinates */
					// front face
					{ 0.0, 0.0, 1.0,		1.0f, 1.0f, 1.0f},
					{ 0.0, 0.0, 1.0,		-1.0f, 1.0f, 1.0f},
					{ 0.0, 0.0, 1.0,		-1.0, -1.0, 1.0},
					{ 0.0, 0.0, 1.0,		1.0, -1.0, 1.0 },
					// left face
					{ -1.0, 0.0, 0.0,		-1.0, 1.0, 1.0 },
					{ -1.0, 0.0, 0.0,		-1.0, 1.0, -1.0},
					{ -1.0, 0.0, 0.0,		-1.0, -1.0, -1.0},
					{ -1.0, 0.0, 0.0,		-1.0, -1.0, 1.0},
					// back face
					{ 0.0, 0.0, -1.0,		1.0, 1.0, -1.0},
					{ 0.0, 0.0, -1.0 ,		1.0, -1.0, -1.0},
					{ 0.0, 0.0, -1.0,		-1.0, -1.0, -1.0},
					{ 0.0, 0.0, -1.0,		-1.0, 1.0, -1.0},
					// top face
					{ 0.0, 1.0, 0.0,		-1.0, 1.0, -1.0 },
					{ 0.0, 1.0, 0.0,		-1.0, 1.0, 1.0 },
					{ 0.0, 1.0, 0.0,		 1.0, 1.0, 1.0 },
					{ 0.0, 1.0, 0.0,		1.0, 1.0, -1.0 },
					// right face
					{ 1.0, 0.0, 0.0,		1.0, 1.0, 1.0},
					{ 1.0, 0.0, 0.0,		1.0, -1.0, 1.0},
					{ 1.0, 0.0, 0.0,		1.0, -1.0, -1.0},
					{ 1.0, 0.0, 0.0,		1.0, 1.0, -1.0},
					// buttom face
					{ 0.0, -1.0, 0.0,		-1.0, -1.0, -1.0},
					{ 0.0, -1.0, 0.0,		1.0, -1.0, -1.0},
					{ 0.0, -1.0, 0.0,		1.0, -1.0, 1.0},
					{ 0.0, -1.0, 0.0,		-1.0, -1.0, 1.0}
};

void drawCube() {
	// front face
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);

		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);
		glVertex3f(1.0, -1.0, 1.0);
	glEnd();

	// back face
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, -1.0);

		glVertex3f(1.0, 1.0, -1.0);
		glVertex3f(1.0, -1.0, -1.0);
		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(-1.0, 1.0, -1.0);
	glEnd();

	// left face
	glBegin(GL_QUADS);
		glNormal3f(-1.0, 0.0, 0.0);

		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(-1.0, 1.0, -1.0);
		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(-1.0, -1.0, 1.0);
	glEnd();

	// right face
	glBegin(GL_QUADS);
		glNormal3f(1.0, 0.0, 0.0);

		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(1.0, -1.0, 1.0);
		glVertex3f(1.0, -1.0, -1.0);
		glVertex3f(1.0, 1.0, -1.0);
	glEnd();

	// top face
	glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0);
		
		glVertex3f(-1.0, 1.0, -1.0);
		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(1.0, 1.0, -1.0);
	glEnd();

	// buttom face
	glBegin(GL_QUADS);
		glNormal3f(0.0, -1.0, 0.0);

		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(1.0, -1.0, -1.0);
		glVertex3f(1.0, -1.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);
	glEnd();
}

void drawCubeO() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glInterleavedArrays(GL_N3F_V3F, 0, cubePoints);
	glDrawArrays(GL_QUADS, 0, 24);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}


void drawWall(Vec3f *pixels, Vec2f *texels, Vec3f normal, size_t partition) {
	glBegin(GL_QUAD_STRIP);
		glNormal3f(normal.x, normal.y, normal.z);
		for (int i = 0; i < 2 * partition - 2; i += 2) {
			glTexCoord2f(texels[i].x, texels[i].y);
			glVertex3f(pixels[i].x, pixels[i].y, pixels[i].z);
			glTexCoord2f(texels[i + 1].x, texels[i + 1].y);
			glVertex3f(pixels[i + 1].x, pixels[i + 1].y, pixels[i + 1].z);
		}
	glEnd();
}

void drawWallO(Vec3f *pixels, Vec2f *texels, Vec3f normal, size_t partition) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pixels);
	if (!scene.optimization.optimize_lighting || scene.optimization.optimize_lighting && scene.lighting_enabled) {
		glNormal3fv(&normal.x);
	}
	if (scene.texture_enabled) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, texels);
	}
	glDrawArrays(GL_QUAD_STRIP, 0, 2 * partition - 2);
	if (scene.texture_enabled)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}


void drawWall(Vec3f *pixels, Vec2f *texels, Vec3f normal, size_t partition, float time)
{
	Vec3f p1, p2, p3, p4;

	p1 = bezueFunction(pixels[0], time);
	p2 = bezueFunction(pixels[1], time);
	p3 = bezueFunction(pixels[2 * partition - 4], time);
	p4 = bezueFunction(pixels[2 * partition - 3], time);

	glBegin(GL_QUAD_STRIP);
		glNormal3f(normal.x, normal.y, normal.z);

		glTexCoord2f(texels[0].x, texels[0].y);
		glVertex3f(p1.x, p1.y, p1.z);
		glTexCoord2f(texels[1].x, texels[1].y);
		glVertex3f(p2.x, p2.y, p2.z);
		glTexCoord2f(texels[2 * partition - 4].x, texels[2 * partition - 4].y);
		glVertex3f(p3.x, p3.y, p3.z);
		glTexCoord2f(texels[2 * partition - 3].x, texels[2 * partition - 3].y);
		glVertex3f(p4.x, p4.y, p4.z);
	glEnd();
}

void drawWallO(Vec3f *pixels, Vec2f *texels, Vec3f normal, size_t partition, float time)
{
	Vec3f p1, p2, p3, p4;

	p1 = bezueFunction(pixels[0], time);
	p2 = bezueFunction(pixels[1], time);
	p3 = bezueFunction(pixels[2 * partition - 4], time);
	p4 = bezueFunction(pixels[2 * partition - 3], time);

	if (scene.texture_enabled) {
		glBegin(GL_QUAD_STRIP);
			if(!scene.optimization.optimize_lighting || (scene.optimization.optimize_lighting && scene.lighting_enabled))
				glNormal3fv(&normal.x);
			glTexCoord2fv(&texels[0].x);
			glVertex3fv(&p1.x);
			glTexCoord2fv(&texels[1].x);
			glVertex3fv(&p2.x);
			glTexCoord2fv(&texels[2 * partition - 4].x);
			glVertex3fv(&p3.x);
			glTexCoord2fv(&texels[2 * partition - 3].x);
			glVertex3f(p4.x, p4.y, p4.z);
		glEnd();
	}
	else {
		glBegin(GL_QUAD_STRIP);
			if ((scene.optimization.optimize_lighting && scene.lighting_enabled) || !scene.optimization.optimize_lighting)
				glNormal3fv(&normal.x);
			glVertex3fv(&p1.x);
			glVertex3fv(&p2.x);
			glVertex3fv(&p3.x);
			glVertex3f(p4.x, p4.y, p4.z);
		glEnd();
	}
}

void drawEllipse(Vec3f *points, Vec2f *texels, Vec3f normal, size_t partition) {
	Vec3f O_(points[0]);
	Vec3f curPoint;

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(normal.x, normal.y, normal.z);
	glTexCoord2f(texels[0].x, texels[0].y);
	glVertex3f(O_.x, O_.y, O_.z);
	for (int i = 1; i < partition; i++) {
		curPoint = points[i];
		glTexCoord2f(texels[i].x, texels[i].y);
		glVertex3f(curPoint.x, curPoint.y, curPoint.z);
	}

	curPoint = points[0];
	glTexCoord2f(texels[partition].x, texels[partition].y);
	glVertex3f(curPoint.x, curPoint.y, curPoint.z);
	glEnd();
}

void drawEllipseO(Vec3f *pixels, Vec2f *texels, Vec3f normal, size_t partition) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormal3fv(&normal.x);
	
	if (!scene.optimization.optimize_lighting || (scene.optimization.optimize_lighting && scene.lighting_enabled))
		glNormal3fv(&normal.x);
	if (scene.texture_enabled) {
		glTexCoordPointer(2, GL_FLOAT, 0, texels);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	glVertexPointer(3, GL_FLOAT, 0, pixels);
	glDrawArrays(GL_TRIANGLE_FAN, 0, partition);
	if (scene.texture_enabled)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawEllipse(Vec3f *points, Vec2f *texels, Vec3f normal, size_t partition, float time) {
	Vec3f O_(bezueFunction(points[0], time));
	Vec3f curPoint;
	glBegin(GL_TRIANGLE_FAN);
		glNormal3f(normal.x, normal.y, normal.z);

		glTexCoord2f(texels[0].x, texels[0].y);
		glVertex3f(O_.x, O_.y, O_.z);
		for (int i = 1; i < partition; i++) {
			curPoint = bezueFunction(points[i], time);
			glTexCoord2f(texels[i].x, texels[i].y);	
			glVertex3f(curPoint.x, curPoint.y, curPoint.z);
		}

		curPoint = bezueFunction(points[0], time);
		glTexCoord2f(texels[partition].x, texels[partition].y);
		glVertex3f(curPoint.x, curPoint.y, curPoint.z);
	glEnd();
}

void drawEllipseO(Vec3f *points, Vec2f *texels, Vec3f normal, size_t partition, float time) {
	Vec3f O_(bezueFunction(points[0], time));
	Vec3f curPoint;
	if (scene.texture_enabled) {
		glBegin(GL_TRIANGLE_FAN);
		if (!scene.optimization.optimize_lighting || (scene.optimization.optimize_lighting && scene.lighting_enabled))
			glNormal3fv(&normal.x);
		glTexCoord2fv(&texels[0].x);
		glVertex3fv(&O_.x);
		for (int i = 1; i < partition; i++) {
			curPoint = bezueFunction(points[i], time);
			glTexCoord2fv(&texels[i].x);
			glVertex3fv(&curPoint.x);
		}

		curPoint = bezueFunction(points[0], time);
		glTexCoord2fv(&texels[partition].x);
		glVertex3fv(&curPoint.x);
		glEnd();
	}
	else {
		glBegin(GL_TRIANGLE_FAN);
		if (!scene.optimization.optimize_lighting || scene.optimization.optimize_lighting && scene.lighting_enabled)
			glNormal3fv(&normal.x);
		glVertex3fv(&O_.x);
		for (int i = 1; i < partition; i++) {
			curPoint = bezueFunction(points[i], time);
			glVertex3fv(&curPoint.x);
		}
		curPoint = bezueFunction(points[0], time);
		glVertex3fv(&curPoint.x);
		glEnd();
	}
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

void createCubeDList()
{
	cubeID = glGenLists(1);
	if (scene.optimization.optimize_drawing) {
		glNewList(cubeID, GL_COMPILE);
		drawCubeO();
		glEndList();
	}
	else {
		glNewList(cubeID, GL_COMPILE);
		drawCube();
		glEndList();
	}
}

void callCubeDList() {
	glCallList(cubeID);
}