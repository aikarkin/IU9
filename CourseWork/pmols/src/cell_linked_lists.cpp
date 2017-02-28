//
// Created by alex on 09.12.16.
//

#include <mol.h>
#include <cell_linked_lists.h>
#include <memory>


pmols::CellLinkedLists::CellLinkedLists(std::vector<pmols::Molecule> &mols, float cellLen, pmols::DistFunc distFunc) {
    this->mols = mols;
    this->cell_len = cellLen;
    this->dist_func = distFunc;
    FormCellLinkedLists();
}

pmols::CellLinkedLists::CellLinkedLists(float cellLen, pmols::DistFunc distFunc) {
    Set(cellLen, distFunc);
}

void pmols::CellLinkedLists::FormCellLinkedLists() {
    if(formed) {
        std::cout << "WARNING: CellLinkedLists already formed. Operation cancelled." << std::endl;
        return;
    }

    calcBoundingBox();

    atoms_count_x = (int)ceilf(box_length/cell_len);
    atoms_count_y = (int)ceilf(box_width/cell_len);
    atoms_count_z = (int)ceilf(box_height/cell_len);

    // allocate atom_grid
    atom_grid = new std::list<Atom>**[atoms_count_x];
    for (int i = 0; i < atoms_count_x; ++i) {
        atom_grid[i] = new std::list<Atom>*[atoms_count_y];
        for (int j = 0; j < atoms_count_y; ++j) {
            atom_grid[i][j] = new std::list<Atom>[atoms_count_z];
        }
    }

    Atom cur_atom;
    int idx_x, idx_y, idx_z;

    // add each atom to grid
    for (int i = 0; i < mols.size(); ++i) {
        for (int j = 0; j < mols[i].AtomsCount(); ++j) {
            cur_atom = mols[i].GetAtom(j);
            cur_atom.parent_mol_id = i;
            mols[i].GetAtom(j).parent_mol_id = i;

            idx_x = (int)floorf((cur_atom.coord.x - appos_point.x)/cell_len);
            idx_y = (int)floorf((cur_atom.coord.y - appos_point.y)/cell_len);
            idx_z = (int)floorf((cur_atom.coord.z - appos_point.z)/cell_len);

            atom_grid[idx_x][idx_y][idx_z].emplace_back(cur_atom);
        }
    }
    formed = true;
}

bool pmols::CellLinkedLists::AddMol(pmols::Molecule &mol) {
    if(formed)
        return false;

    mols.push_back(mol);
    return true;
}

void pmols::CellLinkedLists::calcBoundingBox() {
    Atom atom = mols[0].GetAtom(0);
    float   min_x =  atom.coord.x - atom.vdw_radius,
            min_y = atom.coord.y - atom.vdw_radius,
            min_z = atom.coord.z - atom.vdw_radius;

    float   max_x =  atom.coord.x + atom.vdw_radius,
            max_y = atom.coord.y + atom.vdw_radius,
            max_z = atom.coord.z + atom.vdw_radius;

    for (int i = 0; i < mols.size(); ++i) {
        for (int j = 0; j < mols[i].AtomsCount(); ++j) {
            atom = mols[i].GetAtom(j);

            if(atom.coord.x - atom.vdw_radius < min_x)
                min_x = atom.coord.x - atom.vdw_radius;
            if(atom.coord.x + atom.vdw_radius > max_x)
                max_x = atom.coord.x + atom.vdw_radius;

            if(atom.coord.y - atom.vdw_radius < min_y)
                min_y = atom.coord.y - atom.vdw_radius;
            if(atom.coord.y + atom.vdw_radius > max_y)
                max_y = atom.coord.y + atom.vdw_radius;

            if(atom.coord.z - atom.vdw_radius < min_z)
                min_z = atom.coord.z - atom.vdw_radius;
            if(atom.coord.z + atom.vdw_radius > max_z)
                max_z = atom.coord.z + atom.vdw_radius;
        }
    }

    appos_point = glm::vec3(min_x, min_y, min_z);

    box_length = max_x - min_x;
    box_width = max_y - min_y;
    box_height = max_z - min_z;
}

bool pmols::CellLinkedLists::MoveMol(int molIdx, pmols::MOVE_OP moveOp, float val) {
    Molecule mol = mols[molIdx];
    last_mol = std::make_shared<std::tuple<int, Molecule>>(std::make_tuple(molIdx, mol));

    switch(moveOp) {
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
            mol.Translate(val * glm::vec3(1, 0, 0));
            break;
        case TRANS_Y:
            mol.Translate(val * glm::vec3(0, 1, 0));
            break;
        case TRANS_Z:
            mol.Translate(val * glm::vec3(0, 0, 1));
            break;
        default:
            break;
    }

    if(!molInsideBox(mol)) {
        last_mol.reset();
        return false;
    }

    mol_moved = true;
    repMol(molIdx, mol);

    return true;
}

size_t pmols::CellLinkedLists::MolsCount() {
    return mols.size();
}

bool pmols::CellLinkedLists::molInsideBox(pmols::Molecule &mol) {
    glm::vec3 coord;
    float vdw_radius;

    for (int i = 0; i < mol.AtomsCount(); ++i) {
        coord = mol.GetAtom(i).coord;
        vdw_radius = mol.GetAtom(i).vdw_radius;

        if (    coord.x - vdw_radius < appos_point.x ||
                coord.x + vdw_radius > appos_point.x + box_length ||
                coord.y - vdw_radius < appos_point.y ||
                coord.y + vdw_radius > appos_point.y + box_width ||
                coord.z - vdw_radius < appos_point.z ||
                coord.z + vdw_radius > appos_point.z + box_height   )
            return false;
    }

    return true;
}

std::tuple<int, int, int> pmols::CellLinkedLists::getCellIndex(pmols::Atom &atom) {
    int i, j, k;

    i = (int)floorf((atom.coord.x - appos_point.x)/cell_len);
    j = (int)floorf((atom.coord.y - appos_point.y)/cell_len);
    k = (int)floorf((atom.coord.z - appos_point.z)/cell_len);

    return std::tuple<int, int, int>(i, j, k);
}

float pmols::CellLinkedLists::MolDist(int molIdx) {
    Molecule mol = mols[molIdx];
    Atom *atom;
    float sum = 0;
    float dist;

    for (int i = 0; i < mol.AtomsCount(); ++i) {
        atom = &mol.GetAtom(i);
        CLLNeighbourCells neighbourCells = CLLNeighbourCells(this, *atom);

        while(neighbourCells.hasNext()) {
            std::list<Atom> cell_list = neighbourCells.next();
            if(cell_list.empty()) {
                dist = dist_func(atom, NULL);
                sum += dist;
            }
            else {
                for (auto na : cell_list) {
                    if (mol.GetAtom(i).parent_mol_id != na.parent_mol_id) {
                        dist = dist_func(atom, &na);
                        sum += dist;
                    }
                }
            }
        }
    }

    return sum;
}

void pmols::CellLinkedLists::Set(float cellLen, pmols::DistFunc distFunc) {
    this->cell_len = cellLen;
    this->dist_func = distFunc;
    mol_moved = false;
    formed = false;
}

pmols::Molecule &pmols::CellLinkedLists::GetMol(int molIdx) {
    return mols[molIdx];
}

pmols::CellLinkedLists::~CellLinkedLists() {
    for(int i = 0; i < atoms_count_x; i++) {
        for (int j = 0; j < atoms_count_y; ++j) {
            for (int k = 0; k < atoms_count_z; ++k) {
                if(!atom_grid[i][j][k].empty())
                    atom_grid[i][j][k].clear();
            }
        }
    }

    for (int i = 0; i < atoms_count_y; ++i) {
        for (int j = 0; j < atoms_count_z; ++j) {
            delete []atom_grid[i][j];
        }
        delete []atom_grid[i];
    }

    delete []atom_grid;
}

void pmols::CellLinkedLists::CancelMove() {
    if(!mol_moved) {
        std::cout << "WARNING: Unable to cancel move operation. No one molecule wasn't move." << std::endl;
        return;
    }

    repMol(std::get<0>(*last_mol), std::get<1>(*last_mol));
    mol_moved = false;
}

void pmols::CellLinkedLists::SaveToCSV(std::string file_path) {
    std::ofstream outf_stream(file_path);

    if(!outf_stream.is_open()) {
        outf_stream.close();
        std::cerr << "Error: unable to save Cell Linked Lists to CSV. File could not be open." << std::endl;
        return;
    }
    for (int k = 0; k < atoms_count_z; ++k) {
        outf_stream << "\"Layer #" << k + 1 << "\"" << std::endl;
        for (int j = 0; j < atoms_count_y; ++j) {
            for (int i = 0; i < atoms_count_x; ++i) {
                std::list<Atom> cell_list = atom_grid[i][j][k];
                outf_stream << "\"";

                if(!cell_list.empty()) {
                    for(auto cell_atom_it = cell_list.begin(); cell_atom_it != cell_list.end(); cell_atom_it++) {
                        std::string atom_info = cell_atom_it->toString();
                        outf_stream << atom_info.c_str() << ";" << std::endl;
                    }
                }
                else {
                    outf_stream << "EMPTY";
                }

                outf_stream << "\", ";
            }
            outf_stream << std::endl;
        }
        outf_stream << std::endl;
    }

    outf_stream.close();
}

void pmols::CellLinkedLists::repMol(int molIdx, pmols::Molecule &mol) {
    int idx_x, idx_y, idx_z;
    Atom atom;

    // remove from grid atoms of mol with molIdx
    for (int i = 0; i < mols[molIdx].AtomsCount(); ++i) {
        atom = mols[molIdx].GetAtom(i);
        std::tie(idx_x, idx_y, idx_z) = getCellIndex(atom);

        for(auto na = atom_grid[idx_x][idx_y][idx_z].begin(); na != atom_grid[idx_x][idx_y][idx_z].end(); na++) {
            if(na->parent_mol_id == atom.parent_mol_id) {
                atom_grid[idx_x][idx_y][idx_z].erase(na);
                na--;
            }
        }
    }

    // add mol atoms to grid
    for (int i = 0; i < mol.AtomsCount(); ++i) {
        atom = mol.GetAtom(i);
        atom.parent_mol_id = molIdx;

        std::tie(idx_x, idx_y, idx_z) = getCellIndex(atom);
        atom_grid[idx_x][idx_y][idx_z].push_back(atom);
    }

    // replace mol with molIdx in mols vector
    mols[molIdx] = mol;
}

pmols::CLLNeighbourCells::CLLNeighbourCells(CellLinkedLists *cellLinkedLists, Atom &atom) {
    this->clLists = cellLinkedLists;
    this->initial_atom = atom;

    reset();
}

void pmols::CLLNeighbourCells::reset() {
    trans_code = 0;
    std::tie(i0, j0, k0) = clLists->getCellIndex(initial_atom);
}

std::list<pmols::Atom> pmols::CLLNeighbourCells::next() {
    int x;
    int bit_counter;
    char trans_code_bits[6] = {0};
    char left, right, up, down, far, near;
    int i, j, k;

    do {
        x = trans_code;
        bit_counter = 5;

        while (x > 0) {
            trans_code_bits[bit_counter--] = (char) (x % 2);
            x /= 2;
        }
        for (int l = bit_counter; l >= 0; --l) {
            trans_code_bits[l] = 0;
        }

        left = trans_code_bits[0];
        right = trans_code_bits[1];
        up = trans_code_bits[2];
        down = trans_code_bits[3];
        far = trans_code_bits[4];
        near = trans_code_bits[5];

        i = i0 + right - left;
        j = j0 + up - down;
        k = k0 + near - far;

        if(trans_code == 11 || trans_code == 27)
            trans_code += 5;
        else if(trans_code % 4 == 3)
            trans_code += 2;
        else
            trans_code += 1;
    } while (i < 0 || i >= clLists->atoms_count_x ||
             j < 0 || j >= clLists->atoms_count_y ||
             k < 0 || k >= clLists->atoms_count_z);

    std::list<Atom> cell_list = clLists->atom_grid[i][j][k];

    return cell_list;
}

bool pmols::CLLNeighbourCells::hasNext() {
    return trans_code < 43;
}
