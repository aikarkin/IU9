//
// Created by alex on 05.12.16.
//

#include "MolLattice.h"
#include <OptimizationAlgos.h>
#include "rapidjson/document.h"
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <fstream>


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
    // move apposition point of shell in right place inside initial hjLattice shell
    glm::vec3 P = (cell_shift_i - len_i)/2*vec_i + (cell_shift_j - len_j)/2*vec_j + (cell_shift_k - len_k)*vec_k;
    glm::vec3 vec_app_shift = (P - shell_appos);
    initialShell.translate(-vec_app_shift);

    // hjLattice generation
    int n = (int)ceilf(len_a/cell_shift_i); // amount of hjLattice cells in vec_i direction
    int m = (int)ceilf(len_b/cell_shift_j); // amount of hjLattice cells in vec_j direction
    int l = (int)ceilf(len_c/cell_shift_k); // amount of hjLattice cells in vec_k direction

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
        std::cerr << "Error: unable to generate hjLattice with current configuration. Try to change single cell length." << std::endl;
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

IMolShell &ShellCellLattice::getInitialShell() {
    return initialShell;
}

HJLattice::HJLattice(Molecule mol_prototype) : ClosestPackedLattice(mol_prototype) {
    MolCubeShell cubeShell(&mol_proto);

    MolCubeShell::compareCubeEdgeLes compareFunc = [](float len, float len_opt) {
        return len < len_opt;
    };

    cubeShell.pack(std::bind(compareFunc, std::placeholders::_1, std::placeholders::_2));

    mols.push_back(mol_proto);

    std::vector<glm::vec3> disp_vecs(cubeShell.getDisplacementVectors());

    vec_i = disp_vecs[0];
    vec_j = disp_vecs[1];
    vec_k = disp_vecs[2];

    len_i = glm::length(disp_vecs[0]);
    len_j = glm::length(disp_vecs[1]);
    len_k = glm::length(disp_vecs[2]);

    cur_i = 0;
    cur_j = 0;
    cur_k = 0;
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

    cur_k++;
    return true;
}

void HJLattice::pack() {
    // cell linked lists forming
    float cll_a = n_i * len_i + 2.f;
    float cll_b = n_j * len_j + 2.f;
    float cll_c = n_k * len_k + 2.f;

    std::shared_ptr<CellLinkedLists> clLists = std::make_shared<CellLinkedLists>(cll_cell_len, cll_a, cll_b, cll_c);

    //std::cout << "mols count: " << mols.size() << std::endl;

    for (int i = 0; i < mols.size(); ++i) {
        clLists->addMol(mols[i]);
    }

    //std::shared_ptr<float> sum_dist = std::make_shared<float>(0);
    float sum_dist = 0;
    float cur_dist;

    for (int i = 0; i < mols.size(); ++i) {
        cur_dist = clLists->totalNAtomsDist(mols[i]);
        sum_dist += cur_dist;
        if(cur_dist > 0) {
            //std::cout << "total neighbour atoms distances: " << cur_dist << std::endl;
        }
    }

    //std::cout << std::endl << "initial sum: " << sum_dist << std::endl << std::endl;

    // Hooke Jeeves params preparation
    ublas::vector<float> point(mols.size()*6);
    std::vector<float> displacement;
    std::vector<float> epsilon(point.size());

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

    // optimized function
    OFuncCallback opt_func = [&sum_dist, mols=mols, clLists](ublas::vector<float>& p, int c_idx) -> float {
        float c_val = p[c_idx];
        int mol_idx = c_idx / 6;
        int op_num = c_idx % 6;

        MoveOperation  m_op = (MoveOperation)op_num;
        Molecule cur_mol = mols[mol_idx];

        float old_mol_sum = clLists->totalNAtomsDist(cur_mol);

        if(!clLists->moveMol(cur_mol, m_op, c_val))
            return -1.f;

        float cur_mol_sum = clLists->totalNAtomsDist(cur_mol);
        
        clLists->moveMol(cur_mol, m_op, -c_val);
        
        if(cur_mol_sum < 0)
        	return -1.f;

        sum_dist = sum_dist - old_mol_sum + cur_mol_sum;
//        std::cout << "old_mol_sum: " << old_mol_sum << std::endl;
//        std::cout << "cur_mol_sum: " << old_mol_sum << std::endl;
//        std::cout << "sum_dist: " << sum_dist << std::endl;
//        std::cout << "--------------------" << std::endl;

        return sum_dist;
    };

    // Hooke Jeeves Algorithm
    ublas::vector<float> hj_res(HookeJeevesOptimize(point, displacement, epsilon, opt_func));
    std::vector<Molecule> new_mols;

    for (int i = 0; i < hj_res.size(); ++i) {
        float c_val = hj_res[i];
        int mol_idx = i / 6;
        int op_num = i % 6;

        MoveOperation  m_op = (MoveOperation)op_num;
        moveMol(mols[mol_idx], m_op, c_val);
    }

    bool in_box;
    for (int i = 0; i < mols.size(); ++i) {
        in_box = true;
        glm::vec3 atom_coord;
        for (int j = 0; j < mols[i].AtomsCount(); ++j) {
            atom_coord = mols[i].GetAtom(j).coord;
            if(atom_coord.x < 0 || atom_coord.x > box_a
                    || atom_coord.y < 0 || atom_coord.y > box_b
                    || atom_coord.z < 0 || atom_coord.z > box_c) {
                in_box = false;
                break;
            }
        }
        if(in_box) {
            new_mols.push_back(mols[i]);
        }
    }
    mols.clear();
    mols = new_mols;

}


std::vector<float> HJLattice::getLatticeSize() {
    std::vector<float> res_sizes;

    // TODO: write some code here

    return res_sizes;
}

float HJLattice::getMaxAtomRadius() {
    float max_radius = 0;
    float cur_radius;

    for (int i = 0; i < mol_proto.AtomsCount(); ++i) {
        cur_radius = mol_proto.GetAtom(i).vdw_radius;
        if (cur_radius > max_radius)
            max_radius = cur_radius;
    }

    return max_radius;
}

void HJLattice::setBoxSize(float a, float b, float c) {
    ClosestPackedLattice::setBoxSize(a, b, c);

    cll_cell_len = 2*getMaxAtomRadius();

    n_i = (int)(2*box_a/cll_cell_len);
    n_j = (int)(2*box_b/cll_cell_len);
    n_k = (int)(2*box_c/cll_cell_len);
}

void HJLattice::moveMol(Molecule &mol, MoveOperation move_op, float val) {
    glm::vec3 vec_i(1, 0, 0);
    glm::vec3 vec_j(0, 1, 0);
    glm::vec3 vec_k(0, 0, 1);

    switch(move_op) {
        case ROT_X:
            mol.rotateX(val);
            break;
        case ROT_Y:
            mol.rotateY(val);
            break;
        case ROT_Z:
            mol.rotateZ(val);
            break;
        case TRANS_X:
            mol.translate(val*vec_i);
            break;
        case TRANS_Y:
            mol.translate(val*vec_j);
            break;
        case TRANS_Z:
            mol.translate(val*vec_k);
            break;
        default:
            break;
    }
}

HJLattice::HJLattice(std::vector<Molecule> &molecules) : ClosestPackedLattice(molecules) {
    cur_i = 0;
    cur_j = 0;
    cur_k = 0;

    len_i = glm::length(molecules[0].GetBarycenter() - molecules[1].GetBarycenter());
    len_j = len_i;
    len_k = len_i;
}

void HJLattice::saveToFile(std::string output_file, std::string output_format) {
    if(output_format == "json") {
        rapidjson::Document jsonDoc;
        jsonDoc.SetObject();
        rapidjson::Value mols_arr(rapidjson::kArrayType);
        rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();

        for (int i = 0; i < mols.size(); ++i) {
            rapidjson::Value mol(rapidjson::kObjectType);

            rapidjson::Value bonds(rapidjson::kArrayType);

            for (int j = 0; j < mols[i].BondsCount(); ++j) {
                rapidjson::Value begin;
                begin.SetString(rapidjson::StringRef(std::to_string(mols[i].GetBond(j).begin->atom_idx).c_str()));
                rapidjson::Value end;
                end.SetInt(mols[i].GetBond(j).end->atom_idx);
                rapidjson::Value bond(rapidjson::kObjectType);
                bond.AddMember(begin, end, allocator);

                bonds.PushBack(bond, allocator);
            }

            rapidjson::Value atoms(rapidjson::kObjectType);
            //atoms.SetObject();
            for (int j = 0; j < mols[i].AtomsCount(); ++j) {
                char buf[4];
                int len = std::sprintf(buf, "%d", mols[i].GetAtom(j).atom_idx);
                //std::string a_idx = std::to_string();
                //std::cout << "a_idx: " << a_idx << std::endl;

                rapidjson::Value atom(rapidjson::kObjectType);

                rapidjson::Value atomic_num(mols[i].GetAtom(j).atomic_number);
                atom.AddMember("atomic_num", atomic_num, allocator);

                rapidjson::Value symbol(rapidjson::kObjectType);
                symbol.SetString(mols[i].GetAtom(j).symbol.c_str(), allocator);
                atom.AddMember("symbol", symbol, allocator);
                rapidjson::Value position(rapidjson::kArrayType);
                //position.SetArray();
                position.PushBack(mols[i].GetAtom(j).coord.x, allocator).PushBack(mols[i].GetAtom(j).coord.y, allocator).PushBack(mols[i].GetAtom(j).coord.z, allocator);
                atom.AddMember("position", position, allocator);

                rapidjson::Value vdw_radius(mols[i].GetAtom(j).vdw_radius);
                atom.AddMember("vdw_radius",vdw_radius, allocator);

                rapidjson::Value color(rapidjson::kArrayType);
                color.PushBack(mols[i].GetAtom(j).color.red, allocator).PushBack(mols[i].GetAtom(j).color.green, allocator).PushBack(mols[i].GetAtom(j).color.blue, allocator);
                atom.AddMember("color", color, allocator);

                rapidjson::Value atom_idx;
                atom_idx.SetString(buf, len, allocator);
                atoms.AddMember(atom_idx, atom, allocator);
            }
            mol.AddMember("atoms", atoms, allocator);
            mol.AddMember("bonds", bonds, allocator);
            mols_arr.PushBack(mol, allocator);
        }

        jsonDoc.AddMember("mols", mols_arr, jsonDoc.GetAllocator());
        std::ofstream ofs(output_file);
        rapidjson::OStreamWrapper osw(ofs);

        rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
        jsonDoc.Accept(writer);

        return;
    }

    std::filebuf fb;
    fb.open (output_file, std::ios::out);

    if(!fb.is_open()) {
        std::cout << "Result: failed" << std::endl;
        std::cerr << "error: Can't write to output file" << std::endl;
    }

    std::ostream out_stream(&fb);


    OpenBabel::OBConversion obConversion;
    obConversion.SetOutStream(&out_stream);
    obConversion.SetOutFormat(output_format.c_str(), false);

    std::shared_ptr<OpenBabel::OBMol> mol_lattice = std::make_shared<OpenBabel::OBMol>();
    int atom_b_idx;
    int atom_e_idx;

    for (int i = 0; i < mols.size(); ++i) {
        OpenBabel::OBMol cur_mol = mols[i].OBMol();

        FOR_ATOMS_OF_MOL(a, cur_mol) {
            mol_lattice->AddAtom(*a);
        }

        for (int j = 0; j < mol_proto.BondsCount(); ++j) {
            atom_b_idx = i * mol_proto.AtomsCount() + mol_proto.GetBond(j).begin->atom_idx;
            atom_e_idx = i * mol_proto.AtomsCount() + mol_proto.GetBond(j).end->atom_idx;
            mol_lattice->AddBond(atom_b_idx, atom_e_idx, 1);
        }
    }

    obConversion.Write(mol_lattice.get());
    fb.close();

    std::cout << "Result: success\nMolecules packed: " << mols.size()
              /* << "\nBox size: " << res_box_size[0] << "x" << res_box_size[1] << "x" << res_box_size[2]
               << "\nEdge length of cell: " << cell_len */ << std::endl;
}
