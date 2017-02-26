#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>
#include <iomanip>
#include <hj_algo.h>

namespace po = boost::program_options;

std::string float_to_string(float num, int prec) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(prec) << num;
    std::string s = stream.str();
    return s;
}

std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> internal;
    std::stringstream ss(str);
    std::string tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}

pmols::HJParams parseHJParams(std::string confFile) {
    pmols::HJParams params;
    std::string line;
    std::ifstream confStream(confFile);
    std::vector<std::string> splitted;
    std::string key, value;
    float float_vals[14];

    std::map<std::string, int> key_ind = {
            {"box_length", 0},
            {"box_width", 1},
            {"box_height", 2},
            {"expansivity", 3},
            {"step_x", 4},
            {"step_y", 5},
            {"step_z", 6},
            {"step_alpha", 7},
            {"step_betta", 8},
            {"step_gamma", 9},
            {"lambda", 10},
            {"step_coefficient", 11},
            {"trans_eps", 12},
            {"rot_eps", 13},
    };
    std::string mol_file = "";
    std::string out_file = "";
    std::string out_format = "";

    int i = 0;

    while(std::getline(confStream, line)) {
        i++;
        splitted = split(line, ':');
        key = splitted[0];
        value = splitted[1];

        if(key_ind.count(key)) {
            float_vals[key_ind[key]] = std::stof(value);
        }
        else {
            if(key == "mol_file") {
                mol_file = value;
            }
            else if(key == "out_file") {
                out_file = value;
            }
            else if(key == "out_format") {
                out_format = value;
            }
            else {
                std::cout << "WARNING: unknown key at line " << i << ". Key " + key + " was skipped";
            }
        }
    }

    memcpy((void*)&params, (void*)float_vals, 14 * sizeof(float));

    params.mol_file = mol_file;
    params.out_file = out_file;
    params.out_format = out_format;

    return params;
}

int main(int argc, const char *argv[]) {
//    std::string imp_file;
//    std::string out_file;
//    bool outf_matched = false;
//
//    po::options_description gen_dec("General options");
//    gen_dec.add_options()
//            ("help,h", "Show help")
//            ("length,L", po::value<float>(), "Set lattice length.")
//            ("width,W", po::value<float>(), "Set lattice width.")
//            ("height,H", po::value<float>(), "Set lattice height.")
//            ("input,i", po::value<std::string>(), "Set input Molecule prototype file")
//            ("output,O", po::value<std::string>(), "Output lattice file")
//            ("format,f", po::value<std::string>(), "Output format")
//            ("dir,d", po::value<std::string>(), "Output directory");
//
//    po::variables_map vm;
//    po::parsed_options parsed = po::parse_command_line(argc, argv, gen_dec);
//    po::store(parsed, vm);
//    po::notify(vm);
    pmols::HJParams params = parseHJParams("../resources/hj_lattice.conf");
    pmols::HJPacker packer(params);
    packer.Pack();
    packer.Save();

    return 0;
}
