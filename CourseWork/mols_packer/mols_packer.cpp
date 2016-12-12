#include <MolLattice.h>
#include <CellLinkedLists.h>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>

Molecule *mol_struct;
MolCubeShell *shell;
ShellCellLattice *lattice;
std::vector<Molecule> mols;
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
    mol_struct = new Molecule(params.mol_file);
    shell = new MolCubeShell(mol_struct);

    // calc cell length
    float mol_wt = (float)mol_struct->OBMol().GetMolWt();
    float k = 0.60221413f; // N_a*10^(-24)
    cell_len = powf(mol_wt/(k*params.density), 1.f/3.f);

    lattice = new ShellCellLattice(*shell, glm::vec3(0, 0, 0), params.a, params.b, params.c, cell_len);
    mols = lattice->getMolecules();
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
            std::cerr << "error: invalid number of arguments" << std::endl;
            return 0;
        }
    }
    else {
        std::getline(std::cin, imp_file);
    }

    MolPackingParams params;
    params.deserialize(imp_file);

    if(!outf_matched) {
        std::vector<std::string> split_res = split(imp_file, '.');
        out_file = imp_file.substr(0, imp_file.length() - split_res.back().length() - 1) + "." + params.output_format;
        outf_matched = true;
    }

    generateLattice(params);
    std::vector<float> res_box_size(lattice->getLatticeSize());
    CellLinkedLists cllLists(1.4f, res_box_size[0], res_box_size[1], res_box_size[2]);

    for (int i = 0; i < mols.size(); ++i) {
        for (int j = 0; j < mols[i].AtomsCount(); ++j) {
            std::cout << "(" << i << ", " << j << ")" << std::endl;
            std::cout << "coord: " << vec_to_string(mols[i].GetAtom(j).coord) << std::endl;
            cllLists.addAtom(&mols[i].GetAtom(j));
        }
    }

    std::cout << "--------------" << std::endl;
    Atom *cur_atom;

    for (int i = 0; i < res_box_size[0]; ++i) {
        for (int j = 0; j < res_box_size[1]; ++j) {
            for (int k = 0; k < res_box_size[2]; ++k) {
                cur_atom = cllLists.getAtomByInd(i, j, k);
                if(cur_atom != NULL)
                    std::cout << "[" << i << ", " << j << ", " << k << "]: " << vec_to_string(cur_atom->coord) << std::endl;
            }
        }
    }
    std::cout << "cell length of CLL: " << cllLists.GetCellLength() << std::endl;

    std::cout << "---------------\nDistance to neighbour atoms:" << std::endl;
    for (int i = 0; i < mols.size(); ++i) {
        std::cout << "Molecule #" << i << std::endl;
        for (int j = 0; j < mols[i].AtomsCount(); ++j) {
            std::cout << "\tAtom #" << j << " with coordinates " <<vec_to_string(mols[i].GetAtom(j).coord) << std::endl;
            std::cout << "\tadjacent atoms count:" << cllLists.getNeighbours(&mols[i].GetAtom(j)).size() << std::endl;
            /*CLLNeighbourAtoms neighbours = cllLists.getNeighbours(&mols[i].GetAtom(j));
            std::cout << "--- first atom: " << vec_to_string(neighbours.begin().operator*().coord) << std::endl;
            std::cout << "--- last atom: " << vec_to_string(neighbours.end().operator*().coord) << std::endl;*/

            for(auto cell_atom : cllLists.getNeighbours(&mols[i].GetAtom(j))) {
                std::cout << "\t\tdistance to atom " << vec_to_string(cell_atom.coord) << " is " <<
                          (float)glm::distance(mols[i].GetAtom(j).coord, cell_atom.coord)
                          << std::endl;
            }
        }
    }
    std::cout << "----------------" << std::endl;

    std::filebuf fb;
    fb.open (out_file, std::ios::out);
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

    if (mols.size() > 0) {
        std::cout << "Result: success\nMolecules packed: " << mols.size()
                  << "\nBox size: " << res_box_size[0] << "x" << res_box_size[1] << "x" << res_box_size[2]
                  << "\nEdge length of cell: " << cell_len << std::endl;
    }

    delete mol_struct;
    delete shell;
    delete lattice;

    return 0;
}