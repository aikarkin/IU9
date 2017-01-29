#include <mol.h>

#ifndef COURSEWORK_CELLLINKEDLISTS_H
#define COURSEWORK_CELLLINKEDLISTS_H

#endif //COURSEWORK_CELLLINKEDLISTS_H

class CLLNeighbourAtoms;

enum MoveOperation {ROT_X, ROT_Y, ROT_Z, TRANS_X, TRANS_Y, TRANS_Z};

class CellLinkedLists {

public:
    CellLinkedLists();
    CellLinkedLists(float cell_length, float len_a, float len_b, float len_c);
    void setCellLength(float cell_length);
    void setSize(float len_a, float len_b, float len_c);
    Atom *getAtomByInd(int i, int j, int k);

    float GetCellLength();
    bool addAtom(Atom &atom);
    bool remAtom(Atom &atom);
    bool repAtom(Atom &old_atom, Atom &new_atom);
    CLLNeighbourAtoms getNeighbours(Atom *atom);

    bool addMol(Molecule &mol);
    bool remMol(Molecule &mol);
    bool repMol(Molecule &old_mol, Molecule &new_mol);
    bool moveMol(Molecule &mol, MoveOperation move_op, float val);
    float totalNAtomsDist(Molecule &mol);
    boost::tuple<int, int, int> atomsCountPerAxis();

    void saveToCSV(std::string file_path);

    ~CellLinkedLists();

private:
    void formCellLinkedLists();
    void freeAtomGrid();

    Atom ****atom_grid;

    bool size_setted;
    bool cell_len_setted;

    int atoms_count_x;
    int atoms_count_y;
    int atoms_count_z;

    float cell_len;

    float box_width, box_length, box_height;
};


class CLLNeighbourAtoms {
public:
    CLLNeighbourAtoms(CellLinkedLists*, Atom*);
    Atom *getNext();
    bool hasNext();
    void reset();
private:
    CellLinkedLists *clLists;
    Atom *initial_atom;
    int trans_code;
    int i0, j0, k0;
    int atoms_count_x, atoms_count_y, atoms_count_z;
};
