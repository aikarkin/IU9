//
// Created by alex on 05.12.16.
//

#ifndef COURSEWORK_MOLLATTICE_H
#define COURSEWORK_MOLLATTICE_H

#include <glm/glm.hpp>
#include <MolShell.h>
#include <CellLinkedLists.h>
#include <vector>


class ShellCellLattice {
public:
    ShellCellLattice(IMolShell &initialMolShell, glm::vec3 apposition_point, float a, float b, float c, float cell_length=0);
    ShellCellLattice(IMolShell &initialMolShell, float a, float b, float c, float cell_length=0);
    std::vector<Molecule> &getMolecules();
    IMolShell &getInitialShell();
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

    // params of hjLattice
    std::vector<Molecule> mols;
    glm::vec3 lattice_appos;
    IMolShell& initialShell;
    float cell_shift_i, cell_shift_j, cell_shift_k; // shifts in mathcing directions
    float len_a, len_b, len_c;
    float res_a, res_b, res_c;
    float cell_len;
};

class ClosestPackedLattice {
public:
    ClosestPackedLattice(std::vector<Molecule> &molecules) : mol_proto(molecules[0]) {
        mols = molecules;
        mols_added = true;
    };
    ClosestPackedLattice(Molecule mol_prototype) : mol_proto(mol_prototype) {};
    virtual void setBoxSize(float a, float b, float c) {
        box_a = a;
        box_b = b;
        box_c = c;
    }
    virtual void pack() = 0;
    virtual bool addMolecule() = 0;

    virtual void packMax() {
        if(!mols_added) {
            while (addMolecule()) {
                //std::cout << "go to addMolecule() " << std::endl;
                /*std::cout << "Molecule #" << mols.size() - 1 << std::endl;
                for (int i = 0; i < mols.back().AtomsCount(); ++i) {
                    std::cout << "\t" << "Atom #" << i << ": " << vec_to_string(mols.back().GetAtom(i).coord) << std::endl;
                }
                std::cout << std::endl;*/
            }
        }
        mols_added = true;
        pack();
    };
    std::vector<Molecule> &getMolecules() {
        return mols;
    };
protected:
    std::vector<Molecule> mols;
    Molecule mol_proto;
    float box_a, box_b, box_c;
private:
    bool mols_added;
};

class HJLattice : public ClosestPackedLattice{
public:
    HJLattice(Molecule mol_prototype);
    HJLattice(std::vector<Molecule> &molecules);
    bool addMolecule() override;
    void setPrecision(float tx_eps, float ty_eps, float tz_eps, float x_eps, float y_eps, float z_eps);
    std::vector<float> getLatticeSize();
    void setBoxSize(float a, float b, float c) override;
    void pack() override;
private:
    int n_i, n_j, n_k;
    int cur_i, cur_j, cur_k;
    glm::vec3 vec_i, vec_j, vec_k;
    float len_i, len_j, len_k;

    float tx_e, ty_e, tz_e, x_e, y_e, z_e;

    float cll_cell_len;

    float getMaxAtomRadius();
    void moveMol(Molecule &mol, MoveOperation move_op, float val);
};

#endif //COURSEWORK_MOLLATTICE_H
