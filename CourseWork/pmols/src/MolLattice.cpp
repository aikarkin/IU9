//
// Created by alex on 05.12.16.
//

#include <bits/shared_ptr.h>
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

void HookeJeevesLattice::pack() {
    if (!calcOptimizationFunc()) {
        std::cerr << "Error: intolerable coordinates of molecules" << std::endl;
        return;
    };

    while(exploringSearch()) {
        patternSearch();
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "IncompatibleTypes"
bool HookeJeevesLattice::addMolecule() {
    if(cur_k >= n_k) {
        cur_k = 0;
        cur_j++;
    }
    if(cur_j >= n_j) {
        cur_j = 0;
        cur_i++;
    }
    if(cur_i >= n_i) {
        return false;
    }

    Molecule mol(mol_proto);
    glm::vec3 trans_vec((float)cur_i*vec_i + (float)cur_j*vec_j + (float)cur_k*vec_k);
    mol.translate(trans_vec);

    mols.push_back(mol);

    molInd_iter.emplace_back(90, 90, 90, glm::length(vec_i) / 2.f, glm::length(vec_j) / 2.f, glm::length(vec_k) / 2.f);
    molInd_iter.back().mol_dist_sum = calcDistSumOfMol(mol);

    for (int i = 0; i < mol.AtomsCount(); ++i) {
        clLists.addAtom(&mol.GetAtom(i));
    }

    cur_k++;
    return true;
}
#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma ide diagnostic ignored "IncompatibleTypes"
HookeJeevesLattice::HookeJeevesLattice(Molecule mol_prototype) :
        ClosestPackedLattice(mol_prototype), clLists() {
    MolCubeShell cubeShell(&mol_proto);

    MolCubeShell::compareCubeEdgeLes compareFunc = [](float len, float len_opt) {
        return len > len_opt;
    };
    cubeShell.pack(std::bind(compareFunc, std::placeholders::_1, std::placeholders::_2));
    mols.push_back(mol_proto);

    std::vector<glm::vec3> disp_vecs(cubeShell.getDisplacementVectors());

    float max_radius = 0;
    float cur_radius;

    for (int i = 0; i < mol_proto.AtomsCount(); ++i) {
        cur_radius = mol_proto.GetAtom(i).vdw_radius;
        if (cur_radius > max_radius)
            max_radius = cur_radius;
    }

    clLists.setCellLength(2*max_radius);

    vec_i = disp_vecs[0];
    vec_j = disp_vecs[1];
    vec_k = disp_vecs[2];

    full_dist_sum = 0;

    cur_i = 0;
    cur_j = 0;
    cur_k = 0;
}
#pragma clang diagnostic pop

void HookeJeevesLattice::packMax() {
    while(addMolecule()) {}
    pack();
}

void HookeJeevesLattice::setPrecision(float teta_x_prec, float teta_y_prec, float teta_z_prec, float x_prec, float y_prec,
                                 float z_prec) {
    tx_e = teta_x_prec;
    ty_e = teta_y_prec;
    tz_e = teta_z_prec;

    x_e = x_prec;
    y_e = y_prec;
    z_e = z_prec;
}

void HookeJeevesLattice::setBoxSize(float a, float b, float c) {
    ClosestPackedLattice::setBoxSize(a, b, c);

    float cube_of_radii_sum = 0;

    for (int i = 0; i < mol_proto.AtomsCount(); ++i) {
        cube_of_radii_sum += std::pow(mol_proto.GetAtom(i).vdw_radius, 3);
    }

    float n0 = 0.176776f * (1 / cube_of_radii_sum);

    n_i = (int)(n0*box_a);
    n_j = (int)(n0*box_b);
    n_k = (int)(n0*box_c);

    float cll_a = (int)((float)n_i*glm::length(vec_i)) + 2;
    float cll_b = (int)((float)n_j*glm::length(vec_j)) + 2;
    float cll_c = (int)((float)n_k*glm::length(vec_k)) + 2;

    clLists.setSize(cll_a, cll_b, cll_c);
}

bool HookeJeevesLattice::calcOptimizationFunc() {
    float cur_sum;
    full_dist_sum = 0;
    for (int i = 0; i < mols.size(); ++i) {
        cur_sum = calcDistSumOfMol(mols[i]);
        if(cur_sum < 0) {
            return false;
        }
        full_dist_sum += cur_sum;
    }
    return true;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "IncompatibleTypes"
float HookeJeevesLattice::calcDistSumOfMol(Molecule &mol) {
    float res_sum = 0;
    float cur_dist = 0;

    Atom *cur_atom;
    for (int i = 0; i < mol.AtomsCount(); ++i) {
        cur_atom = &mol.GetAtom(i);
        for(auto a : clLists.getNeighbours(cur_atom)) {
            cur_dist = glm::distance(cur_atom->coord, a.coord) - cur_atom->vdw_radius - a.vdw_radius;
            if (cur_dist < 0)
                return -1;
            else
                res_sum += cur_dist;
        }
    }

    return res_sum;
}
#pragma clang diagnostic pop

bool HookeJeevesLattice::exploringSearch() {
    bool finished = true;

    for (int i = 0; i < molInd_iter.size(); ++i) {
        if (!chooseOptimalState(molInd_iter[i])) {
            molInd_iter[i].h_tx /= 2;
            molInd_iter[i].h_ty /= 2;
            molInd_iter[i].h_tz /= 2;

            molInd_iter[i].h_x /= 2;
            molInd_iter[i].h_y /= 2;
            molInd_iter[i].h_z /= 2;

            if(finished && (molInd_iter[i].h_tx > tx_e ||
                    molInd_iter[i].h_ty > ty_e ||
                    molInd_iter[i].h_tz > tz_e ||
                    molInd_iter[i].h_x > x_e ||
                    molInd_iter[i].h_y > y_e ||
                    molInd_iter[i].h_z > z_e))
                finished = false;
        }
    }

    return finished;
}

void HookeJeevesLattice::patternSearch() {

}

void HookeJeevesLattice::removeExcessMoleules() {

}

bool HookeJeevesLattice::chooseOptimalState(HookeJeevesIteration &iteration) {
    `
    return false;
}
