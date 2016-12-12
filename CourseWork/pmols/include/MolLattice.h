//
// Created by alex on 05.12.16.
//

#ifndef COURSEWORK_MOLLATTICE_H
#define COURSEWORK_MOLLATTICE_H

#include <glm/glm.hpp>
#include <MolShell.h>
#include <vector>


class ClosestPackedLattice {
public:
    ClosestPackedLattice(Molecule mol_prototype) : mol_proto(mol_prototype) {};
    virtual void pack() = 0;
    virtual bool addMolecule() = 0;

    void packMax() {
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
};


class AnnealingSimulationLattice : public ClosestPackedLattice {
public:
    AnnealingSimulationLattice(Molecule mol_prototype);
    void pack();
    bool addMolecule();
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
