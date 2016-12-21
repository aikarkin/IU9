//
// Created by alex on 05.12.16.
//

#include "MolLattice.h"
#include <OptimizationAlgos.h>

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

void ShellCellLattice::initMolShellParams() {
    shellPoints = initialShell.getShellPoints();
    std::vector<glm::vec3> dispVecs(initialShell.getDisplacementVectors());

    vec_i = dispVecs[2];
    vec_j = dispVecs[0];
    vec_k = dispVecs[1];

    len_i = (float)glm::length(vec_i);
    len_j = (float)glm::length(vec_j);
    len_k = (float)glm::length(vec_k);

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

HJLattice::HJLattice(Molecule mol_prototype) : ClosestPackedLattice(mol_prototype) {
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

    std::cout << "CLL construction" << std::endl;
    //clLists = CellLinkedLists();
    clLists.setCellLength(2*max_radius);


    vec_i = disp_vecs[0];
    vec_j = disp_vecs[1];
    vec_k = disp_vecs[2];

    // full_dist_sum = 0;

    cur_i = 0;
    cur_j = 0;
    cur_k = 0;

}

void HJLattice::setBoxSize(float a, float b, float c) {
    ClosestPackedLattice::setBoxSize(a, b, c);

//    float cube_of_radii_sum = 0;
//
//    for (int i = 0; i < mol_proto.AtomsCount(); ++i) {
//        cube_of_radii_sum += std::pow(mol_proto.GetAtom(i).vdw_radius, 3);
//    }
//
//    float n0 = 0.176776f * (1 / cube_of_radii_sum);
//
//    std::cout << "n0: " << n0 << std::endl;
//    std::cout << "box_a: " << box_a << "box_b: " << box_b << "box_c: " << box_c << std::endl;

    n_i = (int)(2*box_a/clLists.GetCellLength());
    n_j = (int)(2*box_b/clLists.GetCellLength());
    n_k = (int)(2*box_c/clLists.GetCellLength());



    std::cout << "box size set! " << std::endl;
    std::cout << "n_i: " << n_i << "; n_j: " << n_j << "; n_k: " << n_k << std::endl;

    float cll_a = n_i*(float)glm::length(vec_i) + 2.f;
    float cll_b = n_j*(float)glm::length(vec_j) + 2.f;
    float cll_c = n_k*(float)glm::length(vec_k) + 2.f;

    clLists.setSize(cll_a, cll_b, cll_c);
}

void HJLattice::setPrecision(float tx_eps, float ty_eps, float tz_eps, float x_eps, float y_eps, float z_eps) {
    tx_e = tx_eps;
    ty_e = ty_eps;
    tz_e = tz_eps;

    x_e = x_eps;
    y_e = y_eps;
    z_e = z_eps;
}

bool HJLattice::addMolecule() {
    /*std::cout << "adding molecule" << std::endl;
    std::cout << "cur_i: " << cur_i << "; cur_j: " << cur_j << "; cur_k: " << cur_k << std::endl;
    std::cout << "n_i: " << n_i << "; n_j: " << n_j << "; n_k: " << n_k << std::endl;
*/
    if(cur_k >= n_k) {
        cur_k = 0;
        cur_j++;
    }
    if(cur_j >= n_j) {
        cur_j = 0;
        cur_i++;
    }
    if(cur_i >= n_i) {
        std::cout << "finished!!" << std::endl;
        return false;
    }

    Molecule mol(mol_proto);
    glm::vec3 trans_vec((float)cur_i*vec_i + (float)cur_j*vec_j + (float)cur_k*vec_k);
//    std::cout << "adding new mol to " << vec_to_string(trans_vec) << std::endl;
    mol.translate(trans_vec);
    mols.push_back(mol);

    for (int i = 0; i < mol.AtomsCount(); ++i) {
        clLists.addAtom(&mol.GetAtom(i));
    }

    cur_k++;
    return true;
}

void HJLattice::pack() {
    ublas::vector<float> point(mols.size()*6);
    std::vector<float> displacement;
    std::vector<float> epsilon(point.size());

    float sum_dist = 0;

    std::cout << "calc initial sum" << std::endl;
    std::cout << "mols size: " << mols.size() << std::endl;
    for (int i = 0; i < mols.size(); ++i) {
        sum_dist += calcMolSum(i);
    }
    std::cout << "initial sum: " << sum_dist << std::endl;

    for (int i = 0, j = 0; i < point.size(); ++i) {
        point[i] = 0;
        j = i%6;

        if(j < 3)
            displacement.push_back(45.f);
        else
            displacement.push_back(0.5f);

        switch(j) {
            case 0:
                epsilon[i] = tx_e;
                break;
            case 1:
                epsilon[i] = ty_e;
                break;
            case 2:
                epsilon[i] = tz_e;
                break;
            case 3:
                epsilon[i] = x_e;
                break;
            case 4:
                epsilon[i] = y_e;
            case 5:
                epsilon[i] = z_e;
                break;
            default:
                break;
        }
    }

    //std::cout << "sum_dist: " << sum_dist << std::endl;
    OFuncCallback opt_func = [sum_dist, this](ublas::vector<float>& p, int coord_ind) -> float {
        //std::cout << "!!! in optimization func" << std::endl;
        //std::cout << "sum_dist1: " << sum_dist << std::endl;
        int i = coord_ind / 6; // mol index
        int op_num = coord_ind % 6;
        float coord_val = p[coord_ind];

        float old_mol_sum = calcMolSum(i);
        std::cout << "old mol sum: " << old_mol_sum << std::endl;

        moveByCode(op_num, i, coord_val);

        updateAtomIndexes(i);

        float new_mol_sum = calcMolSum(i);
        std::cout << "new mol sum: " << new_mol_sum << std::endl;

        //moveByCode(op_num, i, -coord_val);

        if(new_mol_sum < 0.f) {
            return -1.f;
        }

        return sum_dist - old_mol_sum + new_mol_sum;
    };

    HookeJeevesOptimize(point, displacement, epsilon, opt_func);
}

float HJLattice::calcMolSum(int mol_ind) {
    float mol_sum = 0;
    float cur_dist = 0;

    Atom *cur_atom;

    //std::cout << "calc sum of mol with barycenter: " << vec_to_string(mols[mol_ind].GetBarycenter()) << std::endl;
    for (int j = 0; j < mols[mol_ind].AtomsCount(); ++j) {
        //std::cout << "--> atom coordinates: " << vec_to_string(mols[mol_ind].GetAtom(j).coord) << std::endl;
        cur_atom = &mols[mol_ind].GetAtom(j);
        //std::cout << "cur atom coord: " << vec_to_string(cur_atom->coord) << std::endl;
        //std::cout << "\tneighbours count: " <<  clLists.getNeighbours(cur_atom).size() << std::endl;
        for (auto n_atom : clLists.getNeighbours(cur_atom)) {
            //std::cout << "on next" << std::endl;
            //std::cout << "\t--> neighbour atom coordinates: " << vec_to_string(n_atom.coord) << std::endl;
            if (n_atom.parent_mol != cur_atom->parent_mol) {
                //std::cout << "\t\t--> atoms' parents is not equal" << std::endl;
                cur_dist = glm::distance(cur_atom->coord, n_atom.coord)
                           - (cur_atom->vdw_radius + n_atom.vdw_radius);
                //std::cout << "\t\t\tcur distance: " << cur_dist << std::endl;
                if (cur_dist < 0)
                    return -1;
                mol_sum += cur_dist;
                //std::cout << "\t\t\tmol sum: " << mol_sum << std::endl;
            }
            //std::cout << "\t\t\titer success" << std::endl;
        }
        //std::cout << "\tgoing to next atom" << std::endl;
    }
    std::cout << "mol_sum: " << mol_sum << std::endl;
    return mol_sum;
}

std::vector<float> HJLattice::getLatticeSize() {
    std::vector<float> res_sizes;

    // TODO: write some code here

    return res_sizes;
}

void HJLattice::updateAtomIndexes(int mol_ind) {
    for (int i = 0; i < mols[mol_ind].AtomsCount(); ++i) {
        clLists.remAtom(&mols[mol_ind].GetAtom(i));
        clLists.addAtom(&mols[mol_ind].GetAtom(i));
    }
}

void HJLattice::moveByCode(int op_num, int mol_idx, float disp_val) {
    glm::vec3 v_rot_x = glm::normalize(vec_i);
    glm::vec3 v_rot_y = glm::normalize(vec_j);
    glm::vec3 v_rot_z = glm::normalize(vec_k);

    switch(op_num) {
        case 0:
            mols[mol_idx].rotateX(disp_val);
            break;
        case 1:
            mols[mol_idx].rotateY(disp_val);
            break;
        case 2:
            mols[mol_idx].rotateZ(disp_val);
            break;
        case 3:
            mols[mol_idx].translate(disp_val * v_rot_x);
            break;
        case 4:
            mols[mol_idx].translate(disp_val * v_rot_y);
            break;
        case 5:
            mols[mol_idx].translate(disp_val * v_rot_z);
            break;
        default:
            break;
    }
}

