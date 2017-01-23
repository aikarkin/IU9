#include <mol_lattice.h>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>
#include <iomanip>
namespace po = boost::program_options;

std::vector<Molecule> mols;
SimpleLattice *lattice;

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

struct MolPackingParams {
    std::string mol_file;
    std::string output_format = "pdb";
    float a, b, c;
    float density;

    void load_from_file(std::string imp_file) {
        std::ifstream imp_stream(imp_file);
        if(!imp_stream.is_open()) {
            std::cout << "Result: failed" << std::endl;
            std::cerr << "error: cannot open lattice configuration file - file not exists" << std::endl;
            return;
        }
        std::string cur_line;
        std::string param;
        std::string val;


        while(std::getline(imp_stream, cur_line)) {
            std::vector<std::string> param_val = split(cur_line, ':');
            param = param_val[0];
            val = param_val[1];
            if (param == "mol_file") {
                mol_file = val;
            }
            else if (param == "box_size") {
                std::vector<std::string> size_params = split(val, ' ');
                a = std::stof(size_params[0].c_str());
                b = std::stof(size_params[1].c_str());
                c = std::stof(size_params[2].c_str());
            }
            else if (param == "density") {
                density = std::stof(val);
            }
            else if (param == "output_format") {
                output_format = val;
            }
        }
    }
};

void generateLattice(MolPackingParams &params) {
    Molecule mol(params.mol_file);
    lattice = new SimpleLattice(mol, params.a, params.b, params.c);
    mols = lattice->getMolecules();
}

int main(int argc, const char *argv[]) {
    std::string imp_file;
    std::string out_file;
    bool outf_matched = false;

    po::options_description gen_dec("General options");
    gen_dec.add_options()
            ("help,h", "Show help")
            ("length,L", po::value<float>(), "Set lattice length.")
            ("width,W", po::value<float>(), "Set lattice width.")
            ("height,H", po::value<float>(), "Set lattice height.")
            ("input,i", po::value<std::string>(), "Set input Molecule prototype file")
            ("output,O", po::value<std::string>(), "Output lattice file")
            ("format,f", po::value<std::string>(), "Output format")
            ("dir,d", po::value<std::string>(), "Output directory");

    po::variables_map vm;
    po::parsed_options parsed = po::parse_command_line(argc, argv, gen_dec);
    po::store(parsed, vm);
    po::notify(vm);


    MolPackingParams params;

    if(vm.count("help")) {
        std::cout << "Molecules' closest packer" << std::endl << std::endl;
        std::cout << gen_dec << std::endl;
        return 0;
    }

    if(vm.count("length") && vm.count("width") && vm.count("height")) {
        params.a = vm["length"].as<float>();
        params.b = vm["width"].as<float>();
        params.c = vm["height"].as<float>();

        std::cout << "size: " << params.a << ", " << params.b << ", " << params.c << std::endl;
    }
    else {
        std::cerr << "Error: size required." << std::endl;
        return 0;
    }

    if(vm.count("input")) {
        params.mol_file = vm["input"].as<std::string>();
        std::cout << "mol_file: " << params.mol_file << std::endl;
    }
    else {
        std::cout << "Error: Molecule prototype is not matched" << std::endl;
        return 0;
    }

    if(vm.count("format")) {
        params.output_format = vm["format"].as<std::string>();
        std::cout << "format: " << params.output_format << std::endl;
    }
    else {
        params.output_format = split(params.mol_file, '.').back();
    }

    if(vm.count("output")) {
        out_file = vm["output"].as<std::string>();
        std::cout << "out file: " << out_file << std::endl;
    }
    else {
        std::string base = "";
        std::vector<std::string> dirs = split(params.mol_file, '/');
        std::string fname = (dirs.back());
        std::vector<std::string> split_res = split(fname, '.');
        if(vm.count("dir"))
            base = vm["dir"].as<std::string>();
        else {
            for (int i = 0; i < dirs.size() - 1; ++i) {
                base += dirs[i] + '/';
            }
        }
        out_file = base + fname.substr(0, fname.length() - split_res.back().length() - 1)
                   + "_" + float_to_string(params.a, 3) + "x"
                   + float_to_string(params.b, 3) + "x"
                   + float_to_string(params.c, 3) + "." + params.output_format;

        std::cout << "out_file: " << out_file << std::endl;
    }

    generateLattice(params);

    if (mols.size() > 0) {
        lattice->saveToFile(out_file, params.output_format);
    }
    else {
        std::cout << "Result: failed" << std::endl;
        std::cerr << "unknown error" << std::endl;
    }

    delete lattice;
    return 0;
}