#include <mol.h>
#include <functional>
#include <memory>

#ifndef COURSEWORK_CELLLINKEDLISTS_H
#define COURSEWORK_CELLLINKEDLISTS_H

#endif //COURSEWORK_CELLLINKEDLISTS_H

namespace pmols {
    class CLLNeighbourCells;

    enum MOVE_OP {
        TRANS_X,
        TRANS_Y,
        TRANS_Z,
        ROT_X,
        ROT_Y,
        ROT_Z,
    };

    typedef std::function<float(pmols::Atom*, pmols::Atom*)> DistFunc;

    class CellLinkedLists {
        friend class CLLNeighbourCells;
    public:
        CellLinkedLists() {};
        CellLinkedLists(std::vector<Molecule> &mols, float cellLen, DistFunc distFunc);
        CellLinkedLists(float cellLen, DistFunc distFunc);
        void Set(float cellLen, DistFunc distFunc);
        void FormCellLinkedLists();

        Molecule &GetMol(int molIdx);
        bool AddMol(Molecule &mol);
        bool MoveMol(int molIdx, MOVE_OP moveOp, float val);
        void CancelMove();
        float MolDist(int molIdx);
        void SaveToCSV(std::string file_path);

        size_t MolsCount();
        ~CellLinkedLists();
    private:
        void calcBoundingBox();
        bool molInsideBox(Molecule& mol);
        void repMol(int molIdx, Molecule &mol);
        std::tuple<int, int, int> getCellIndex(Atom &atom);

        int atoms_count_x, atoms_count_y, atoms_count_z;

        glm::vec3 appos_point;
        float box_length, box_width, box_height;

        float cell_len;
        DistFunc dist_func;

        bool formed;
        bool mol_moved;

        std::vector<Molecule> mols;
        std::list<Atom> ***atom_grid;
        std::shared_ptr<std::tuple<int, Molecule>> last_mol;
    };


    class CLLNeighbourCells {
    public:
        CLLNeighbourCells(CellLinkedLists *cellLinkedLists, Atom &atom);
        std::list<Atom> next();
        bool hasNext();
        void reset();

    private:
        CellLinkedLists *clLists;
        Atom initial_atom;

        int trans_code;
        int i0, j0, k0;
    };
}