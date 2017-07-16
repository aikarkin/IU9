#include <string>
#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

struct Texture {
	std::string file_path = "";
	bool wrap_s_repeat = true;
	bool wrap_t_repeat = true;;
	unsigned int texture_id;
	unsigned int mag_filter = 0x2601;
	unsigned int min_filter = 0x2601;
	unsigned int env_mode = 2;
	unsigned int detail_level = 0;
	void load();
	void bind();
};

void deserializeTextures(std::vector<Texture>&, rapidjson::Document &);