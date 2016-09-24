#include <Light.h>
#include <fstream>
#include <string>
#include <iostream>

unsigned int switch_code(int i)
{
	unsigned int res;
	switch (i) {
	case 0:
		res = GL_LIGHT0;
		break;
	case 1:
		res = GL_LIGHT1;
		break;
	case 2:
		res = GL_LIGHT2;
		break;
	case 3:
		res = GL_LIGHT3;
		break;
	case 4:
		res = GL_LIGHT4;
		break;
	case 5:
		res = GL_LIGHT5;
		break;
	case 6:
		res = GL_LIGHT6;
		break;
	case 7:
		res = GL_LIGHT7;
		break;
	default:
		cerr << "Incorrect light index. You can't make more then 8 lights." << endl;
		return -1;
	}
	return res;
}

void initGloabalLight(GlobalLight params) {
	glShadeModel(params.shade_mode);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, params.global_ambient_color);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, params.local_viewer);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, params.two_side);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
}

void initLight(Light params) {
	params.code = switch_code(params.num);
	if(params.enabled)
		glEnable(params.code);

	glLightfv(params.code, GL_POSITION, params.position);

	glLightfv(params.code, GL_AMBIENT, params.ambient_color);
	glLightfv(params.code, GL_DIFFUSE, params.diffuse_color);
	glLightfv(params.code, GL_SPECULAR, params.specular_color);
	
	glLightf(params.code, GL_SPOT_CUTOFF, params.spot_cutoff);
	glLightf(params.code, GL_SPOT_EXPONENT, params.spot_exponent);
	glLightfv(params.code, GL_SPOT_DIRECTION, params.spot_direction);

	glLightf(params.code, GL_CONSTANT_ATTENUATION, params.constant_attenuation);
	glLightf(params.code, GL_LINEAR_ATTENUATION, params.linear_attenuation);
	glLightf(params.code, GL_QUADRATIC_ATTENUATION, params.quadric_attenuation);
}

void setMaterial(Material params) {
	glMaterialfv(params.ambient_side, GL_AMBIENT, params.ambient_color);
	glMaterialfv(params.diffuse_side, GL_DIFFUSE, params.diffuse_color);
	glMaterialfv(params.specular_side, GL_SPECULAR, params.specular_color);
	glMaterialfv(params.emission_side, GL_EMISSION, params.emission_color);
	glMaterialf(params.shiness_side, GL_SHININESS, params.shiness);
}

void deserializeLightParams(std::string file, GlobalLight &global, vector<Light> &lights, vector<Material> &materials)
{
	std::ifstream ifs(file);
	IStreamWrapper isw(ifs);
	
	Document doc;
	doc.ParseStream(isw);

	// chek json validity
	if (!doc.HasMember("lighting")) {
		cerr << "Incorrect settings file. Directive lighting is absent." << endl;
		return;
	}

	if (!doc["lighting"].HasMember("materials") && doc["lighting"]["materials"].IsArray()) {
		cerr << "Incorrect settings file. Directive materials is absent." << endl;
		return;
	}
	if (!doc["lighting"].HasMember("local") && doc["lighting"]["local"].IsArray()) {
		cerr << "Incorrect settings file. Directive local is absent." << endl;
		return;
	}

	auto &jGlobal = doc["lighting"]["global"];
	auto jMaterial = doc["lighting"]["materials"].GetArray();
	auto jLocal = doc["lighting"]["local"].GetArray();

	
	// parse global light
	if (jGlobal.HasMember("ambient_color")) {
		for (int i = 0; i < 4; i++) {
			global.global_ambient_color[i] = (float)jGlobal["ambient_color"][i].GetDouble();
		}
	}
	if (jGlobal.HasMember("local_viewer")) 
		global.local_viewer = jGlobal["local_viewer"].GetBool();
	if (jGlobal.HasMember("two_side"))
		global.two_side = jGlobal["two_side"].GetBool();

	// parse materials
	for (int j = 0; j < (int)jMaterial.Size(); j++)
	{
		Material curMaterial;
		if (jMaterial[j].HasMember("ambient_color"))
		{
			for (int i = 0; i < 4; i++) {
				curMaterial.ambient_color[i] = (float)jMaterial[j]["ambient_color"].GetArray()[i].GetDouble();
			}
		}

		if (jMaterial[j].HasMember("diffuse_color")) {
			for (int i = 0; i < 4; i++) {
				curMaterial.diffuse_color[i] = (float)jMaterial[j]["diffuse_color"].GetArray()[i].GetDouble();
			}
		}
		if (jMaterial[j].HasMember("specular_color")) {
			for (int i = 0; i < 4; i++) {
				curMaterial.specular_color[i] = (float)jMaterial[j]["specular_color"].GetArray()[i].GetDouble();
			}
		}

		if (jMaterial[j].HasMember("emission_color")) {
			for (int i = 0; i < 4; i++) {
				curMaterial.emission_color[i] = (float)jMaterial[j]["emission_color"].GetArray()[i].GetDouble();
			}
		}
		materials.push_back(curMaterial);
	}

	// parse lights
	if (jLocal.Size() > 7) {
		cerr << "Error! To much lights. Maximum count of lights is 8" << endl;
		return;
	}
	for (int j = 0; j < (int)jLocal.Size(); ++j) {
		Light curLight;
		if (jLocal[j].HasMember("num")) {
			curLight.num = jLocal[j]["num"].GetInt();
		}
		if (jLocal[j].HasMember("enabled")) {
			curLight.enabled = jLocal[j]["enabled"].GetBool();
		}
		if (jLocal[j].HasMember("diffuse_color")) {
			for (int i = 0; i < 4; i++) {
				curLight.diffuse_color[i] = (float)jLocal[j]["diffuse_color"][i].GetDouble();
			}
		}

		if (jLocal[j].HasMember("ambient_color")) {
			for (int i = 0; i < 4; i++) {
				curLight.ambient_color[i] = (float)jLocal[j]["ambient_color"][i].GetDouble();
			}
		}

		if (jLocal[j].HasMember("specular_color")) {
			for (int i = 0; i < 4; i++) {
				curLight.specular_color[i] = (float)jLocal[j]["specular_color"][i].GetDouble();
			}
		}
		
		if (jLocal[j].HasMember("position")) {
			for (int i = 0; i < 4; i++) {
				curLight.position[i] = (float)jLocal[j]["position"][i].GetDouble();
			}
		}
		
		if(jLocal[j].HasMember("spot_cutoff"))
			curLight.spot_cutoff = (float)jLocal[j]["spot_cutoff"].GetDouble();
		if (jLocal[j].HasMember("spot_exponent"))
			curLight.spot_exponent = (float)jLocal[j]["spot_exponent"].GetDouble();
		if (jLocal[j].HasMember("spot_direction")) {
			for (int i = 0; i < 3; i++) {
				curLight.spot_direction[i] = (float)jLocal[j]["spot_direction"][i].GetDouble();
			}
		}

		lights.push_back(curLight);
	}
}