//
// Created by alex on 26.02.17.
//

#include <fstream>
#include <conf_parser.h>

std::string floatToString(float num, int precision) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << num;
    std::string s = stream.str();
    return s;
}

std::tuple<std::string, std::string> parseFileName(std::string fileName) {
    size_t dot_pos = fileName.length();

    while(dot_pos > 0 && fileName[dot_pos] != '.')
        dot_pos--;

    return std::tuple<std::string, std::string>(fileName.substr(0, dot_pos), fileName.substr(dot_pos + 1));
}

bool prepareHJParams(pmols::HJParams &params) {
    // 1. Check size of box
    if(params.box_length <= 0) {
        if(params.box_length == 0)
            std::cerr << "ERROR: Initial length of packing box is not specified" << std::endl;
        else
            std::cerr << "ERROR: Intolerable length of packing box. Length should be positive number" << std::endl;
        return false;
    }
    if(params.box_width <= 0) {
        if(params.box_width == 0)
            std::cerr << "ERROR: Initial width of packing box is not specified" << std::endl;
        else
            std::cerr << "ERROR: Intolerable width of packing box. Width should be positive number" << std::endl;
        return false;
    }
    if(params.box_height <= 0) {
        if(params.box_height == 0)
            std::cerr << "ERROR: Initial height of packing box is not specified" << std::endl;
        else
            std::cerr << "ERROR: Intolerable height of packing box. Height should be positive number" << std::endl;
        return false;
    }

    // 2. Check input molecule file
    if(params.mol_file == "") {
        std::cerr << "ERROR: Molecule file is not specified" << std::endl;
        return false;
    }
    else {
        FILE *fp = fopen(params.mol_file.c_str(), "r");
        if(fp == NULL) {
            std::cerr << "ERROR: Could not open molecule file. Are you sure that path to the file is correct?"
                      << std::endl;
            return false;
        }
        fclose(fp);
    }

    // 3. Check output file and format
    if(params.out_file == "" || params.out_format == "") {
        std::string of_name, of_format;
        std::tie(of_name, of_format) = parseFileName(params.mol_file);

        if(params.out_format == "")
            params.out_format = of_format;

        if(params.out_file == "") {
            std::string bs_pref =   "_" + floatToString(params.box_length, 2)
                                    + "x" + floatToString(params.box_width, 2)
                                    + "x" + floatToString(params.box_height, 2);
            params.out_file = of_name + bs_pref + "." + params.out_format;
        }
    }


    // 4. Check cell_length
    if(params.cell_length <= 0) {
        if(params.cell_length == 0) {
            std::cout << "WARNING: Initial cell length of cell linked lists is not specified. Using default value: ";
            pmols::Molecule mol(params.mol_file);

            float max_radius = 0;
            for (int i = 0; i < mol.AtomsCount(); ++i) {
                if(mol.GetAtom(i).vdw_radius > max_radius)
                    max_radius = mol.GetAtom(i).vdw_radius;
            }
            params.cell_length = 2 * max_radius;
            std::cout << "cell_length: " << params.cell_length << std::endl;
        }
        else {
            std::cerr
                    << "ERROR: Intolerable cell length of cell linked lists. Cell linked lists should be positive number"
                    << std::endl;;
            return false;
        }
    }
    if(params.substance == "") {
        pmols::Molecule mol(params.mol_file);
        params.substance = mol.GetFormula();
    }


    return true;
}

JSONConfigParser::JSONConfigParser(pmols::HJParams &params, std::istream &jsonInputStream) : params(params) {
    rapidjson::IStreamWrapper isw(jsonInputStream);
    doc.ParseStream(isw);
}

void JSONConfigParser::Parse() {
    if(doc.HasMember("lattice"))
        parseLattice();
    if(doc.HasMember("packing_params"))
        parsePackingParams();
    if(doc.HasMember("cell_linked_lists"))
        parseCellLinkedLists();
}

void JSONConfigParser::parseFloats(rapidjson::Value &obj, std::map<std::string, int> &keyToField) {
    assert(obj.IsObject());

    int field_idx;
    std::string field_key;

    for (rapidjson::Value::ConstMemberIterator itr = obj.MemberBegin(); itr != obj.MemberEnd(); ++itr) {
        field_key = itr->name.GetString();

        if(itr->value.IsNumber() && keyToField.count(field_key)) {
            field_idx = keyToField.at(field_key);
            if(!params.val_setted[field_idx])
                setParamByIndex(itr->value.GetFloat(), field_idx);
        }
    }
}

void JSONConfigParser::setParamByIndex(float val, int idx) {
    float *fParams = (float*)&params;
    *(fParams + idx) = val;
}

void JSONConfigParser::parseLattice() {
    assert(doc["lattice"].IsObject());

    std::map<std::string, int> fkey_fieldIdx = {
            {"box_length", 0},
            {"box_width", 1},
            {"box_height", 2},
            {"expansivity", 3},
    };
    parseFloats(doc["lattice"], fkey_fieldIdx);

    if(doc["lattice"].HasMember("substance")) {
        assert(doc["lattice"]["substance"].IsString());
        if(!params.val_setted[16])
            params.substance = doc["lattice"]["substance"].GetString();
    }
    if(doc["lattice"].HasMember("mol_file")) {
        assert(doc["lattice"]["mol_file"].IsString());
        if(!params.val_setted[17])
            params.mol_file = doc["lattice"]["mol_file"].GetString();
    }
    if(doc["lattice"].HasMember("out_file")) {
        assert(doc["lattice"]["out_file"].IsString());
        if(!params.val_setted[18])
            params.out_file = doc["lattice"]["out_file"].GetString();
    }
    if(doc["lattice"].HasMember("out_format")) {
        assert(doc["lattice"]["out_format"].IsString());
        if(!params.val_setted[19])
            params.out_format = doc["lattice"]["out_format"].GetString();
    }
}

void JSONConfigParser::parsePackingParams() {
    assert(doc["packing_params"].IsObject());

    std::map<std::string, int> fkey_fieldIdx = {
            {"trans_x", 4},
            {"trans_y", 5},
            {"trans_z", 6},
            {"rot_x", 7},
            {"rot_y", 8},
            {"rot_z", 9},
            {"lambda", 10},
            {"dec_coefficient", 11},
            {"trans_eps", 12},
            {"rot_eps", 13},
            {"lj_epsilon", 14}
    };
    parseFloats(doc["packing_params"], fkey_fieldIdx);

    if(doc["packing_params"].HasMember("steps")) {
        parseFloats(doc["packing_params"]["steps"], fkey_fieldIdx);
    }
    if(doc["packing_params"].HasMember("precision")) {
        parseFloats(doc["packing_params"]["precision"], fkey_fieldIdx);
    }
}

void JSONConfigParser::parseCellLinkedLists() {
    assert(doc["cell_linked_lists"].IsObject());

    std::map<std::string, int> fkey_fieldIdx = {{"cell_length", 15}};
    parseFloats(doc["cell_linked_lists"], fkey_fieldIdx);
}

void JSONConfigParser::ParseFromFile(pmols::HJParams &params, std::string confFile) {
    std::ifstream ifs(confFile);
    JSONConfigParser parser(params, ifs);
    parser.Parse();
}


CommandLineParser::CommandLineParser(int argc, const char **argv) {
    this->argc = argc;
    this->argv = argv;
    this->conf_file = "";
}

void CommandLineParser::parseFloat(boost::program_options::variables_map vm, std::map<std::string, int> paramNameToFieldIdx) {
    int field_idx;
    float field_val;

    for(const auto& it : vm) {
        if(paramNameToFieldIdx.count(it.first)) {
            field_idx = paramNameToFieldIdx[it.first];
            field_val = it.second.as<float>();

            *((float*)&params + field_idx) = field_val;
            params.val_setted[field_idx] = true;
        }
    }
}

void CommandLineParser::Parse() {
    boost::program_options::options_description gen_dec("General options");

    gen_dec.add_options()
            ("help,h", "Show help")
            ("config,c", boost::program_options::value<std::string>(), "Set JSON configuration file.")
            ("length,L", boost::program_options::value<float>(), "Set lattice length.")
            ("width,W", boost::program_options::value<float>(), "Set lattice width.")
            ("height,H", boost::program_options::value<float>(), "Set lattice height.")
            ("input,I", boost::program_options::value<std::string>(), "Set input molecule prototype file")
            ("output,O", boost::program_options::value<std::string>(), "Set output lattice file")
            ("format,F", boost::program_options::value<std::string>(), "Set output format")
            ("substance,S", boost::program_options::value<std::string>(), "Set substance naming of initial molecule")
            ("expansivity,E", boost::program_options::value<float>(), "Set expansivity coefficient of packing box")
            ("cell_length,C", boost::program_options::value<float>(), "Set unit cell length of Cell Linked Lists")
            ("log_dir", boost::program_options::value<std::string>(), "Set directory where logs will be putted")
            ("trans_x", boost::program_options::value<float>(), "Set initial shift in Hooke Jeeves algorithm in X axis")
            ("trans_y", boost::program_options::value<float>(), "Set initial shift in Hooke Jeeves algorithm in Y axis")
            ("trans_z", boost::program_options::value<float>(), "Set initial shift in Hooke Jeeves algorithm in Z axis")
            ("rot_x", boost::program_options::value<float>(), "Set initial rotate in Hooke Jeeves algorithm around X axis")
            ("rot_y", boost::program_options::value<float>(), "Set initial rotate in Hooke Jeeves algorithm around Y axis")
            ("rot_z", boost::program_options::value<float>(), "Set initial rotate in Hooke Jeeves algorithm around Z axis")
            ("step_coefficient", boost::program_options::value<float>(), "Set decreasing coefficient in Hooke Jeeves algorithm (from 0 to 1)")
            ("rot_eps", boost::program_options::value<float>(), "Set precision of angle step in Hooke Jeeves algorithm")
            ("trans_eps", boost::program_options::value<float>(), "Set precision of shift step in Hooke Jeeves algorithm")
            ("lambda", boost::program_options::value<float>(), "Set coefficient in pattern search of Hooke Jeeves algorithm")
            ("lj_epsilon", boost::program_options::value<float>(), "Set coefficient in Lennard Jones energy equation");

    boost::program_options::variables_map vm;
    boost::program_options::parsed_options parsed = boost::program_options::parse_command_line(argc, argv, gen_dec);
    boost::program_options::store(parsed, vm);
    boost::program_options::notify(vm);


    if(vm.count("help")) {
            std::cout << "Molecules' packer" << std::endl << std::endl;
            std::cout << gen_dec << std::endl;
            std::exit(0);
    }
    if(vm.count("log_dir")) {
            params.log_dir = vm.at("log_dir").as<std::string>();
    }
    if(vm.count("substance")) {
        params.substance = vm.at("substance").as<std::string>();
        params.val_setted[16] = true;
    }
    if(vm.count("input")) {
        params.mol_file = vm.at("input").as<std::string>();
        params.val_setted[17] = true;
    }
    if(vm.count("output")) {
        params.out_file = vm.at("output").as<std::string>();
        params.val_setted[18] = true;
    }
    if(vm.count("format")) {
        params.out_format = vm.at("format").as<std::string>();
        params.val_setted[19] = true;
    }
    if(vm.count("config")) {
        conf_file = vm.at("config").as<std::string>();
    }

    std::map<std::string, int> paramToFloatFields = {
            {"length", 0 },         {"width", 1},
            {"height", 2},          {"expansivity", 3},
            {"trans_x", 4},         {"trans_y", 5},
            {"trans_z", 6},         {"rot_x", 7},
            {"rot_y", 8},           {"rot_z", 9},
            {"lambda", 10},         {"step_coefficient", 11},
            {"trans_eps", 12},      {"rot_eps", 13},
            {"lj_epsilon", 14},     {"cell_length", 15}
    };

    parseFloat(vm, paramToFloatFields);
}

std::tuple<std::string, pmols::HJParams> CommandLineParser::GetParseResult() {
    return std::tuple<std::string, pmols::HJParams>(conf_file, params);
}
