//
// Created by alex on 05.12.16.
//

#include "mol_lattice.h"
#include "rapidjson/document.h"
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <fstream>


bool isOrthogonal(glm::vec3 vec1, glm::vec3 vec2) {
    return std::abs(glm::angle(vec1, vec2) - (float)M_PI_2) < 0.0001f;
};

void pmols::ClosestPackedLattice::formLattice() {
    boost::tuple<glm::vec3, boost::tuple<float, float, float>> rect_shell = mol_proto.GetRectangularShell();
    glm::vec3 appos_point = rect_shell.get<0>();

    float dx = rect_shell.get<1>().get<0>(),
            dy = rect_shell.get<1>().get<1>(),
            dz = rect_shell.get<1>().get<2>();

//    std::cout << "shell size: " << dx << "x" << dy << "x" << dz << std::endl;

    int nx = (int)ceilf(length / dx),
            ny = (int)ceilf(width / dy),
            nz = (int)ceilf(height / dz);

    float shift_x, shift_y, shift_z;
    mol_proto.Translate(-appos_point);

    for (int i = 0; i < nx; ++i) {
        shift_x = i * dx;
        for (int j = 0; j < ny; ++j) {
            shift_y = j * dy;
            for (int k = 0; k < nz; ++k) {
                shift_z = k * dz;
                mol_proto.Translate(glm::vec3(shift_x, shift_y, shift_z));
                mols.push_back(mol_proto);
                mol_proto.Translate(glm::vec3(-shift_x, -shift_y, -shift_z));
            }
        }
    }
}

void pmols::ClosestPackedLattice::saveToFile(std::string output_file, std::string output_format) {
    if(output_format == "json") {
        rapidjson::Document jsonDoc;
        jsonDoc.SetObject();
        rapidjson::Value mols_arr(rapidjson::kArrayType);
        rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();
        char b_str_buf[7];

        for (int i = 0; i < mols.size(); ++i) {
            rapidjson::Value mol_val(rapidjson::kObjectType);

            rapidjson::Value bonds(rapidjson::kArrayType);

            for (int j = 0; j < mols[i].BondsCount(); ++j) {
                rapidjson::Value begin;
                std::string b_str = std::to_string(mols[i].GetBond(j).begin->atom_idx);

                int buf_len = sprintf(b_str_buf, "%s", b_str.c_str());
////                std::cout << "buf_len: " << buf_len << std::endl;
////                std::cout << "buf: " << b_str_buf << std::endl;
                begin.SetString(b_str_buf, buf_len, allocator);
                memset(b_str_buf, 0, 7);

                rapidjson::Value end;
                end.SetInt(mols[i].GetBond(j).end->atom_idx);
////                std::cout << "json - bond # " << j << ": " << begin.GetString() << " --> " << end.GetInt() << std::endl;

                rapidjson::Value bond(rapidjson::kObjectType);
                bond.AddMember(begin, end, allocator);

                bonds.PushBack(bond, allocator);
            }

            rapidjson::Value atoms(rapidjson::kObjectType);
            for (int j = 0; j < mols[i].AtomsCount(); ++j) {
                char buf[4];
                int len = std::sprintf(buf, "%d", mols[i].GetAtom(j).atom_idx);
                rapidjson::Value atom(rapidjson::kObjectType);

                rapidjson::Value atomic_num(mols[i].GetAtom(j).atomic_number);
                atom.AddMember("atomic_num", atomic_num, allocator);

                rapidjson::Value symbol(rapidjson::kObjectType);
                symbol.SetString(mols[i].GetAtom(j).symbol.c_str(), allocator);
                atom.AddMember("symbol", symbol, allocator);
                rapidjson::Value position(rapidjson::kArrayType);

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
            mol_val.AddMember("atoms", atoms, allocator);
            mol_val.AddMember("bonds", bonds, allocator);
            mols_arr.PushBack(mol_val, allocator);
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
//        std::cout << "Result: failed" << std::endl;
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
        for (int j = 0; j < mols[i].AtomsCount(); ++j) {
            OpenBabel::OBAtom obAtom = mols[i].GetAtom(j).OBAtom();
            mol_lattice->AddAtom(obAtom);
        }

        for (int j = 0; j < mols[i].BondsCount(); ++j) {
            atom_b_idx = mols[i].GetBond(j).begin->atom_idx;
            atom_e_idx = mols[i].GetBond(j).end->atom_idx;
            mol_lattice->AddBond(atom_b_idx, atom_e_idx, 1);
        }
    }

    obConversion.Write(mol_lattice.get());
    fb.close();

    std::cout << "Result: success\nMolecules packed: " << mols.size()
              /* << "\nBox size: " << res_box_size[0] << "x" << res_box_size[1] << "x" << res_box_size[2]
               << "\nEdge length of cell: " << cell_len */ << std::endl;
}


void pmols::HJLattice::setPrecision(float tx_eps, float ty_eps, float tz_eps, float x_eps, float y_eps, float z_eps) {
    tx_e = tx_eps;
    ty_e = ty_eps;
    tz_e = tz_eps;

    x_e = x_eps;
    y_e = y_eps;
    z_e = z_eps;
}


void pmols::HJLattice::pack() {
//    std::cout << "packing ..." << std::endl;
    // ! Optimization params preparation
    cll_cell_len = 2*getMaxAtomRadius();

    // cell linked lists forming
    std::shared_ptr<CellLinkedLists> clLists
            = std::make_shared<CellLinkedLists>(cll_cell_len, 2*length, 2*width, 2*height);

    bool mol_added;
    for (int i = 0; i < mols.size(); ++i) {
        mol_added = clLists->addMol(mols[i]);
    }
    clLists -> saveToCSV("../resources/cell_linked_lists_initial.csv");
    std::cout << "Cell Linked lists has successfully." << std::endl;

    // calc initial sum
    float sum_dist = 0;
    float cur_dist;

    for (int i = 0; i < mols.size(); ++i) {
        cur_dist = clLists->totalNAtomsDist(mols[i]);
        sum_dist += cur_dist;
    }
    std::cout << "initial sum of dist: " << sum_dist << std::endl;

    prepareOptimizationParams();

    // optimized function
    OFuncCallback opt_func = [&sum_dist, mols=mols, clLists](ublas::vector<float>& p, int c_idx) -> float {
        float c_val = p[c_idx];
        int mol_idx = c_idx / 6;
        int op_num = c_idx % 6;

//        std::cout << "opt_func - initial params: c_val: " << c_val
//                  << "mol_idx: " << mol_idx
//                  << "op_num: " << op_num << std::endl;

        MoveOperation  m_op = (MoveOperation)op_num;
        Molecule cur_mol = mols[mol_idx];

        float old_mol_sum = clLists->totalNAtomsDist(cur_mol);
        //std::cout << "old_mol_sum: " << old_mol_sum << std::endl;
        bool mol_moved = clLists->moveMol(cur_mol, m_op, c_val);
        //std::cout << "opt_func - mol_moved=" << mol_moved << std::endl;

        if(!mol_moved)
        {
            //std::cout << "mol was not move" << std::endl;
            return -1.f;
        }
        //std::cout << "\tmol moved" << std::endl;
        float cur_mol_sum = clLists->totalNAtomsDist(cur_mol);

        clLists->moveMol(cur_mol, m_op, -c_val);

        if(cur_mol_sum < 0)
            return -1.f;

        sum_dist = sum_dist - old_mol_sum + cur_mol_sum;
////        std::cout << "old_mol_sum: " << old_mol_sum << std::endl;
////        std::cout << "cur_mol_sum: " << old_mol_sum << std::endl;
//        //std::cout << "sum_dist: " << sum_dist << std::endl;
////        std::cout << "--------------------" << std::endl;
        std::cout << "cur sum_dist: " << sum_dist << std::endl;
        return sum_dist;
    };

    // Hooke Jeeves Algorithm
    ublas::vector<float> hj_res(HookeJeevesOptimize(points, displacements, precisions, opt_func));
    std::vector<Molecule> new_mols;

    for (int i = 0; i < hj_res.size(); ++i) {
        float c_val = hj_res[i];
        int mol_idx = i / 6;
        int op_num = i % 6;

        MoveOperation  m_op = (MoveOperation)op_num;
        moveMol(mols[mol_idx], m_op, c_val);
    }

//    std::cout << "final sum of dist: " << sum_dist << std::endl;

    bool in_box;

//    std::cout << "Result atoms: " << std::endl;
    for (int i = 0; i < mols.size(); ++i) {
        in_box = true;
        glm::vec3 atom_coord;
        for (int j = 0; j < mols[i].AtomsCount(); ++j) {
            atom_coord = mols[i].GetAtom(j).coord;

            if(atom_coord.x < 0 || atom_coord.x > length
               || atom_coord.y < 0 || atom_coord.y > width
               || atom_coord.z < 0 || atom_coord.z > height) {
                in_box = false;
                break;
            }
        }
        if(in_box) {
            new_mols.push_back(mols[i]);
//            std::cout << "Mol #" << i + 1 << " - " << new_mols[i].GetFormula() << std::endl;
//            for (int j = 0; j < new_mols[i].AtomsCount(); ++j) {
//                std::cout << "\tatom" << j + 1 << new_mols[i].GetAtom(j).toString() << std:: endl;
//            }
        }
    }
    mols.clear();
    mols = new_mols;

}


float pmols::HJLattice::getMaxAtomRadius() {
    float max_radius = 0;
    float cur_radius;
//    std::cout << "HJLattice params preparation: finding maximum Van Der Waals radius" << std::endl;

    for (int i = 0; i < mol_proto.AtomsCount(); ++i) {
        cur_radius = mol_proto.GetAtom(i).vdw_radius;
//        std::cout << "\tcur rad: " << cur_radius << std::endl;
        if (cur_radius > max_radius)
            max_radius = cur_radius;
    }


//    std::cout << "\tmax rad: " << max_radius << std::endl;
    return max_radius;
}

void pmols::HJLattice::moveMol(Molecule &mol, MoveOperation move_op, float val) {
    glm::vec3 vec_i(1, 0, 0);
    glm::vec3 vec_j(0, 1, 0);
    glm::vec3 vec_k(0, 0, 1);

    switch(move_op) {
        case ROT_X:
            mol.RotateX(val);
            break;
        case ROT_Y:
            mol.RotateY(val);
            break;
        case ROT_Z:
            mol.RotateZ(val);
            break;
        case TRANS_X:
            mol.Translate(val * vec_i);
            break;
        case TRANS_Y:
            mol.Translate(val * vec_j);
            break;
        case TRANS_Z:
            mol.Translate(val * vec_k);
            break;
        default:
            break;
    }
}

void pmols::HJLattice::setInitialDisplacement(float shift_x=0.5, float shift_y=0.5, float shift_z=0.5,
                                       float shift_alpha=45, float shift_beta=45, float shift_gamma=45) {
    this->shift_x = shift_x;
    this->shift_y = shift_y;
    this->shift_z = shift_z;

    this->shift_alpha = shift_alpha;
    this->shift_beta = shift_beta;
    this->shift_gamma = shift_gamma;
}

void pmols::HJLattice::prepareOptimizationParams() {
    points = ublas::vector<float>(mols.size()*6);
    precisions = std::vector<float>(points.size());

    for (int i = 0, j = 0; i < points.size(); ++i) {
        points[i] = 0;
        j = i%6;

        if(j < 3)
            displacements.push_back(45.f);
        else
            displacements.push_back(0.5f);

        switch(j) {
            case 0:
                precisions[i] = tx_e;
                break;
            case 1:
                precisions[i] = ty_e;
                break;
            case 2:
                precisions[i] = tz_e;
                break;
            case 3:
                precisions[i] = x_e;
                break;
            case 4:
                precisions[i] = y_e;
            case 5:
                precisions[i] = z_e;
                break;
            default:
                break;
        }
    }
}

void pmols::HJLattice::formLattice() {
    float init_length = length;
    float init_width = width;
    float init_height = height;

/*
    length *= 2;
    width *= 2;
    height *= 2;
*/    

    ClosestPackedLattice::formLattice();
    length = init_length;
    width = init_width;
    height = init_height;
}
