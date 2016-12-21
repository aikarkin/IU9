#include <Molecule.h>

#ifndef COURSEWORK_CELLLINKEDLISTS_H
#define COURSEWORK_CELLLINKEDLISTS_H

#endif //COURSEWORK_CELLLINKEDLISTS_H

class CLLNeighbourAtoms;

enum MoveOperation {ROT_X, ROT_Y, ROT_Z, TRANS_X, TRANS_Y, TRANS_Z};

template <typename T>
class CLLNeighbourAtomsIter;

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


template<typename T>
class CLLNeighbourAtomsIter: public std::iterator<std::input_iterator_tag, T> {
    friend class CLLNeighbourAtoms;
private:
    CellLinkedLists *cellLinkedLists;
    T *initial_atom;
    T *atom_cursor;
    char trans_code;

    void moveByCode();
    void moveToFirst();

    CLLNeighbourAtomsIter(CellLinkedLists *clLists, T* atom);
public:
    ~CLLNeighbourAtomsIter();
    CLLNeighbourAtomsIter(const CLLNeighbourAtomsIter &it);

    bool operator!=(CLLNeighbourAtomsIter const& other) const;
    bool operator==(CLLNeighbourAtomsIter const& other) const;
    typename CLLNeighbourAtomsIter::reference operator*() const;
    CLLNeighbourAtomsIter &operator++();
};


class CLLNeighbourAtoms {
public:
    typedef CLLNeighbourAtomsIter<Atom> atom_iter;
    typedef CLLNeighbourAtomsIter<const Atom> const_atom_iter;

    CLLNeighbourAtoms(CellLinkedLists *cellLinkedLists, Atom *initialAtom) :
            clLists(cellLinkedLists),
            atom(initialAtom), count(0),
            firstIter(cellLinkedLists, initialAtom),
            lastIter(cellLinkedLists, initialAtom) {};
    atom_iter begin() {
        firstIter.moveToFirst();    
        return firstIter;
    }

    atom_iter end() {
        lastIter.atom_cursor = NULL;    
        return lastIter;
    }

    const_atom_iter begin() const {
        const_atom_iter firstIter(clLists, atom);
        firstIter.moveToFirst();
        return firstIter;
    }
    const_atom_iter end() const {
        const_atom_iter lastIter(clLists, atom);
        lastIter.atom_cursor = NULL;
        return lastIter;
    }

    size_t size() {
        count = 0;
        for (atom_iter it = begin(); it != end(); ++it) {
            count++;
        }

        return count;
    }
private:
    atom_iter firstIter;
    atom_iter lastIter;

    CellLinkedLists *clLists;
    Atom *atom;
    size_t count;
};


template<typename T>
void CLLNeighbourAtomsIter<T>::moveByCode() {
    int x = trans_code;
    int bit_counter = 5;
    char trans_code_bits[6] = {0};

    while(x > 0) {
        trans_code_bits[bit_counter--] = (char)(x%2);
        x/=2;
    }

    for (int l = bit_counter; l >= 0 ; --l) {
        trans_code_bits[l] = 0;
    }

    /*std::cout << "--->trans code: ";
    for (int m = 0; m < 6; ++m) {
        std::cout << (int)trans_code_bits[m] << " ";
    }
    std::cout << std::endl;*/

    char left = trans_code_bits[0];
    char right = trans_code_bits[1];
    char up = trans_code_bits[2];
    char down = trans_code_bits[3];
    char far = trans_code_bits[4];
    char near = trans_code_bits[5];

    float cell_length = cellLinkedLists->GetCellLength();
    
    int i0 = (int)floorf(initial_atom->coord.x/cell_length);
    int j0 = (int)floorf(initial_atom->coord.y/cell_length);
    int k0 = (int)floorf(initial_atom->coord.z/cell_length);

    int i = i0 + right - left;
    int j = j0 + up - down;
    int k = k0 + near - far;

    atom_cursor = cellLinkedLists->getAtomByInd(i, j, k);
}

template<typename T>
CLLNeighbourAtomsIter<T> & CLLNeighbourAtomsIter<T>::operator++() {
//    std::cout << "CLLIter.operator++()" << std::endl;
//    std::cout << "atom cursor before ++: " << atom_cursor << std::endl;
    atom_cursor = NULL;
    while(trans_code < 43 && atom_cursor == NULL) {
        moveByCode();
        trans_code++;
        if(trans_code == 11 || trans_code == 27)
            trans_code += 5;
        else if(trans_code % 4 == 3)
            trans_code++;
    }
    /*std::cout << "--> trans code: " << (int)trans_code << std::endl;
    if(atom_cursor != NULL)
        std::cout << "--> atom_cursor coord: " << vec_to_string(atom_cursor->coord) << std::endl;*/
    if(trans_code >= 43) {
        atom_cursor = NULL;
    }
//    std::cout << "a_cursor after ++: " << atom_cursor << std::endl;

    return *this;
}

template<typename T>
bool CLLNeighbourAtomsIter<T>::operator==(CLLNeighbourAtomsIter const &other) const {
    return cellLinkedLists==other.cellLinkedLists && initial_atom == other.initial_atom && atom_cursor == other.atom_cursor;
}

template<typename T>
bool CLLNeighbourAtomsIter<T>::operator!=(CLLNeighbourAtomsIter const &other) const {
//    std::cout << "not equal" << std::endl;
    bool expr = (atom_cursor != other.atom_cursor ||
                cellLinkedLists != other.cellLinkedLists ||
                initial_atom != other.initial_atom);
//    std::cout << "expr_res : " << (int)expr << std::endl;
    return expr;
}

template<typename T>
CLLNeighbourAtomsIter<T>::CLLNeighbourAtomsIter(const CLLNeighbourAtomsIter &it) {
    cellLinkedLists = it.cellLinkedLists;
    initial_atom = it.initial_atom;
    atom_cursor = it.atom_cursor;
    trans_code = it.trans_code;
}

template<typename T>
typename CLLNeighbourAtomsIter<T>::reference CLLNeighbourAtomsIter<T>::operator*() const {
    return *atom_cursor;
}

template<typename T>
void CLLNeighbourAtomsIter<T>::moveToFirst() {
    atom_cursor = NULL;
    if(initial_atom == NULL) {
        return;
    }

    atom_cursor = NULL;
    trans_code = 1;

    while(trans_code < 43 && atom_cursor == NULL) {
        moveByCode();
        trans_code++;
        if(trans_code == 27 || trans_code == 11)
            trans_code += 5;
        else if(trans_code % 4 == 3)
            trans_code++;
    }

    if(trans_code == 43) {
        atom_cursor = NULL;
        return;
    }
}

template<typename T>
CLLNeighbourAtomsIter<T>::CLLNeighbourAtomsIter(CellLinkedLists *clLists, T *atom) : cellLinkedLists(clLists),
                                                                                     trans_code(1),
                                                                                     atom_cursor(NULL) {
    float cell_len = cellLinkedLists->GetCellLength();
    
    int i = atom->coord.x/cell_len;
    int j = atom->coord.y/cell_len;
    int k = atom->coord.z/cell_len;
    initial_atom = clLists->getAtomByInd(i, j, k);
}

template<typename T>
CLLNeighbourAtomsIter<T>::~CLLNeighbourAtomsIter() {
}
