//
// Created by alex on 05.12.16.
//

#ifndef COURSEWORK_MOLLATTICE_H
#define COURSEWORK_MOLLATTICE_H

#include <glm/glm.hpp>
#include <MolShell.h>
#include <CellLinkedLists.h>
#include <vector>


class ClosestPackedLattice {
public:
    ClosestPackedLattice(Molecule mol_prototype) : mol_proto(mol_prototype) {};
    virtual void setBoxSize(float a, float b, float c) {
        box_a = a;
        box_b = b;
        box_c = c;
    }
    virtual void pack() = 0;
    virtual bool addMolecule() = 0;

    virtual void packMax() {
        while(addMolecule()) {
            pack();
        }
    };
    std::vector<Molecule> &getMolecules() {
        return mols;
    };
protected:
    std::vector<Molecule> mols;
    Molecule mol_proto;
    float box_a, box_b, box_c;
};

struct HookeJeevesIteration {
    float h_tx_, h_ty_, h_tz_, h_x_, h_y_, h_z_;
    float mol_dist_sum = 0;

    HookeJeevesIteration(float h_tx, float h_ty, float h_tz, float h_x, float h_y, float h_z) {
        h_tx_ = h_tx;
        h_ty_ = h_ty;
        h_tz_ = h_tz;

        h_x_ = h_x;
        h_y_ = h_y;
        h_z_ = h_z;
    }
};

class HookeJeevesLattice : public ClosestPackedLattice {
public:
    HookeJeevesLattice(Molecule mol_prototype);
    bool addMolecule();
    void pack();
    void packMax();
    void setBoxSize(float a, float b, float c);
    void setPrecision(float teta_x_prec, float teta_y_prec, float teta_z_prec, float x_prec, float y_prec, float z_prec);
private:
    float tx_e, ty_e, tz_e, x_e, y_e, z_e;
    glm::vec3 vec_i, vec_j, vec_k;
    int n_i, n_j, n_k;
    int cur_i, cur_j, cur_k;
    float full_dist_sum;
    std::vector<Molecule> mols;
    std::vector<HookeJeevesIteration> molInd_iter;
    CellLinkedLists clLists;


    bool calcOptimizationFunc();
    float calcDistSumOfMol(Molecule &mol);
    bool exploringSearch();
    void patternSearch();
    void removeExcessMoleules();
};


class ShellCellLattice {
public:
    ShellCellLattice(IMolShell &initialMolShell, glm::vec3 apposition_point, float a, float b, float c, float cell_length=0);
    ShellCellLattice(IMolShell &initialMolShell, float a, float b, float c, float cell_length=0);
    std::vector<Molecule> &getMolecules();
    std::vector<float> getLatticeSize();
private:
    void initLattice();
    void initMolShellParams();
    void formLattice();

    // params of packing shell
    std::vector<glm::vec3> shellPoints;
    glm::vec3 vec_i, vec_j, vec_k; // shell displacement direction vectors (normalized)
    float len_i, len_j, len_k; // lengths of matching vectors
    glm::vec3 shell_appos;

    // params of lattice
    std::vector<Molecule> mols;
    glm::vec3 lattice_appos;
    IMolShell& initialShell;
    float cell_shift_i, cell_shift_j, cell_shift_k; // shifts in mathcing directions
    float len_a, len_b, len_c;
    float res_a, res_b, res_c;
    float cell_len;
};


#endif //COURSEWORK_MOLLATTICE_H
