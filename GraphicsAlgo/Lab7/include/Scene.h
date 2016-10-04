#pragma once
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <Texture.h>
#include <Light.h>

struct OptimizationParams {
	bool optimize_drawing = false;
	bool display_lists = false;
	bool optimize_animation = false;
	bool optimize_lighting = false;
};

struct Scene {
	std::string settings_file;
	bool lighting_enabled;
	bool carcas_enabled;
	bool animation_enabled;
	int partition;
	float current_time;
	float rotate_x, rotate_y;
	float scale;
	bool texture_enabled;
	std::vector<Material> materials;
	std::vector<Light> local_lights;
	GlobalLight global_light;
	std::vector<Texture> textures;
	OptimizationParams optimization;
	void load();
	void save();
};