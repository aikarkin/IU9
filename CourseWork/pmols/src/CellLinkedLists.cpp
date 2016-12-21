//
// Created by alex on 09.12.16.
//

#include <Molecule.h>
#include <CellLinkedLists.h>

CellLinkedLists::CellLinkedLists(float cell_length, float len_a, float len_b, float len_c) {

    cell_len = cell_length;
    
    cell_len_setted = true;
    size_setted = true;

    box_length = len_a;
    box_width = len_b;
    box_height = len_c;
    
//    atom_grid = NULL;

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
            mol.rotateX(val);
            break;
        case ROT_Y:
            mol.rotateY(val);
            break;
        case ROT_Z:
            mol.rotateZ(val);
            break;
        case TRANS_X:
            mol.translate(val*vec_i);
            break;
        case TRANS_Y:
            mol.translate(val*vec_j);
            break;
        case TRANS_Z:
            mol.translate(val*vec_k);
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
        for(auto n_atom : getNeighbours(&mol.GetAtom(i))) {
            if(mol.GetAtom(i).parent_mol_id != n_atom.parent_mol_id) {
                cur_dist = glm::distance(mol.GetAtom(i).coord, n_atom.coord)
                       - mol.GetAtom(i).vdw_radius - n_atom.vdw_radius;
                if(cur_dist < 0)
                    return -1;
                sum += cur_dist;
            }
        }
    }

    return sum;
}

