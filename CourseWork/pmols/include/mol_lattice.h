//
// Created by alex on 05.12.16.
//

#ifndef COURSEWORK_MOLLATTICE_H
#define COURSEWORK_MOLLATTICE_H

#include <glm/glm.hpp>
#include <mol_shell.h>
#include <cell_linked_lists.h>
#include <vector>
#include <pack_optimization.h>


class ClosestPackedLattice {
public:
    ClosestPackedLattice(std::vector<Molecule> &molecules) : mol_proto(molecules[0]) {
        mols = molecules;
    };
    ClosestPackedLattice(Molecule mol_prototype) : mol_proto(mol_prototype) {};
    virtual void setBoxSize(float a, float b, float c) {
        length = a;
        width = b;
        height = c;
        formLattice();
    }

    virtual void pack() = 0;

    void saveToFile(std::string output_file, std::string output_format);
    std::vector<Molecule> &getMolecules() {
        return mols;
    };
protected:
    std::vector<Molecule> mols;
    Molecule mol_proto;
    float length, width, height;
    virtual void formLattice();
};

class HJLattice : public ClosestPackedLattice{
public:
    HJLattice(Molecule mol_prototype) : ClosestPackedLattice(mol_prototype) {};
    HJLattice(std::vector<Molecule> &molecules) : ClosestPackedLattice(molecules) {};
    void setPrecision(float tx_eps, float ty_eps, float tz_eps, float x_eps, float y_eps, float z_eps);
    void setInitialDisplacement(float, float, float, float, float, float);
    void pack() override;
private:
    float shift_x, shift_y, shift_z;
    float shift_alpha, shift_beta, shift_gamma;
    float tx_e, ty_e, tz_e, x_e, y_e, z_e;

    ublas::vector<float> points;
    std::vector<float> displacements;
    std::vector<float> precisions;

    float cll_cell_len;

    float getMaxAtomRadius();
    void moveMol(Molecule &mol, MoveOperation move_op, float val);
    void prepareOptimizationParams();
};

#endif //COURSEWORK_MOLLATTICE_H
