#include <Molecule.h>

#ifndef COURSEWORK_CELLLINKEDLISTS_H
#define COURSEWORK_CELLLINKEDLISTS_H

#endif //COURSEWORK_CELLLINKEDLISTS_H

class CLLNeighbourAtoms;


/*
template<class T, template<class> class iter_class >
class AtomContainer {
public:
    typedef iter_class<T> atom_iter;
    typedef iter_class<const T> const_atom_iter;

    virtual atom_iter begin() = 0;
    virtual atom_iter end() = 0;
    virtual const_atom_iter begin() const = 0;
    virtual const_atom_iter end() const = 0;
    virtual size_t size() = 0;
};
*/


template <typename T>
class CLLNeighbourAtomsIter;

class CellLinkedLists {
public:
    CellLinkedLists(float cell_length, float len_a, float len_b, float len_c);
    Atom * getAtomByInd(int i, int j, int k);
    CLLNeighbourAtoms getNeighbours(Atom *atom);
    float GetCellLength();
    bool addAtom(Atom *atom);
    bool remAtom(Atom *atom);
    ~CellLinkedLists();

private:
    Atom ****atom_grid;

    int atoms_count_x;
    int atoms_count_y;
    int atoms_count_z;

    float cell_len;
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
    // TODO: iter->counter == 42 => iter->atom_cursor = NULL; end()->atom_cursor <-- NULL
public:
    typedef CLLNeighbourAtomsIter<Atom> atom_iter;
    typedef CLLNeighbourAtomsIter<const Atom> const_atom_iter;

    CLLNeighbourAtoms(CellLinkedLists *cellLinkedLists, Atom *initialAtom) :
            clLists(cellLinkedLists),
            atom(initialAtom), count(0),
            firstIter(cellLinkedLists, initialAtom),
            lastIter(cellLinkedLists, initialAtom) {};
    atom_iter begin() {
//        std::cout << "start calc first iter" << std::endl;
        firstIter.moveToFirst();
//        std::cout << "1st calc finished" << std::endl;
        return firstIter;
    }

    atom_iter end() {
//        std::cout << "start calc last iter" << std::endl;
        lastIter.atom_cursor = NULL;
//        std::cout << "last calc finished" << std::endl;
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
//        std::cout << "try to get size" << std::endl;
        count = 0;
        /*std::cout << "--- begin iter definition ..." << std::endl;
        atom_iter first = begin();
        std::cout << "--- last iter definition ..." << std::endl;
        atom_iter last = end();
        std::cout << "--- begin iter condition ..." << std::endl;
        if(first.atom_cursor!=NULL)
            std::cout << "first atom: " << vec_to_string(first.atom_cursor->coord) << std::endl;
        std::cout << "--- end iter condition ..." << std::endl;
        if(last.atom_cursor!=NULL)
            std::cout << "last atom: " << vec_to_string(last.atom_cursor->coord) << std::endl;

        std::cout << "--- for loop ..." << std::endl;
        std::cout << "first iter: " << first.atom_cursor << " " << first.initial_atom << " " << first.cellLinkedLists << std::endl;
        std::cout << "last iter: " << last.atom_cursor << " " << last.initial_atom << " " << last.cellLinkedLists << std::endl;*/
        for (atom_iter it = begin(); it != end(); ++it) {
//            std::cout <<"--- iteration #" << count << std::endl;
            count++;
        }
//        std::cout << "size has got\n------------------" << std::endl;

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
    char trans_code_bits[6];

//    std::cout << "=== counter:" << int(trans_code) << std::endl;

    while(x > 0) {
        trans_code_bits[bit_counter--] = (char)(x%2);
        x/=2;
    }

    for (int l = bit_counter; l >= 0 ; --l) {
        trans_code_bits[l] = 0;
    }

    char left = trans_code_bits[0];
    char right = trans_code_bits[1];
    char up = trans_code_bits[2];
    char down = trans_code_bits[3];
    char far = trans_code_bits[4];
    char near = trans_code_bits[5];

    /*std::cout << "--->trans code: ";
    for (int m = 0; m < 6; ++m) {
        std::cout << (int)trans_code_bits[m] << " ";
    }
    std::cout << std::endl;*/

    float cell_length = cellLinkedLists->GetCellLength();
//    std::cout << "-----" << (initial_atom == NULL) << std::endl;
    int i0 = (int)floorf(initial_atom->coord.x/cell_length);
    int j0 = (int)floorf(initial_atom->coord.y/cell_length);
    int k0 = (int)floorf(initial_atom->coord.z/cell_length);

    //std::cout << "atom before trans: [" << i0 << ", " << j0 << ", " << k0 << "]" << std::endl;
    int i = i0 + right - left;
    int j = j0 + up - down;
    int k = k0 + near - far;
    //std::cout << "atom after trans: [" << i << ", " << j << ", " << k << "]" << std::endl;

    atom_cursor = cellLinkedLists->getAtomByInd(i, j, k);
}

template<typename T>
CLLNeighbourAtomsIter<T> & CLLNeighbourAtomsIter<T>::operator++() {
    atom_cursor = NULL;
    while(trans_code < 43 && atom_cursor == NULL) {
        moveByCode();
        trans_code++;
        if(trans_code == 11 || trans_code == 27)
            trans_code += 5;
        else if(trans_code % 4 == 3)
            trans_code++;
    }
    if(trans_code == 43) {
        atom_cursor = NULL;
    }
    //std::cout << "current iter cursor: " << atom_cursor << std::endl;

    return *this;
    /*if(atom_cursor != NULL) {
        std::cout << "current atom: " << vec_to_string(atom_cursor->coord) << std::endl;
    }*/
}

template<typename T>
bool CLLNeighbourAtomsIter<T>::operator==(CLLNeighbourAtomsIter const &other) const {
    return cellLinkedLists==other.cellLinkedLists && initial_atom == other.initial_atom && atom_cursor == other.atom_cursor;
}

template<typename T>
bool CLLNeighbourAtomsIter<T>::operator!=(CLLNeighbourAtomsIter const &other) const {
    //std::cout << "current iter: " << atom_cursor << " " << initial_atom << " " << cellLinkedLists << std::endl;
    //std::cout << "other iter: " << other.atom_cursor << " " << other.initial_atom << " " << other.cellLinkedLists << std::endl;
    bool expr = (atom_cursor != other.atom_cursor ||
                cellLinkedLists != other.cellLinkedLists ||
                initial_atom != other.initial_atom);
    //std::cout << "cond result: " << expr << std::endl;
    return expr;
}

template<typename T>
CLLNeighbourAtomsIter<T>::CLLNeighbourAtomsIter(const CLLNeighbourAtomsIter &it) {
    cellLinkedLists = it.cellLinkedLists;
    initial_atom = it.initial_atom;
    atom_cursor = it.atom_cursor;
    trans_code = it.trans_code;
    /*for(int i = 0; i < 6; i++)
        trans_code_bits[i] = it.trans_code_bits[i];*/
}

template<typename T>
typename CLLNeighbourAtomsIter<T>::reference CLLNeighbourAtomsIter<T>::operator*() const {
    return *atom_cursor;
}

template<typename T>
void CLLNeighbourAtomsIter<T>::moveToFirst() {
    //std::cout << "moving to first..." << std::endl;
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


    //std::cout << "first has found" << std::endl;
    //trans_code = 1;

   /* std::cout << "first atom is null: " << (atom_cursor == NULL) << std::endl;
    if(atom_cursor != NULL)
        std::cout << "first atom: " << vec_to_string(atom_cursor->coord) << std::endl;*/
}

template<typename T>
CLLNeighbourAtomsIter<T>::CLLNeighbourAtomsIter(CellLinkedLists *clLists, T *atom) : cellLinkedLists(clLists),
                                                                                     trans_code(1),
                                                                                     atom_cursor(NULL) {
    /*std:: << "---===" << (clLists==NULL) << std::endl;
    std::cout << "---===" << (atom==NULL) << std::endl;*/
    float cell_len = cellLinkedLists->GetCellLength();
//    std::cout << "---===" << cell_len << std::endl;
    int i = atom->coord.x/cell_len;
    int j = atom->coord.y/cell_len;
    int k = atom->coord.z/cell_len;
    initial_atom = clLists->getAtomByInd(i, j, k);
}

template<typename T>
CLLNeighbourAtomsIter<T>::~CLLNeighbourAtomsIter() {
   // delete []trans_code_bits;
}
