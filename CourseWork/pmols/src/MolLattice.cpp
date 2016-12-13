//
// Created by alex on 05.12.16.
//

#include "MolLattice.h"

bool isOrthogonal(glm::vec3 vec1, glm::vec3 vec2) {
    return std::abs(glm::angle(vec1, vec2) - (float)M_PI_2) < 0.0001f;
};

ShellCellLattice::ShellCellLattice(IMolShell &initialMolShell, glm::vec3 apposition_point, float a, float b, float c, float cell_length)
        : initialShell(initialMolShell), cell_len(cell_length),
          lattice_appos(apposition_point),
          len_a(a), len_b(b), len_c(c) {
    initLattice();
}

ShellCellLattice::ShellCellLattice(IMolShell &initialMolShell, float a, float b, float c, float cell_length)
        : initialShell(initialMolShell), cell_len(cell_length), len_a(a), len_b(b), len_c(c) {
    lattice_appos = glm::vec3(-a/2, -b/2, -c/2);
    initLattice();
}

std::vector<Molecule> &ShellCellLattice::getMolecules() {
    return mols;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "IncompatibleTypes"
void ShellCellLattice::initMolShellParams() {
    shellPoints = initialShell.getShellPoints();
    std::vector<glm::vec3> dispVecs(initialShell.getDisplacementVectors());

    vec_i = dispVecs[2];
    vec_j = dispVecs[0];
    vec_k = dispVecs[1];

    len_i = glm::length(vec_i);
    len_j = glm::length(vec_j);
    len_k = glm::length(vec_k);

    if(cell_len == 0) {
        cell_shift_i = len_i;
        cell_shift_j = len_j;
        cell_shift_k = len_k;
    }
    else {
        cell_shift_i = cell_len;
        cell_shift_j = cell_len;
        cell_shift_k = cell_len;
    }

    vec_i = glm::normalize(vec_i);
    vec_j = glm::normalize(vec_j);
    vec_k = glm::normalize(vec_k);

    shell_appos = initialShell.getAppositionPoint();
}
#pragma clang diagnostic pop

void ShellCellLattice::formLattice() {
    // move apposition point of shell in right place inside initial lattice shell
    glm::vec3 P = (cell_shift_i - len_i)/2*vec_i + (cell_shift_j - len_j)/2*vec_j + (cell_shift_k - len_k)*vec_k;
    glm::vec3 vec_app_shift = (P - shell_appos);
    initialShell.translate(-vec_app_shift);

    // lattice generation
    int n = (int)ceilf(len_a/cell_shift_i); // amount of lattice cells in vec_i direction
    int m = (int)ceilf(len_b/cell_shift_j); // amount of lattice cells in vec_j direction
    int l = (int)ceilf(len_c/cell_shift_k); // amount of lattice cells in vec_k direction

    res_a = n*cell_shift_i;
    res_b = m*cell_shift_j;
    res_c = l*cell_shift_k;
    glm::vec3 vec_disp;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            for (int k = 0; k < l; ++k) {
                vec_disp = glm::vec3(i*cell_shift_i*vec_i + j*cell_shift_j*vec_j + k*cell_shift_k*vec_k);
                Molecule mol(initialShell.getMolecule());
                mol.translate(vec_disp);
                mols.push_back(mol);
            }
        }
    }
}

void ShellCellLattice::initLattice() {
    initialShell.pack();
    initMolShellParams();

    if (cell_len !=0 && std::max(len_i, std::max(len_j, len_k)) >= cell_len) {
        std::cout << "Result: failed" << std::endl;
        std::cerr << "Error: unable to generate lattice with current configuration. Try to change single cell length." << std::endl;
        return;
    }

    formLattice();
}

std::vector<float> ShellCellLattice::getLatticeSize() {
    std::vector<float> res;

    res.push_back(res_a);
    res.push_back(res_b);
    res.push_back(res_c);

    return res;
}


void AnnealingSimulationLattice::pack() {

}

bool AnnealingSimulationLattice::addMolecule() {

}

AnnealingSimulationLattice::AnnealingSimulationLattice(Molecule mol_prototype) : ClosestPackedLattice(mol_prototype) {
}
