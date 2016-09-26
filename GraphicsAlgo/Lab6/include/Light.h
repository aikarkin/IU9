#pragma once
#include <GLFW/glfw3.h>
#include <cstdio>
#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

using namespace rapidjson;
using namespace std;

struct Light {
	unsigned int num = 0;
	unsigned int code = GL_LIGHT0;
	bool enabled = false;
	float diffuse_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float ambient_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float specular_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	float position[4] = { 0.0f, 0.5f, 1.0f, 0.f };

	float constant_attenuation = 1.0f;
	float linear_attenuation = 1.0f;
	float quadric_attenuation = 0.0f;

	float spot_cutoff; 
	float spot_exponent; 
	float spot_direction[3] = { 0.0f, 0.0f, -1.0f }; 
};

struct GlobalLight {
	unsigned int shade_mode = GL_SMOOTH;
	float global_ambient_color[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	bool local_viewer = true;
	bool two_side = false;
};

struct Material {
	unsigned int ambient_side = GL_FRONT_AND_BACK;
	float ambient_color[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

	unsigned int diffuse_side = GL_FRONT_AND_BACK;
	float diffuse_color[4] = { 0.8f, 0.8f, 0.8f, 1.0f };

	unsigned int specular_side = GL_FRONT_AND_BACK;
	float specular_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	unsigned int emission_side = GL_FRONT_AND_BACK;
	float emission_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	unsigned int shiness_side = GL_FRONT_AND_BACK;
	float shininess = 0.2f * 128.0f;;
};

unsigned int switch_code(int i);
void initGloabalLight(GlobalLight);
void initLight(Light);
void setMaterial(Material);

void deserializeLightParams(std::string file, GlobalLight&, vector<Light> &, vector<Material>&);