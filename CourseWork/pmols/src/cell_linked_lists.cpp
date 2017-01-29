//
// Created by alex on 09.12.16.
//

#include <mol.h>
#include <cell_linked_lists.h>

CellLinkedLists::CellLinkedLists(float cell_length, float len_a, float len_b, float len_c) {
    std::cout << "CLL: initialization" << std::endl;
    cell_len = cell_length;
    
    cell_len_setted = true;
    size_setted = true;

    box_length = len_a;
    box_width = len_b;
    box_height = len_c;

    std::cout << "\tcell_len:" << cell_len << std::endl;

    std::cout << "\tbox_length:" << box_length << std::endl;
    std::cout << "\tbox_width:" << box_width << std::endl;
    std::cout << "\tbox_height:" << box_height << std::endl;


    formCellLinkedLists();
}

CellLinkedLists::~CellLinkedLists() {
  //  if(atom_grid != NULL)
        freeAtomGrid();
}

bool CellLinkedLists::addAtom(Atom &atom) {
    int i = (int)floorf(atom.coord.x/cell_len);
    int j = (int)floorf(atom.coord.y/cell_len);
    int k = (int)floorf(atom.coord.z/cell_len);

    std::cout << "\tadding atom " << atom.toString() << " to cell (" << i << ", " << j << ", " << k << ")" << std::endl;

    if (i < 0 || i >= atoms_count_x ||
        j < 0 || j >= atoms_count_y ||
        k < 0 || k >= atoms_count_z ||
             atom_grid[i][j][k] != NULL)
        return false;
    
    atom_grid[i][j][k] = new Atom(atom);

    return true;
}

bool CellLinkedLists::remAtom(Atom &atom) {
    int i = (int)floorf(atom.coord.x/cell_len);
    int j = (int)floorf(atom.coord.y/cell_len);
    int k = (int)floorf(atom.coord.z/cell_len);

    if (i < 0 || i >= atoms_count_x ||
        j < 0 || j >= atoms_count_y ||
        k < 0 || k >= atoms_count_z ||
            atom_grid[i][j][k] == NULL)
        return false;

    delete atom_grid[i][j][k];
    atom_grid[i][j][k] = NULL;

    return true;
}

float CellLinkedLists::GetCellLength() {
    return cell_len;
}

Atom *CellLinkedLists::getAtomByInd(int i, int j, int k) {
    int n = atoms_count_x;
    int m = atoms_count_y;
    int p = atoms_count_z;

    if (i < 0 || i >= n ||
            j < 0 || j >= m ||
            k < 0 || k >= p)
        return NULL;

    return atom_grid[i][j][k];
}

CLLNeighbourAtoms CellLinkedLists::getNeighbours(Atom *atom) {
    return CLLNeighbourAtoms(this, atom);
}

CellLinkedLists::CellLinkedLists() {
    cell_len_setted = false;
    size_setted = false;
    //atom_grid = NULL;
}

void CellLinkedLists::formCellLinkedLists() {
    atoms_count_x = (int)ceilf(box_length/cell_len);
    atoms_count_y = (int)ceilf(box_width/cell_len);
    atoms_count_z = (int)ceilf(box_height/cell_len);

    /*if(atom_grid != NULL) {
        freeAtomGrid();
    }*/

    atom_grid = new Atom***[atoms_count_x];

    for (int i = 0; i < atoms_count_x; ++i) {
        atom_grid[i] = new Atom**[atoms_count_y];
        for (int j = 0; j < atoms_count_y; ++j) {
            atom_grid[i][j] = new Atom*[atoms_count_z];
        }
    }


    for (int i = 0; i < atoms_count_x; ++i) {
        for (int j = 0; j < atoms_count_y; ++j) {
            for (int k = 0; k < atoms_count_z; ++k) {
                atom_grid[i][j][k] = NULL;
            }
        }
    }
}

void CellLinkedLists::setCellLength(float cell_length) {
    cell_len = cell_length;
    
    cell_len_setted = true;
    
    if (size_setted)
        formCellLinkedLists();
}

void CellLinkedLists::setSize(float len_a, float len_b, float len_c) {
    box_length = len_a;
    box_width = len_b;
    box_height = len_c;
    
    size_setted = true;

    if(cell_len_setted)
        formCellLinkedLists();
}

void CellLinkedLists::freeAtomGrid() {
    for(int i = 0; i < atoms_count_x; i++) {
        for (int j = 0; j < atoms_count_y; ++j) {
            for (int k = 0; k < atoms_count_z; ++k) {
                if(atom_grid[i][j][k] != NULL)
                    delete atom_grid[i][j][k];
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

bool CellLinkedLists::repAtom(Atom &old_atom, Atom &new_atom) {
    return remAtom(old_atom) && addAtom(new_atom);
}

bool CellLinkedLists::addMol(Molecule &mol) {
    std::cout << "CLL: adding molecule ..." << std::endl;
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

bool CellLinkedLists::remMol(Molecule &mol) {
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

bool CellLinkedLists::repMol(Molecule &old_mol, Molecule &new_mol) {
    return remMol(old_mol) && addMol(new_mol);
}

bool CellLinkedLists::moveMol(Molecule &mol, MoveOperation move_op, float val) {
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

float CellLinkedLists::totalNAtomsDist(Molecule &mol) {
    float sum = 0;
    float cur_dist = 0;

    for (int i = 0; i < mol.AtomsCount(); ++i) {
        CLLNeighbourAtoms neighbourAtoms = getNeighbours(&mol.GetAtom(i));
        while(neighbourAtoms.hasNext()) {
            Atom *n_atom = neighbourAtoms.getNext();
            if(n_atom == NULL) {
                cur_dist = 2.0;
            }
            else if(mol.GetAtom(i).parent_mol_id != n_atom->parent_mol_id) {
                cur_dist = std::fabs(glm::distance(mol.GetAtom(i).coord, n_atom->coord)
                       - mol.GetAtom(i).vdw_radius - n_atom->vdw_radius);
            }
            sum += cur_dist;
        }
    }

    return sum;
}

boost::tuple<int, int, int> CellLinkedLists::atomsCountPerAxis() {
    return boost::tuple<int, int, int>(atoms_count_x, atoms_count_y, atoms_count_z);
}

void CellLinkedLists::saveToCSV(std::string file_path) {
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
                Atom *atom = getAtomByInd(i, j, k);
                if(atom != NULL) {
                    std::string atom_info = atom->toString();
                    outf_stream << "\"" << atom_info.c_str() << "\",";
                }
                else {
                    outf_stream << "\"EMPTY\",";
                }
            }
            outf_stream << std::endl;
        }
        outf_stream << std::endl;
    }

    outf_stream.close();
}

CLLNeighbourAtoms::CLLNeighbourAtoms(CellLinkedLists *clLists, Atom *initialAtom) {
    this->clLists = clLists;
    this->initial_atom = initialAtom;
    boost::tuple<int, int, int> atoms_count = clLists->atomsCountPerAxis();

    atoms_count_x = atoms_count.get<0>();
    atoms_count_y = atoms_count.get<1>();
    atoms_count_z = atoms_count.get<2>();
    reset();
}

Atom *CLLNeighbourAtoms::getNext() {
    int x;
    int bit_counter;
    char trans_code_bits[6] = {0};
    char left, right, up, down, far, near;
    int i, j, k;

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

        trans_code++;
        if(trans_code == 11 || trans_code == 27)
            trans_code += 5;
        else if(trans_code % 4 == 3)
            trans_code++;
    } while (i < 0 || i >= atoms_count_x ||
             j < 0 || j >= atoms_count_y ||
             k < 0 || k >= atoms_count_z);

    return clLists->getAtomByInd(i, j, k);
}

bool CLLNeighbourAtoms::hasNext() {
    return trans_code < 43;
}

void CLLNeighbourAtoms::reset() {
    trans_code = 0;

    float cell_length = clLists->GetCellLength();
    i0 = (int)floorf(initial_atom->coord.x/cell_length);
    j0 = (int)floorf(initial_atom->coord.y/cell_length);
    k0 = (int)floorf(initial_atom->coord.z/cell_length);
}


