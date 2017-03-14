#include<iostream>
#include <hj_algo.h>
const float EXP = 1.0;

float randomFloat(float a, float b) {
    return a + (float)rand()/(float)(RAND_MAX) * b;
}


void testCellLinkedLists(std::string mol_file, float box_length, float box_width, float box_height) {
    // init CellLinkedLists
    pmols::Molecule mol(mol_file);
    std::tuple<float, float, float> shellSize;

    float shell_length, shell_width, shell_height;
    int atoms_count_x, atoms_count_y, atoms_count_z;
    glm::vec3 appos_point;

    std::tie(appos_point, shellSize) = mol.GetRectangularShell();
    std::tie(shell_length, shell_width, shell_height) = shellSize;

    float exp_length = box_length * EXP;
    float exp_width = box_width * EXP;
    float exp_height = box_height * EXP;

    atoms_count_x = (int)ceilf(exp_length / shell_length);
    atoms_count_y = (int)ceilf(exp_width / shell_width);
    atoms_count_z = (int)ceilf(exp_height / shell_height);

    float max_radius = 0;
    float radius;

    std::cout << "atoms_count: " << atoms_count_x << " " << atoms_count_y << " " << atoms_count_z << std::endl;

    for (int i = 0; i < mol.AtomsCount(); ++i) {
        radius = mol.GetAtom(i).vdw_radius;
        if (radius > max_radius)
            max_radius = radius;
    }

    std::cout << "max atom radius: " << max_radius << std::endl;

    pmols::CellLinkedLists cellLinkedLists(2 * max_radius, [](pmols::Atom *a, pmols::Atom *b) -> float {
        return (a == NULL || b == NULL) ?
               6.0f :
               std::fabs(glm::distance(a->coord, b->coord) - a->vdw_radius - b->vdw_radius);
    });

    float shift_x, shift_y, shift_z;

    for (int i = 0; i < atoms_count_x; ++i) {
        shift_x  = i * shell_length;
        for (int j = 0; j < atoms_count_y; ++j) {
            shift_y = j * shell_width;
            for (int k = 0; k < atoms_count_z; ++k) {
                shift_z = k * shell_height;
                mol.Translate(glm::vec3(shift_x, shift_y, shift_z));
                cellLinkedLists.AddMol(mol);
                mol.Translate(-glm::vec3(shift_x, shift_y, shift_z));
            }
        }
    }

    cellLinkedLists.Update();
    float sum = 0;
    float dist;
    int mol_num;
    bool move_res;
    float rot_a = 5.0f, rot_b = 45.f;
    float trans_a = 0.1f, trans_b = 3.f;
    float rot_x, rot_y, rot_z;
    float trans_x, trans_y, trans_z;

    // calc sum distance between atoms
    for (int i = 0; i < cellLinkedLists.MolsCount(); ++i) {
        sum += cellLinkedLists.MolDist(i);
    }
    std::cout << "initial sum: " << sum << std::endl;

    srand((unsigned int)time(NULL));

    // make 100 random movement operation with cancellation of each ones
    for (int i = 0; i < 100; ++i) {
        // choose random mol
        mol_num = rand() % (unsigned int)cellLinkedLists.MolsCount();
        // choose random move steps
        trans_x = randomFloat(trans_a, trans_b);
        trans_y = randomFloat(trans_a, trans_b);
        trans_z = randomFloat(trans_a, trans_b);

        rot_x = randomFloat(rot_a, rot_b);
        rot_y = randomFloat(rot_a, rot_b);
        rot_z = randomFloat(rot_a, rot_b);

        // translations:
        move_res = cellLinkedLists.MoveMol(mol_num, pmols::TRANS_X, trans_x);
        if(move_res) {
            cellLinkedLists.CancelMove();
        }

        move_res = cellLinkedLists.MoveMol(mol_num, pmols::TRANS_Y, trans_y);
        if(move_res) {
            cellLinkedLists.CancelMove();
        }

        move_res = cellLinkedLists.MoveMol(mol_num, pmols::TRANS_Z, trans_z);
        if(move_res) {
            cellLinkedLists.CancelMove();
        }


        // rotations:
        move_res = cellLinkedLists.MoveMol(mol_num, pmols::ROT_X, rot_x);
        if(move_res) {
            cellLinkedLists.CancelMove();
        }

        move_res = cellLinkedLists.MoveMol(mol_num, pmols::ROT_Y, rot_y);
        if(move_res) {
            cellLinkedLists.CancelMove();
        }

        move_res = cellLinkedLists.MoveMol(mol_num, pmols::ROT_Z, rot_z);
        if(move_res) {
            cellLinkedLists.CancelMove();
        }
    }

    // re-calc total sum of distances between mols
    sum = 0;
    for (int i = 0; i < cellLinkedLists.MolsCount(); ++i) {
        sum += cellLinkedLists.MolDist(i);
    }
    std::cout << "sum after move operations: " << sum << std::endl;


    // 1. Calc Total Sum of all molecules (mol distance test)
    for (int i = 0; i < cellLinkedLists.MolsCount(); ++i) {
        sum += cellLinkedLists.MolDist(i);
    }
    std::cout << std::endl;
    std::cout << "Total sum of distances between all neighbour molecules: " << sum << std::endl;

    std::cout << std::endl;
    dist = cellLinkedLists.MolDist(mol_num);
    std::cout << "initial distance: " << dist << std::endl;
    cellLinkedLists.SaveToCSV("../resources/cell0.csv");
    for (int i = 0; i < cellLinkedLists.GetMol(mol_num)->AtomsCount(); ++i) {
        std::cout << cellLinkedLists.GetMol(mol_num)->GetAtom(i).toString() << "; ";
    }
    std::cout << std::endl;

    move_res = cellLinkedLists.MoveMol(mol_num, pmols::TRANS_Y, -1.2f);
    dist = cellLinkedLists.MolDist(mol_num);
    std::cout << "after move: move_res=" << move_res << "; dist=" << dist << std::endl;

    cellLinkedLists.CancelMove();
    move_res = cellLinkedLists.MoveMol(mol_num, pmols::TRANS_Y, 1.2f);
    dist = cellLinkedLists.MolDist(mol_num);
    std::cout << "distance after cancellation: " << dist << std::endl;
    std::cout << "final atoms coordinates: " << std::endl;
    for (int i = 0; i < cellLinkedLists.GetMol(mol_num)->AtomsCount(); ++i) {
        std::cout << cellLinkedLists.GetMol(mol_num)->GetAtom(i).toString() << "; ";
    }
    cellLinkedLists.SaveToCSV("../resources/cell1.csv");

    std::cout << std::endl;
}


int main() {
    testCellLinkedLists("../resources/Structure3D_CID_962.sdf", 10, 10, 10);

    return 0;
}
