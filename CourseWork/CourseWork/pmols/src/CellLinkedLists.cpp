//
// Created by alex on 09.12.16.
//

#include <Molecule.h>
#include <CellLinkedLists.h>

CellLinkedLists::CellLinkedLists(float cell_length, float len_a, float len_b, float len_c) {
    cell_len = cell_length;

    box_length = len_a;
    box_width = len_b;
    box_heigth = len_c;

    formCellLinkedLists();
}

CellLinkedLists::~CellLinkedLists() {
    if(atom_grid != NULL)
        freeAtomGrid();
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

CellLinkedLists::CellLinkedLists() {
    cell_len_setted = false;
    size_setted = false;
    atom_grid = NULL;
}

void CellLinkedLists::formCellLinkedLists() {
    atoms_count_x = (int)ceilf(box_length/cell_len);
    atoms_count_y = (int)ceilf(box_width/cell_len);
    atoms_count_z = (int)ceilf(box_heigth/cell_len);

    if(atom_grid != NULL) {
        freeAtomGrid();
    }

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
    box_heigth = len_c;
    size_setted = true;

    if(cell_len_setted)
        formCellLinkedLists();
}

void CellLinkedLists::freeAtomGrid() {
    for (int i = 0; i < atoms_count_y; ++i) {
        for (int j = 0; j < atoms_count_z; ++j) {
            delete []atom_grid[i][j];
        }
        delete []atom_grid[i];
    }

    delete []atom_grid;
}