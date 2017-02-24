//
// Created by alex on 09.12.16.
//

#include <mol.h>
#include <cell_linked_lists.h>
#include <memory>

pmols::CellLinkedLists::CellLinkedLists(float cell_length, float len_a, float len_b, float len_c) {
    cell_len = cell_length;
    
    cell_len_setted = true;
    size_setted = true;

    box_length = len_a;
    box_width = len_b;
    box_height = len_c;

    formCellLinkedLists();
}

pmols::CellLinkedLists::~CellLinkedLists() {
  //  if(atom_grid != NULL)
        freeAtomGrid();
}

bool pmols::CellLinkedLists::addAtom(Atom &atom) {
    int i = (int)floorf(atom.coord.x/cell_len);
    int j = (int)floorf(atom.coord.y/cell_len);
    int k = (int)floorf(atom.coord.z/cell_len);

    if (i < 0 || i >= atoms_count_x ||
        j < 0 || j >= atoms_count_y ||
        k < 0 || k >= atoms_count_z) {
        return false;
    }
    
    atom_grid[i][j][k].emplace_back(atom);

    return true;
}

bool pmols::CellLinkedLists::remAtom(Atom &atom) {
    int i = (int)floorf(atom.coord.x/cell_len);
    int j = (int)floorf(atom.coord.y/cell_len);
    int k = (int)floorf(atom.coord.z/cell_len);

    if (i < 0 || i >= atoms_count_x ||
        j < 0 || j >= atoms_count_y ||
        k < 0 || k >= atoms_count_z)
        return false;

    for(auto atom_it = atom_grid[i][j][k].begin(); atom_it != atom_grid[i][j][k].end(); atom_it++) {
        if(atom_it->atom_idx == atom.atom_idx) {
            atom_grid[i][j][k].erase(atom_it);
            return true;
        }
    }

    return false;
}

float pmols::CellLinkedLists::GetCellLength() {
    return cell_len;
}

std::list<pmols::Atom> pmols::CellLinkedLists::getCellListByInd(int i, int j, int k) {
    int n = atoms_count_x;
    int m = atoms_count_y;
    int p = atoms_count_z;

    if (i < 0 || i >= n ||
            j < 0 || j >= m ||
            k < 0 || k >= p) {
        std::cerr << "Error: CellLinkedLists - index out of range" << std::endl;
        return std::list<Atom>();
    }

    return atom_grid[i][j][k];
}

pmols::CLLNeighbourCells pmols::CellLinkedLists::getNeighbourCells(Atom &atom) {
    return CLLNeighbourCells(this, atom);
}

pmols::CellLinkedLists::CellLinkedLists() {
    cell_len_setted = false;
    size_setted = false;
    //atom_grid = NULL;
}

void pmols::CellLinkedLists::formCellLinkedLists() {
    atoms_count_x = (int)ceilf(box_length/cell_len);
    atoms_count_y = (int)ceilf(box_width/cell_len);
    atoms_count_z = (int)ceilf(box_height/cell_len);

    atom_grid = new std::list<Atom>**[atoms_count_x];

    for (int i = 0; i < atoms_count_x; ++i) {
        atom_grid[i] = new std::list<Atom>*[atoms_count_y];
        for (int j = 0; j < atoms_count_y; ++j) {
            atom_grid[i][j] = new std::list<Atom>[atoms_count_z];
        }
    }
}

void pmols::CellLinkedLists::setCellLength(float cell_length) {
    cell_len = cell_length;
    
    cell_len_setted = true;
    
    if (size_setted)
        formCellLinkedLists();
}

void pmols::CellLinkedLists::setSize(float len_a, float len_b, float len_c) {
    box_length = len_a;
    box_width = len_b;
    box_height = len_c;
    
    size_setted = true;

    if(cell_len_setted)
        formCellLinkedLists();
}

void pmols::CellLinkedLists::freeAtomGrid() {
    for(int i = 0; i < atoms_count_x; i++) {
        for (int j = 0; j < atoms_count_y; ++j) {
            for (int k = 0; k < atoms_count_z; ++k) {
                if(!atom_grid[i][j][k].empty())
                    atom_grid[i][j][k].clear();
            }
        }
    }

    for (int i = 0; i < atoms_count_y; ++i) {
        for (int j = 0; j < atoms_count_z; ++j) {
            delete []atom_grid[i][j];
        }
        delete []atom_grid[i];
    }

    delete []atom_grid;
}

bool pmols::CellLinkedLists::repAtom(Atom &old_atom, Atom &new_atom) {
    return remAtom(old_atom) && addAtom(new_atom);
}

bool pmols::CellLinkedLists::addMol(Molecule &mol) {
    for (int i = 0; i < mol.AtomsCount(); ++i) {
        if(!addAtom(mol.GetAtom(i))) {
            for(int j = i; j >= 0; --j) {
                remAtom(mol.GetAtom(j));
            }
            return false;
        }
    }

    return true;
}

bool pmols::CellLinkedLists::remMol(Molecule &mol) {
    for (int i = 0; i < mol.AtomsCount(); ++i) {
        if(!remAtom(mol.GetAtom(i))) {
            for(int j = i; j >= 0; --j) {
                addAtom(mol.GetAtom(j));
            }

            return false;
        }
    }

    return true;
}

bool pmols::CellLinkedLists::repMol(Molecule &old_mol, Molecule &new_mol) {
    bool rem_mol_res = remMol(old_mol);
    bool add_mol_res = addMol(new_mol);

    return rem_mol_res && add_mol_res;
}

bool pmols::CellLinkedLists::moveMol(Molecule &mol, MoveOperation move_op, float val) {
    Molecule old_mol = mol;

    glm::vec3 vec_i(1, 0, 0);
    glm::vec3 vec_j(0, 1, 0);
    glm::vec3 vec_k(0, 0, 1);

    switch(move_op) {
        case ROT_X:
            mol.RotateX(val);
            break;
        case ROT_Y:
            mol.RotateY(val);
            break;
        case ROT_Z:
            mol.RotateZ(val);
            break;
        case TRANS_X:
            mol.Translate(val * vec_i);
            break;
        case TRANS_Y:
            mol.Translate(val * vec_j);
            break;
        case TRANS_Z:
            mol.Translate(val * vec_k);
            break;
        default:
            break;
    }

    return repMol(old_mol, mol);
}

float pmols::CellLinkedLists::totalNAtomsDist(Molecule &mol) {
    float sum = 0;
    float cur_dist = 0;

    for (int i = 0; i < mol.AtomsCount(); ++i) {
        CLLNeighbourCells neighbourCells = getNeighbourCells(mol.GetAtom(i));
        while(neighbourCells.hasNext()) {
            std::list<Atom> cell_list = neighbourCells.next();
            if(cell_list.empty())
                sum += 6.0;
            else {
                for (auto n_atom : cell_list) {
                    if (mol.GetAtom(i).parent_mol_id != n_atom.parent_mol_id) {
                        cur_dist = std::fabs(glm::distance(mol.GetAtom(i).coord, n_atom.coord)
                                             - mol.GetAtom(i).vdw_radius - n_atom.vdw_radius);
                        sum += cur_dist;
                    }
                }
            }
        }
    }

    return sum;
}

boost::tuple<int, int, int> pmols::CellLinkedLists::atomsCountPerAxis() {
    return boost::tuple<int, int, int>(atoms_count_x, atoms_count_y, atoms_count_z);
}

void pmols::CellLinkedLists::saveToCSV(std::string file_path) {
    std::ofstream outf_stream(file_path);

    if(!outf_stream.is_open()) {
        outf_stream.close();
        std::cerr << "Error: unable to save Cell Linked Lists to CSV. File could not be open." << std::endl;
        return;
    }
    for (int k = 0; k < atoms_count_z; ++k) {
        outf_stream << "\"Layer #" << k + 1 << "\"" << std::endl;
        for (int j = 0; j < atoms_count_y; ++j) {
            for (int i = 0; i < atoms_count_x; ++i) {
                std::list<Atom> cell_list = getCellListByInd(i, j, k);
                outf_stream << "\"";

                if(!cell_list.empty()) {
                    for(auto cell_atom_it = cell_list.begin(); cell_atom_it != cell_list.end(); cell_atom_it++) {
                        std::string atom_info = cell_atom_it->toString();
                        outf_stream << atom_info.c_str() << ";" << std::endl;
                    }
                }
                else {
                    outf_stream << "EMPTY";
                }

                outf_stream << "\", ";
            }
            outf_stream << std::endl;
        }
        outf_stream << std::endl;
    }

    outf_stream.close();
}

pmols::CLLNeighbourCells::CLLNeighbourCells(CellLinkedLists *clLists, Atom &initialAtom) {
    this->clLists = clLists;
    this->initial_atom = initialAtom;
    boost::tuple<int, int, int> atoms_count = clLists->atomsCountPerAxis();

    atoms_count_x = atoms_count.get<0>();
    atoms_count_y = atoms_count.get<1>();
    atoms_count_z = atoms_count.get<2>();
    reset();
}

std::list<pmols::Atom> pmols::CLLNeighbourCells::next() {
    int x;
    int bit_counter;
    char trans_code_bits[6] = {0};
    char left, right, up, down, far, near;
    int i, j, k;
    int trans_code_step;

    int old_trans_code = trans_code;

    do {
        x = trans_code;
        bit_counter = 5;

        while (x > 0) {
            trans_code_bits[bit_counter--] = (char) (x % 2);
            x /= 2;
        }
        for (int l = bit_counter; l >= 0; --l) {
            trans_code_bits[l] = 0;
        }

        left = trans_code_bits[0];
        right = trans_code_bits[1];
        up = trans_code_bits[2];
        down = trans_code_bits[3];
        far = trans_code_bits[4];
        near = trans_code_bits[5];

        i = i0 + right - left;
        j = j0 + up - down;
        k = k0 + near - far;

        if(trans_code == 11 || trans_code == 27)
            trans_code_step = 5;
        else if(trans_code % 4 == 3)
            trans_code_step = 2;
        else
            trans_code_step = 1;
        trans_code += trans_code_step;
    } while (i < 0 || i >= atoms_count_x ||
             j < 0 || j >= atoms_count_y ||
             k < 0 || k >= atoms_count_z);

    std::list<Atom> cell_list = clLists->getCellListByInd(i, j, k);
//    if(old_trans_code == 0) {
//        cell_list.remove(initial_atom);
//    }

    return cell_list;
}

bool pmols::CLLNeighbourCells::hasNext() {
    return trans_code < 43;
}

void pmols::CLLNeighbourCells::reset() {
    trans_code = 0;

    float cell_length = clLists->GetCellLength();
    i0 = (int)floorf(initial_atom.coord.x/cell_length);
    j0 = (int)floorf(initial_atom.coord.y/cell_length);
    k0 = (int)floorf(initial_atom.coord.z/cell_length);
}


