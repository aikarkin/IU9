#include <MolLattice.h>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>

Molecule *mol_struct;
MolCubeShell *shell;
std::vector<Molecule> mols;

HJLattice *hjLattice;
ShellCellLattice *scLattice;

float cell_len;

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

    void deserialize(std::string imp_file) {
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

void generateHJLattice(MolPackingParams& params) {
    mol_struct = new Molecule(params.mol_file);
    hjLattice = new HJLattice(*mol_struct);
    hjLattice->setBoxSize(params.a, params.b, params.c);
    hjLattice->setPrecision(0.5f, 0.5f, 0.5f, 0.01f, 0.01f, 0.01f);
    hjLattice->packMax();

    mols = hjLattice->getMolecules();
}

void generateSCLattice(MolPackingParams &params) {
    mol_struct = new Molecule(params.mol_file);
    shell = new MolCubeShell(mol_struct);

    // calc cell length
    float mol_wt = (float)mol_struct->OBMol().GetMolWt();
    float k = 0.60221413f; // N_a*10^(-24)
    cell_len = powf(mol_wt/(k*params.density), 1.f/3.f);

    scLattice = new ShellCellLattice(*shell, glm::vec3(0, 0, 0), params.a, params.b, params.c, cell_len);
    mols = scLattice->getMolecules();
}

int main(int argc, char *argv[]) {
    std::string imp_file;
    std::string out_file;
    bool outf_matched = false;

    if(argc >= 2) {
        imp_file = argv[1];
        if (argc == 3) {
            out_file = argv[2];
            outf_matched = true;
        }
        else if(argc > 3) {
            std::cout << "Result: failed" << std::endl;
            std::cerr << "error: invalid number of arguments" << std::endl;
            return 0;
        }
    }
    else {
        std::cout << "hjLattice configuration file:";
        std::getline(std::cin, imp_file);
    }

    MolPackingParams params;
    params.deserialize(imp_file);

    if(!outf_matched) {
        std::vector<std::string> split_res = split(imp_file, '.');
        out_file = imp_file.substr(0, imp_file.length() - split_res.back().length() - 1) + "." + params.output_format;
        outf_matched = true;
    }

    generateHJLattice(params);
    //generateSCLattice(params);

    /*std::vector<float> res_box_size(hjLattice->getLatticeSize());
    CellLinkedLists cllLists(2.8f, res_box_size[0], res_box_size[1], res_box_size[2]);

    for (int i = 0; i < mols.size(); ++i) {
        cllLists.addMol(mols[i]);
    }

    Atom *cur_atom;

    std::cout << "CellLinkedLists testing ..." << std::endl;
    std::cout << "cell length of CLL: " << cllLists.GetCellLength() << std::endl;

    std::cout << "---------------\nDistance to neighbour atoms:" << std::endl;
    for (int i = 0; i < mols.size(); ++i) {
        std::cout << "Molecule #" << i << std::endl;
        for (int j = 0; j < mols[i].AtomsCount(); ++j) {
            std::cout << "\tAtom #" << j << " with coordinates " <<vec_to_string(mols[i].GetAtom(j).coord) << std::endl;
            std::cout << "\tadjacent atoms count:" << cllLists.getNeighbours(&mols[i].GetAtom(j)).size() << std::endl;

            for(auto cell_atom : cllLists.getNeighbours(&mols[i].GetAtom(j))) {
                std::cout << "\t\tdistance to atom with coordinates " << vec_to_string(cell_atom.coord) << " is " <<
                          (float)glm::distance(mols[i].GetAtom(j).coord, cell_atom.coord)
                          << std::endl;
            }
        }
    }
    std::cout << "----------------" << std::endl;
*/
    if (mols.size() > 0) {
        std::filebuf fb;
        fb.open (out_file, std::ios::out);

        if(!fb.is_open()) {
            std::cout << "Result: failed" << std::endl;
            std::cerr << "error: Can't write to output file" << std::endl;
        }

        std::ostream out_stream(&fb);


        OpenBabel::OBConversion obConversion;
        obConversion.SetOutStream(&out_stream);
        obConversion.SetOutFormat(params.output_format.c_str(), false);

        std::shared_ptr<OpenBabel::OBMol> mol;

        for (int i = 0; i < mols.size(); ++i) {
            mol = std::make_shared<OpenBabel::OBMol>(mols[i].OBMol());
            obConversion.Write(mol.get());
        }

        fb.close();

        std::cout << "Result: success\nMolecules packed: " << mols.size()
                  /* << "\nBox size: " << res_box_size[0] << "x" << res_box_size[1] << "x" << res_box_size[2]
                   << "\nEdge length of cell: " << cell_len */ << std::endl;
    }
    else {
        std::cout << "Result: failed" << std::endl;
        std::cerr << "unknown error" << std::endl;
    }

    delete mol_struct;
    delete hjLattice;

    return 0;
}