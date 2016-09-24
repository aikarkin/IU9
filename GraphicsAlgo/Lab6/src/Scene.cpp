#include <Scene.h>

void Scene::load()
{
	std::ifstream ifs(settings_file);
	rapidjson::IStreamWrapper isw(ifs);

	Document doc;
	doc.ParseStream(isw);

	if (!doc.HasMember("scene")) {
		std::cerr << "Incorrect settings file. Directive scene is absent" << endl;
	}

	Value& scene = doc["scene"];
	carcas_enabled = scene["carcas_enabled"].GetBool();
	animation_enabled = scene["animation_enabled"].GetBool();
	partition = scene["partition"].GetInt();
	current_time = scene["current_time"].GetFloat();
	rotate_x = scene["rotate_x"].GetFloat();
	rotate_y = scene["rotate_y"].GetFloat();
	scale = scene["scale"].GetFloat();
	texture_enabled = scene["texture_enabled"].GetBool();
	deserializeLightParams(settings_file, global_light, local_lights, materials);
}

void Scene::save()
{
	std::ifstream ifs(settings_file);
	rapidjson::IStreamWrapper isw(ifs);

	Document doc;
	doc.ParseStream(isw);
	ifs.close();

	ofstream ofs(settings_file);
	OStreamWrapper osw(ofs);
	Writer<OStreamWrapper> writer(osw);

	Value& scene = doc["scene"];

	scene["carcas_enabled"].SetBool(carcas_enabled);
	scene["animation_enabled"].SetBool(animation_enabled);
	scene["texture_enabled"].SetBool(texture_enabled);
	scene["partition"].SetInt(partition);
	scene["current_time"].SetFloat(current_time);
	scene["rotate_x"].SetFloat(rotate_x);
	scene["rotate_y"].SetFloat(rotate_y);
	scene["scale"].SetFloat(scale);

	auto lights = doc["lighting"]["local"].GetArray();
	for(auto light : local_lights)
		lights[light.num]["enabled"].SetBool(light.enabled);

	doc.Accept(writer);
	ofs.close();
}
