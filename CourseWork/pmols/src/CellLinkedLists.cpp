//
// Created by alex on 09.12.16.
//

#include <Molecule.h>
#include <CellLinkedLists.h>

CellLinkedLists::CellLinkedLists(float cell_length, float len_a, float len_b, float len_c) {
    cell_len = cell_length;
    atoms_count_x = (int)ceilf(len_a/cell_len);
    atoms_count_y = (int)ceilf(len_b/cell_len);
    atoms_count_z = (int)ceilf(len_c/cell_len);

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

CellLinkedLists::~CellLinkedLists() {
    for (int i = 0; i < atoms_count_y; ++i) {
        for (int j = 0; j < atoms_count_z; ++j) {
            delete []atom_grid[i][j];
        }
        delete []atom_grid[i];
    }

    delete []atom_grid;
}

bool CellLinkedLists::addAtom(Atom *atom) {
    int i = (int)floorf(atom->coord.x/cell_len);
    int j = (int)floorf(atom->coord.y/cell_len);
    int k = (int)floorf(atom->coord.z/cell_len);

    if (i < 0 || i >= atoms_count_x ||
        j < 0 || j >= atoms_count_y ||
        k < 0 || k >= atoms_count_z ||
             atom_grid[i][j][k] != NULL)
        return false;

    atom_grid[i][j][k] = atom;

    return true;
}

bool CellLinkedLists::remAtom(Atom *atom) {
    int i = (int)floorf(atom->coord.x/cell_len);
    int j = (int)floorf(atom->coord.y/cell_len);
    int k = (int)floorf(atom->coord.z/cell_len);

    if (i < 0 || i >= atoms_count_x ||
        j < 0 || j >= atoms_count_y ||
        k < 0 || k >= atoms_count_z ||
            atom_grid[i][j][k] == NULL)
        return false;

    atom_grid[i][j][k] = NULL;

    return true;
}

float CellLinkedLists::GetCellLength() {
    return cell_len;
}

Atom * CellLinkedLists::getAtomByInd(int i, int j, int k) {
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