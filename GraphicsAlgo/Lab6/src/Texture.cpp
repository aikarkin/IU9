#include <Texture.h>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <GL\glew.h>
#include <SOIL.h>

void Texture::load()
{
	int width, height;
	if (file_path == "") {
		std::cerr << "Texture file is not specified" << std::endl;
		return;
	}
	unsigned char *img = SOIL_load_image(file_path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s_repeat ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t_repeat ? GL_REPEAT : GL_CLAMP);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, env_mode);

	SOIL_free_image_data(img);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
}

void deserializeTextures(std::vector<Texture> &textures, std::string& settings_path)
{
	std::ifstream ifs(settings_path);
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document doc;
	doc.ParseStream(isw);

	if (doc.HasParseError()) {
		std::cerr << "Incorrect json." << std::endl;
		return;
	}
	if (!doc.HasMember("textures")) {
		std::cerr << "Incorrect settings file. Directive lighting is absent." << std::endl;
		return;
	}


	rapidjson::Value& jTextures = doc["textures"];
	for (rapidjson::Value& jTex : jTextures.GetArray()) {
		Texture tex;
		assert(jTex.HasMember("file_path") && jTex["file_path"].IsString());
		tex.file_path = jTex["file_path"].GetString();

		if (jTex.HasMember("wrap_s_repeat"))
			tex.wrap_s_repeat = jTex["wrap_s_repeat"].GetBool();
		if (jTex.HasMember("wrap_t_repeat"))
			tex.wrap_t_repeat = jTex["wrap_t_repeat"].GetBool();

		if (jTex.HasMember("min_filter")) {
			if (jTex["min_filter"].IsInt()) {
				unsigned int glMinFiltervi[] = {
					GL_NEAREST,
					GL_LINEAR,
					GL_NEAREST_MIPMAP_NEAREST,
					GL_LINEAR_MIPMAP_NEAREST,
					GL_NEAREST_MIPMAP_LINEAR,
					GL_LINEAR_MIPMAP_LINEAR
				};
				tex.min_filter = glMinFiltervi[jTex["min_filter"].GetInt()];
			}
			else if (jTex["min_filter"].IsString()) {
				std::unordered_map<std::string, unsigned int> glMinFilters({
					{ "nearest", GL_NEAREST },
					{ "linear", GL_LINEAR },
					{ "nearest_mipmap_nearest", GL_NEAREST_MIPMAP_NEAREST },
					{ "linear_mipmap_nearest", GL_LINEAR_MIPMAP_NEAREST },
					{ "nearest_mipmap_linear", GL_NEAREST_MIPMAP_LINEAR },
					{ "linear_mipmap_linear", GL_LINEAR_MIPMAP_LINEAR }
				});
				tex.min_filter = glMinFilters[jTex["min_filter"].GetString()];
			}
		}

		if (jTex.HasMember("mag_filter")) {
			if (jTex["mag_filter"].IsInt())
				tex.mag_filter = (jTex["mag_filter"].GetInt() == 1 ? GL_NEAREST : GL_LINEAR);
			else if (jTex["mag_filter"].IsString())
				tex.mag_filter = (jTex["mag_filter"].GetString() == "nearest" ? GL_NEAREST : GL_LINEAR);
		}

		if (jTex.HasMember("env_mode")) {
			if (jTex["env_mode"].IsInt()) {
				unsigned int glEnvModevi[]{
					GL_REPLACE,
					GL_MODULATE,
					GL_ADD,
					GL_ADD_SIGNED,
					GL_INTERPOLATE,
					GL_SUBTRACT,
					GL_DOT3_RGB,
					GL_DOT3_RGBA
				};
				tex.env_mode = glEnvModevi[jTex["env_mode"].GetInt()];
			}
			else if (jTex["env_mode"].IsString()) {
				std::unordered_map<std::string, unsigned int> glEnvModes({
					{ "replace", GL_REPLACE },
					{ "modulate", GL_MODULATE },
					{ "add", GL_ADD },
					{ "add_signed", GL_ADD_SIGNED },
					{ "interpolate", GL_INTERPOLATE },
					{ "substract", GL_SUBTRACT },
					{ "dot3_rgb", GL_DOT3_RGB },
					{ "dot3_rgba", GL_DOT3_RGBA }
				});
				tex.env_mode = glEnvModes[jTex["env_mode"].GetString()];
			}
		}

		if (jTex.HasMember("detail_level"))
			tex.detail_level = jTex["detail_level"].GetInt();

		textures.push_back(tex);
	}
}

