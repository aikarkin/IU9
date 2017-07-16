//
// Created by alex on 24.02.17.
//

#include <hj_algo.h>
#include "rapidjson/document.h"
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
<<<<<<< HEAD
=======
#include <GL/glew.h>
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
#include <ctime>
#include <boost/filesystem.hpp>

pmols::HJPacker::HJPacker(HJParams params) {
    this->params = params;
    init();
}
    
void pmols::HJPacker::init() {
<<<<<<< HEAD
    int mols_count_x, mols_count_y, mols_count_z;
    float shell_length, shell_width, shell_height;
    float shift_x, shift_y, shift_z;
    std::tuple<float, float, float> shellSize;

    Molecule mol(params.mol_file);
    std::tie(appos_point, shellSize) = mol.GetRectangularShell();
    std::tie(shell_length, shell_width, shell_height) = shellSize;

    shell_length *= params.compression;
    shell_width *= params.compression;
    shell_height *= params.compression;

    mols_count_x = (int)ceilf(params.box_length / shell_length);
    mols_count_y = (int)ceilf(params.box_width / shell_width);
    mols_count_z = (int)ceilf(params.box_height / shell_height);

    float exp_length = params.box_length * params.expansivity;
    float exp_width = params.box_width * params.expansivity;
    float exp_height = params.box_height * params.expansivity;

    float dx = (exp_length - params.box_length) / 2.f;
    float dy = (exp_width - params.box_width) / 2.f;
    float dz = (exp_height - params.box_height) / 2.f;

    cellLinkedLists = std::make_shared<CellLinkedLists>(params.cell_length, params.distanceFunc);
    cellLinkedLists->SetBoundingBox(
            std::make_tuple(appos_point - glm::vec3(dx, dy, dz), std::make_tuple(exp_length, exp_width, exp_height))
    );


    for (int i = 0; i < mols_count_x; ++i) {
        shift_x  = i * shell_length;
        for (int j = 0; j < mols_count_y; ++j) {
            shift_y = j * shell_width;
            for (int k = 0; k < mols_count_z; ++k) {
=======
    Molecule mol(params.mol_file);
    std::tuple<float, float, float> shellSize;
    
    float shell_length, shell_width, shell_height;
    int atoms_count_x, atoms_count_y, atoms_count_z;
    
    std::tie(appos_point, shellSize) = mol.GetRectangularShell();
    std::tie(shell_length, shell_width, shell_height) = shellSize;

    float exp_length = params.box_length * params.expansivity;
    float exp_width = params.box_width * params.expansivity;
    float exp_height = params.box_height * params.expansivity;
    
    atoms_count_x = (int)ceilf(exp_length / shell_length);
    atoms_count_y = (int)ceilf(exp_width / shell_width);
    atoms_count_z = (int)ceilf(exp_height / shell_height);
    
    shell_length *= 0.75;
    shell_width *= 0.75;    
    shell_height *= 0.75;

    cellLinkedLists = std::make_shared<CellLinkedLists>(params.cell_length, params.distanceFunc);

    float shift_x, shift_y, shift_z;

    for (int i = 0; i < atoms_count_x; ++i) {
        shift_x  = i * shell_length;
        for (int j = 0; j < atoms_count_y; ++j) {
            shift_y = j * shell_width;
            for (int k = 0; k < atoms_count_z; ++k) {
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
                shift_z = k * shell_height;
                mol.Translate(glm::vec3(shift_x, shift_y, shift_z));
                cellLinkedLists->AddMol(mol);
                mol.Translate(-glm::vec3(shift_x, shift_y, shift_z));
            }
        }
    }
<<<<<<< HEAD
    cellLinkedLists->Update();
=======
    cellLinkedLists->FormCellLinkedLists();
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
    coordVec.resize(6 * cellLinkedLists->MolsCount());
    step.resize(coordVec.size());

    for (int i = 0; i < coordVec.size(); ++i) {
        int mol_idx = i / 6;
<<<<<<< HEAD
        Molecule *mol = cellLinkedLists->GetMol(mol_idx);
        if(mol == NULL) {
            std::cerr << "ERROR: Molecule with current index is not exists. HJPacker initialization failed." << std::endl;
            return;
        }

        switch (i % 6) {
            case 0:
                coordVec[i] = mol->GetBarycenter().x;
                step[i] = params.step_x;
                break;
            case 1:
                coordVec[i] = mol->GetBarycenter().y;
                step[i] = params.step_y;
                break;
            case 2:
                coordVec[i] = mol->GetBarycenter().z;
=======

        switch (i % 6) {
            case 0:
                coordVec[i] = cellLinkedLists->GetMol(mol_idx).GetBarycenter().x;
                step[i] = params.step_x;
                break;
            case 1:
                coordVec[i] = cellLinkedLists->GetMol(mol_idx).GetBarycenter().y;
                step[i] = params.step_y;
                break;
            case 2:
                coordVec[i] = cellLinkedLists->GetMol(mol_idx).GetBarycenter().z;
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
                step[i] = params.step_z;
                break;
            case 3:
                coordVec[i] = 0;
                step[i] = params.step_alpha;
                break;
            case 4:
                coordVec[i] = 0;
                step[i] = params.step_betta;
                break;
            case 5:
                coordVec[i] = 0;
                step[i] = params.step_gamma;
                break;
            default:
                break;
        }
    }
    totalDist = objectiveFunc();
    prevTotalDist = totalDist;

    ptrSearchFpItCount = 0;
    ptrSearchItCount = 0;
    expSearchItCount = 0;
}

void pmols::HJPacker::Pack() {
    ublas::vector<float> x1_ = coordVec;
    ublas::vector<float> x2_ = x1_, x3_, x4_;
    int it_num = 1;


<<<<<<< HEAD
    int search_res = exploringSearch(x2_, true);

    do {
//         std::cout << "STEP #" << ++it_num << std::endl;
=======
    int search_res = exploringSearch(x2_, true); 

    do {
        // std::cout << "STEP #" << ++it_num << std::endl;
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
        if(search_res == 1) {
            ptrSearchItCount++;
            search_res = patternSearch(x1_, x2_);
        }
        else if(search_res == 0) {
            expSearchItCount++;
            search_res = exploringSearch(x2_, true);
        }
<<<<<<< HEAD
//         std::cout << "——————————————————————————————————————————————" << std::endl;
    } while(search_res >= 0);

    removeOutOfBoxMols();
    cellLinkedLists->Update();
=======
        // std::cout << "——————————————————————————————————————————————" << std::endl;
    } while(search_res >= 0);
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
}

float pmols::HJPacker::eps(int coord_number) {
    return coord_number % 6 < 3 ? params.trans_eps : params.rot_eps;
}

void pmols::HJPacker::Save() {
    Save(params.out_file);
}

float pmols::HJPacker::objectiveFunc() {
    float sum = 0;
    float dist;

    for (int i = 0; i < cellLinkedLists->MolsCount(); ++i) {
        dist = cellLinkedLists->MolDist(i);
        sum += dist;
    }

    return sum;
}

int pmols::HJPacker::exploringSearch(ublas::vector<float> &x_, bool change_step) {
<<<<<<< HEAD
//     std::cout << "EXPLORING SEARCH (with " << (change_step ? "step change" : "no step change") << ")" << std::endl;
//     std::cout << " —— objective func before iteration: " << prevTotalDist << std::endl;
=======
    // std::cout << "EXPLORING SEARCH (with " << (change_step ? "step change" : "no step change") << ")" << std::endl;
    // std::cout << " —— objective func before iteration: " << totalDist << std::endl;
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
    // std::cout << "  prevTotalDist: " << prevTotalDist << std::endl;
    // std::cout << "  totalDist: " << totalDist << std::endl;

    float sum1, sum2;
    int mol_idx, op_num;
    float dist0, dist;
    bool changed = false;
    bool terminate = true;
    bool mol_moved;
    float max_step_rot = 0, max_step_trans = 0;
    float min_step_rot = params.step_alpha, min_step_trans = params.step_x;

    for (int i = 0; i < x_.size(); ++i) {
<<<<<<< HEAD
//        std::cout << "\tcoord number: " << i << std::endl;
=======
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
        if(step[i] < eps(i))
            continue;

        if(terminate)
            terminate = false;

        mol_idx = i / 6;
        op_num = i % 6;
        dist0 = cellLinkedLists->MolDist(mol_idx);

        mol_moved = cellLinkedLists->MoveMol(mol_idx, (MOVE_OP)op_num, step[i]);
        dist = cellLinkedLists->MolDist(mol_idx);
        sum1 = mol_moved ? ( totalDist - dist0 + dist ) : totalDist;
        if(mol_moved) {
            cellLinkedLists->CancelMove();
        }

        mol_moved = cellLinkedLists->MoveMol(mol_idx, (MOVE_OP)op_num, -step[i]);
        dist = cellLinkedLists->MolDist(mol_idx);
        sum2 = mol_moved ? ( totalDist - dist0 + dist ) : totalDist;
        if(mol_moved) {
            cellLinkedLists->CancelMove();
        }

        if (totalDist <= std::min(sum1, sum2) && change_step)
            step[i] *= params.step_coefficient;
        else {
            if(sum1 < std::min(totalDist, sum2)) {
                x_[i] += step[i];
                cellLinkedLists->MoveMol(mol_idx, (MOVE_OP)op_num, step[i]);
                totalDist = sum1;
                changed = true;
            }
            else if(sum2 < std::min(totalDist, sum1)) {
                x_[i] -= step[i];
                cellLinkedLists->MoveMol(mol_idx, (MOVE_OP)op_num, -step[i]);
                totalDist = sum2;
                changed = true;
            }
        }
    }
<<<<<<< HEAD
//    std::cout << " —— objective func after iteration: " << totalDist << std::endl;
=======
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043

    if (terminate)
        return -1;

    if(!change_step) {
        if (totalDist < prevTotalDist) {
            prevTotalDist = totalDist;
            return 1;
        } else {
            ptrSearchFpItCount++;
            return 0;
        }
    }

    return (int)changed;
}

void applyVecToCll(pmols::CellLinkedLists &cellLinkedLists, ublas::vector<float> &vec) {
    int mol_idx;
    pmols::MOVE_OP move_op;
    float val;

    for (int i = 0; i < vec.size(); ++i) {
        mol_idx = i / 6;
        move_op = static_cast<pmols::MOVE_OP>(i % 6);
        val = vec[i];
        cellLinkedLists.MoveMol(mol_idx, move_op, val);
    }
}

int pmols::HJPacker::patternSearch(ublas::vector<float> &x1_, ublas::vector<float> &x2_) {
<<<<<<< HEAD
//     std::cout << "PATTERN SEARCH" << std::endl;
=======
    // std::cout << "PATTERN SEARCH" << std::endl;    
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
    ublas::vector<float> x3_ = x1_ + params.lambda * (x2_ - x1_);
    ublas::vector<float> x4_ = x3_;
    ublas::vector<float> dx_ = x3_ - x2_;

    applyVecToCll(*cellLinkedLists, dx_);
    totalDist = objectiveFunc();

    int exploring_res = exploringSearch(x4_, false);

    if(exploring_res == 1) {
        x1_ = x2_;
        x2_ = x4_;
    }
    else if(exploring_res == 0) {
        x1_ = x2_;
    }

    return exploring_res;
}

void pmols::HJPacker::Save(std::string out_file) {
    Atom::obatom_count = 0;
    std::tuple<float, float, float> box_size(params.box_length, params.box_width, params.box_height);

    if(params.out_format == "json") {
        rapidjson::Document jsonDoc;
        jsonDoc.SetObject();
        rapidjson::Value mols_arr(rapidjson::kArrayType);
        rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();
        char b_str_buf[7];

        for (int i = 0; i < cellLinkedLists->MolsCount(); ++i) {
<<<<<<< HEAD
            if(cellLinkedLists->GetMol(i) == NULL)
                continue;
            Molecule mol(*cellLinkedLists->GetMol(i));

=======
            if(!MolInsideBox(cellLinkedLists->GetMol(i), appos_point, box_size))
                continue;
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
            rapidjson::Value mol_val(rapidjson::kObjectType);

            rapidjson::Value bonds(rapidjson::kArrayType);

<<<<<<< HEAD
            for (int j = 0; j < mol.BondsCount(); ++j) {
                rapidjson::Value begin;
                std::string b_str = std::to_string(mol.GetBond(j).begin->atom_idx);
=======
            for (int j = 0; j < cellLinkedLists->GetMol(i).BondsCount(); ++j) {
                rapidjson::Value begin;
                std::string b_str = std::to_string(cellLinkedLists->GetMol(i).GetBond(j).begin->atom_idx);
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043

                int buf_len = sprintf(b_str_buf, "%s", b_str.c_str());
                begin.SetString(b_str_buf, buf_len, allocator);
                memset(b_str_buf, 0, 7);

                rapidjson::Value end;
<<<<<<< HEAD
                end.SetInt(mol.GetBond(j).end->atom_idx);
=======
                end.SetInt(cellLinkedLists->GetMol(i).GetBond(j).end->atom_idx);
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043

                rapidjson::Value bond(rapidjson::kObjectType);
                bond.AddMember(begin, end, allocator);

                bonds.PushBack(bond, allocator);
            }

            rapidjson::Value atoms(rapidjson::kObjectType);
<<<<<<< HEAD
            for (int j = 0; j < mol.AtomsCount(); ++j) {
                char buf[4];
                int len = std::sprintf(buf, "%d", mol.GetAtom(j).atom_idx);
                rapidjson::Value atom(rapidjson::kObjectType);

                rapidjson::Value atomic_num(mol.GetAtom(j).atomic_number);
=======
            for (int j = 0; j < cellLinkedLists->GetMol(i).AtomsCount(); ++j) {
                char buf[4];
                int len = std::sprintf(buf, "%d", cellLinkedLists->GetMol(i).GetAtom(j).atom_idx);
                rapidjson::Value atom(rapidjson::kObjectType);

                rapidjson::Value atomic_num(cellLinkedLists->GetMol(i).GetAtom(j).atomic_number);
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
                atom.AddMember("atomic_num", atomic_num, allocator);

//                rapidjson::Value symbol(rapidjson::kObjectType);
//                symbol.SetString(cellLinkedLists->GetMol(i).GetAtom(j).symbol.c_str(), allocator);
//                atom.AddMember("symbol", symbol, allocator);
                rapidjson::Value position(rapidjson::kArrayType);

                position.
<<<<<<< HEAD
                        PushBack(mol.GetAtom(j).coord.x, allocator).
                        PushBack(mol.GetAtom(j).coord.y, allocator).
                        PushBack(mol.GetAtom(j).coord.z, allocator);

                atom.AddMember("position", position, allocator);

                rapidjson::Value vdw_radius(mol.GetAtom(j).vdw_radius);
=======
                        PushBack(cellLinkedLists->GetMol(i).GetAtom(j).coord.x, allocator).
                        PushBack(cellLinkedLists->GetMol(i).GetAtom(j).coord.y, allocator).
                        PushBack(cellLinkedLists->GetMol(i).GetAtom(j).coord.z, allocator);

                atom.AddMember("position", position, allocator);

                rapidjson::Value vdw_radius(cellLinkedLists->GetMol(i).GetAtom(j).vdw_radius);
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
                atom.AddMember("vdw_radius",vdw_radius, allocator);

                rapidjson::Value color(rapidjson::kArrayType);
                color.
<<<<<<< HEAD
                        PushBack(mol.GetAtom(j).color.red, allocator).
                        PushBack(mol.GetAtom(j).color.green, allocator).
                        PushBack(mol.GetAtom(j).color.blue, allocator);
=======
                        PushBack(cellLinkedLists->GetMol(i).GetAtom(j).color.red, allocator).
                        PushBack(cellLinkedLists->GetMol(i).GetAtom(j).color.green, allocator).
                        PushBack(cellLinkedLists->GetMol(i).GetAtom(j).color.blue, allocator);
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
                atom.AddMember("color", color, allocator);

                rapidjson::Value atom_idx;
                atom_idx.SetString(buf, len, allocator);
                atoms.AddMember(atom_idx, atom, allocator);
            }
            mol_val.AddMember("atoms", atoms, allocator);
            mol_val.AddMember("bonds", bonds, allocator);
            mols_arr.PushBack(mol_val, allocator);
        }

<<<<<<< HEAD
        rapidjson::Value bounding_box(rapidjson::kObjectType);

        rapidjson::Value box_length(params.box_length);
        rapidjson::Value box_width(params.box_width);
        rapidjson::Value box_height(params.box_height);

        rapidjson::Value box_position(rapidjson::kArrayType);
        box_position.
                PushBack(appos_point.x, allocator).
                PushBack(appos_point.y, allocator).
                PushBack(appos_point.z, allocator);

        bounding_box.AddMember("position", box_position, allocator);
        bounding_box.AddMember("length", box_length, allocator);
        bounding_box.AddMember("width", box_width, allocator);
        bounding_box.AddMember("height", box_height, allocator);

        jsonDoc.AddMember("bounding_box", bounding_box, allocator);
        jsonDoc.AddMember("mols", mols_arr, jsonDoc.GetAllocator());

=======
        jsonDoc.AddMember("mols", mols_arr, jsonDoc.GetAllocator());
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
        std::ofstream ofs(out_file);
        rapidjson::OStreamWrapper osw(ofs);

        rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
        jsonDoc.Accept(writer);

        return;
    }

    std::filebuf fb;
    fb.open (out_file, std::ios::out);

    if(!fb.is_open()) {
        std::cerr << "error: Can't write to output file" << std::endl;
    }

    std::ostream out_stream(&fb);

    OpenBabel::OBConversion obConversion;
    obConversion.SetOutStream(&out_stream);
<<<<<<< HEAD
    obConversion.SetOutFormat(params.out_format.c_str());
=======
    obConversion.SetOutFormat(params.out_format.c_str(), false);
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043

    std::shared_ptr<OpenBabel::OBMol> mol_lattice = std::make_shared<OpenBabel::OBMol>();
    int atom_b_id;
    int atom_e_id;

    for (int i = 0; i < cellLinkedLists->MolsCount(); ++i) {
<<<<<<< HEAD
        if(cellLinkedLists->GetMol(i) == NULL)
            continue;
        Molecule mol(*cellLinkedLists->GetMol(i));

        for (int j = 0; j < mol.AtomsCount(); ++j) {
            OpenBabel::OBAtom obAtom = mol.GetAtom(j).OBAtom();
            mol_lattice->AddAtom(obAtom);
        }

        for (int j = 0; j < mol.BondsCount(); ++j) {
            atom_b_id = mol.GetBond(j).begin->atom_id;
            atom_e_id = mol.GetBond(j).end->atom_id;
=======
        if(!MolInsideBox(cellLinkedLists->GetMol(i), appos_point, box_size))
            continue;
        for (int j = 0; j < cellLinkedLists->GetMol(i).AtomsCount(); ++j) {
            OpenBabel::OBAtom obAtom = cellLinkedLists->GetMol(i).GetAtom(j).OBAtom();
            mol_lattice->AddAtom(obAtom);
        }

        for (int j = 0; j < cellLinkedLists->GetMol(i).BondsCount(); ++j) {
            atom_b_id = cellLinkedLists->GetMol(i).GetBond(j).begin->atom_id;
            atom_e_id = cellLinkedLists->GetMol(i).GetBond(j).end->atom_id;
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
            mol_lattice->AddBond(atom_b_id, atom_e_id, 1);
        }
    }

    obConversion.Write(mol_lattice.get());
    fb.close();
}

pmols::HJStatistics pmols::HJPacker::GetStatistics() {
    return pmols::HJStatistics(cellLinkedLists.get(),
         expSearchItCount, ptrSearchItCount, ptrSearchFpItCount, 
<<<<<<< HEAD
         std::make_tuple(params.box_length, params.box_width, params.box_height));
}

void pmols::HJPacker::removeOutOfBoxMols() {
    for (int i = 0; i < cellLinkedLists->MolsCount(); ++i) {
        if(cellLinkedLists->GetMol(i) == NULL)
            continue;
        Molecule mol(*cellLinkedLists->GetMol(i));
        std::tuple<float, float, float> boxSize(params.box_length, params.box_width, params.box_height);
        if(!MolInsideBox(mol, appos_point, boxSize)) {
            cellLinkedLists->RemoveMol(i);
        }
    }
=======
         std::tuple<float, float, float>(params.box_length, params.box_width, params.box_height));
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
}

pmols::HJStatistics::HJStatistics(pmols::CellLinkedLists *cellLinkedLists,
                                  int expSearchIt, int ptrSearchIt, 
                                  int ptrSearchFpIt, std::tuple<float, float, float> boxSize) {
    es_it = expSearchIt + 1;
    ps_it = ptrSearchIt;
    ps_fp_it = ptrSearchFpIt;
    it = ps_it + es_it;
<<<<<<< HEAD
=======
    box_size = boxSize;
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043


    calcMolStatistics(cellLinkedLists);
}

void pmols::HJStatistics::calcMolStatistics(pmols::CellLinkedLists *cll) {
    Atom atom;
    float dist;

    empty_cells = 0;
    inters = 0;
    mols_count = 0;

    min_atoms_dist = FLT_MAX;
    max_atoms_dist = 0;
    atoms_sum = 0;

    min_inter = FLT_MAX;
    max_inter = 0;
    inter_sum = 0;
<<<<<<< HEAD
    int mol_sum_added = 0;;
    boundingBox = cll->GetBoundingBox();

    for (int i = 0; i < cll->MolsCount(); ++i) {
        Molecule mol(*cll->mols[i]);
=======
    int mol_sum_added = 0;
    

    for (int i = 0; i < cll->MolsCount(); ++i) {
        Molecule mol = cll->mols[i];
        if(!MolInsideBox(mol, cll->appos_point, box_size))
            continue;
        mols_count++;
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
        for (int j = 0; j < mol.AtomsCount(); ++j) {
            atom = mol.GetAtom(j);
            CLLNeighbourCells neighbourCells = CLLNeighbourCells(cll, atom);

            while(neighbourCells.hasNext()) {
                std::list<Atom> cell_list = neighbourCells.next();
                if (!cell_list.empty()) {
                    for (auto na: cell_list) {
                        if (atom.parent_mol_id != na.parent_mol_id) {
                            dist = atomsEDist(atom, na);
                            atoms_sum += dist;
                            mol_sum_added++;
                            if (dist < 0) {
                                inter_sum += std::fabs(dist);

                                if (std::fabs(dist) < min_inter)
                                    min_inter = std::fabs(dist);
                                else if (std::fabs(dist) > max_inter)
                                    max_inter = std::fabs(dist);

                                inters++;
<<<<<<< HEAD
                            }

                            if (dist < min_atoms_dist)
                                min_atoms_dist = dist;
                            else if (dist > max_atoms_dist)
                                max_atoms_dist = dist;

=======
                            } else {
                                if (dist < min_atoms_dist)
                                    min_atoms_dist = dist;
                                else if (dist > max_atoms_dist)
                                    max_atoms_dist = dist;
                            }
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < cll->cells_count_x; ++i) {
        for (int j = 0; j < cll->cells_count_y; ++j) {
            for (int k = 0; k < cll->cells_count_z; ++k) {
                if(cll->atom_grid[i][j][k].empty())
                    empty_cells++;
            }
        }
    }

<<<<<<< HEAD
    mols_count = (int)cll->MolsCount();
    avg_atoms_sum = atoms_sum / (float)mol_sum_added;
=======
    avg_atoms_sum = atoms_sum / mol_sum_added;
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
    avg_inter = inter_sum / (float)inters;
}

float pmols::HJStatistics::atomsEDist(Atom &a, Atom &b) {
    return (float)glm::distance(a.coord, b.coord) - a.vdw_radius - b.vdw_radius;
}
<<<<<<< HEAD

std::tuple<glm::vec3, std::tuple<float, float, float>> pmols::HJStatistics::GetBoundingBox() {
    return boundingBox;
}
=======
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
