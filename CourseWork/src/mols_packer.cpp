#include <MolLattice.h>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>

Molecule *mol_struct;
MolCubeShell *shell;
MolLattice *lattice;
std::vector<Molecule> mols;

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
    float a, b, c;
    float density;

    void deserialize(std::string imp_file) {
        std::cout << "imp_file: " << imp_file << std::endl;
        std::ifstream imp_stream(imp_file);
        std::string cur_line;
        std::string param;
        std::string val;

        std::cout << "parsing file" << std::endl;

        while(std::getline(imp_stream, cur_line)) {
            std::vector<std::string> param_val = split(cur_line, ':');
            param = param_val[0];
            val = param_val[1];
            std::cout << param << ": " << val << std::endl;
            if (param == "mol_file") {
                mol_file = val;
            }
            else if (param == "box_size") {
                std::vector<std::string> size_params = split(val, ' ');
                std::cout << "box size param: " << size_params[0] << "x" << size_params[1] << "x" << size_params[2] << std::endl;
                a = std::stof(size_params[0].c_str());
                b = std::stof(size_params[1].c_str());
                c = std::stof(size_params[2].c_str());
            }
            else if (param == "density") {
                density = std::stof(val);
            }

        }
    }
};

void generateLattice(MolPackingParams &params) {
    mol_struct = new Molecule(params.mol_file);
    shell = new MolCubeShell(mol_struct);
    lattice = new MolLattice(*shell, params.a, params.b, params.c, 0);
    mols = lattice->getMolecules();
}

int main() {
    std::string imp_file = "/home/alex/dev/src/cpp/CourseWork/resources/H2O.imp";

    MolPackingParams params;
    params.deserialize(imp_file);

    std::cout << "parsed params from file: " << std::endl;
    std::cout << params.mol_file << std::endl;
    std::cout << params.density << std::endl;
    std::cout << params.a << "x" << params.b << "x" << params.c << std::endl;

    generateLattice(params);


    std::string sdf_file = "/home/alex/dev/src/cpp/CourseWork/resources/H2O.pdb";
    std::filebuf fb;
    fb.open (sdf_file, std::ios::out);
    std::ostream out_stream(&fb);

    OpenBabel::OBConversion obConversion;
    obConversion.SetOutStream(&out_stream);
    obConversion.SetOutFormat("pdb", false);

    std::shared_ptr<OpenBabel::OBMol> mol;
    std::cout << "Success!!! " << mols.size() << " have been packed" << std::endl;
    for (int i = 0; i < mols.size(); ++i) {
        mol = std::make_shared<OpenBabel::OBMol>(mols[i].OBMol());
        obConversion.Write(mol.get());
    }

    fb.close();
    
    delete mol_struct;
    delete shell;
    delete lattice;

    return 0;
}