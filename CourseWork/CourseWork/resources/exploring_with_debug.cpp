int pmols::HJPacker::exploringSearch(ublas::vector<float> &x_, bool change_step) {
    std::cout << "EXPLORING SEARCH (with " << (change_step ? "step change" : "no step change") << ")" << std::endl;
    std::cout << " —— objective func before iteration: " << totalDist << std::endl;
    float sum1, sum2;
    int mol_idx, op_num;
    float dist0, dist;
    bool changed = false;
    bool terminate = true;
    bool mol_moved;
    float max_step_rot = 0, max_step_trans = 0;
    float min_step_rot = params.step_alpha, min_step_trans = params.step_x;
    std:: cout << "  prevTotalDist: " << prevTotalDist << std::endl;
    std:: cout << "  totalDist: " << totalDist << std::endl;

    for (int i = 0; i < x_.size(); ++i) {
        // std::cout << "\t——— coord num: " << i << std::endl;

        if(i % 6 < 3) {
            if(step[i] > max_step_trans)
                max_step_trans = step[i];
            else if(step[i] < min_step_trans)
                min_step_trans = step[i];
        }
        else {
            if (step[i] > max_step_rot)
                max_step_rot = step[i];
            else if(step[i] < min_step_rot)
                min_step_rot = step[i];
        }

        if(step[i] < eps(i))
            continue;

        if(terminate)
            terminate = false;

        mol_idx = i / 6;
        op_num = i % 6;
        dist0 = cellLinkedLists->MolDist(mol_idx);
        // std::cout << "\t\tdist0: " << dist0 << std::endl;


        mol_moved = cellLinkedLists->MoveMol(mol_idx, (MOVE_OP)op_num, step[i]);
        dist = cellLinkedLists->MolDist(mol_idx);
        sum1 = mol_moved ? ( totalDist - dist0 + dist ) : totalDist;
        if(mol_moved) {
            // std::cout << "\t\tsum1: " << sum1 << std::endl;
            // std::cout << "\t\tdist: " << dist << std::endl;
            cellLinkedLists->CancelMove();
        }
//        if(sum1 < 0) {
//            std::cout << "\t——— Negative value of sum." << std::endl;
//            std::cout << "\t\tcoord number: " << i << std::endl;
//            std::cout << "\t\tsum1: " << sum2 << std::endl;
//            std::cout << "\t\tsum2: " << sum2 << std::endl;
//            std::cout << "\t\tprevTotalSum: " << prevTotalDist << std::endl;
//            std::cout << "\t\ttotalSum: " << totalDist << std::endl;
//            std::cout << "\t\tdist0: " << dist0 << std::endl;
//            std::cout << "\t\tdist: " << dist << std::endl;
//            std::cout << "\t\tmol_moved: " << mol_moved << std::endl;
//        }
        

        mol_moved = cellLinkedLists->MoveMol(mol_idx, (MOVE_OP)op_num, -step[i]);
        dist = cellLinkedLists->MolDist(mol_idx);
        sum2 = mol_moved ? ( totalDist - dist0 + dist ) : totalDist;
        if(mol_moved) {
            // std::cout << "\t\tsum2: " << sum2 << std::endl;
            // std::cout << "\t\tdist: " << dist << std::endl;
            cellLinkedLists->CancelMove();
        }

//        if(sum2 < 0) {
//            std::cout << "\t——— Negative value of sum." << std::endl;
//            std::cout << "\t\tcoord number: " << i << std::endl;
//            std::cout << "\t\tsum1: " << sum1 << std::endl;
//            std::cout << "\t\tsum2: " << sum2 << std::endl;
//            std::cout << "\t\tprevTotalSum: " << prevTotalDist << std::endl;
//            std::cout << "\t\ttotalSum: " << totalDist << std::endl;
//            std::cout << "\t\tdist0: " << dist0 << std::endl;
//            std::cout << "\t\tdist: " << dist << std::endl;
//            std::cout << "\t\tmol_moved: " << mol_moved << std::endl;
//        }
//        if(sum1 < 0 || sum2 < 0)
//            std::cout << std::endl;


        if (prevTotalDist <= std::min(sum1, sum2) && change_step)
            step[i] *= params.step_coefficient;
        else {
            if(sum1 < std::min(prevTotalDist, sum2)) {
                x_[i] += step[i];
                cellLinkedLists->MoveMol(mol_idx, (MOVE_OP)op_num, step[i]);
                totalDist = sum1;
                prevTotalDist = totalDist;
                changed = true;
                // std::cout << "\t\ttotal sum changed to sum1, current value: " << totalDist << std::endl;
            }
            else if(sum2 < std::min(prevTotalDist, sum1)) {
                x_[i] -= step[i];
                cellLinkedLists->MoveMol(mol_idx, (MOVE_OP)op_num, -step[i]);
                totalDist = sum2;
                prevTotalDist = totalDist;
                // std::cout << "\t\ttotal sum changed to sum2, current value: " << totalDist << std::endl;
                changed = true;
            }
        }
        // std::cout << std::endl;
    }
    std::cout << "\tmax rotation step: " << max_step_rot << std::endl;
    std::cout << "\tmax translation step: " << max_step_trans << std::endl << std::endl;

    std::cout << "\tmin rotation step: " << min_step_rot << std::endl;
    std::cout << "\tmin translation step: " << min_step_trans << std::endl;

    std::cout << " —— objective func after iteration: " << totalDist << std::endl;
    if (terminate)
        return -1;

    return (int)changed;
}

