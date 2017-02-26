//
// Created by alex on 24.02.17.
//
#include <iostream>
#include <tuple>
#include <cell_linked_lists.h>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#ifndef COURSEWORK_HJ_ALGO_H
#define COURSEWORK_HJ_ALGO_H

#endif //COURSEWORK_HJ_ALGO_H

namespace ublas = boost::numeric::ublas;

namespace pmols {
    // WARNING: don't change order of field, it used in parsing
    struct HJParams {
        // size of box to be packed
        float box_length; // 0
        float box_width; // 1
        float box_height; // 2
        float expansivity; // 3

        // initial step in matched directions
        float step_x; // 4
        float step_y; // 5
        float step_z; // 6
        float step_alpha; // 7
        float step_betta; // 8
        float step_gamma; // 9

        // Hooke Jeeves pattern search step coefficient
        float lambda; //10
        float step_coefficient; // 11

        // precision of Hooke Jeeves algorithm
        float trans_eps; // 12
        float rot_eps; // 13
        float lj_epsilon; // 14
        float lj_sigma; // 15
        float cell_length; // 16

        // input/output file paths and output file format
        std::string substance;
        std::string mol_file;
        std::string out_file;
        std::string out_format;
        std::string distance_func;

        std::string to_string() {
            std::stringstream ss;
            ss        << "HJParams{\n"
                      << "\tbox_length:" << box_length << "; \n"
                      << "\tbox_width:" << box_width << "; \n"
                      << "\tbox_height:" << box_height << "; \n"
                      << "\tstep_x:" << step_x << "; \n"
                      << "\tstep_y:" << step_y << "; \n"
                      << "\tstep_z:" << step_z << "; \n"
                      << "\tstep_alpha:" << step_alpha << "; \n"
                      << "\tstep_betta:" << step_betta << "; \n"
                      << "\tstep_gamma:" << step_gamma << "; \n"
                      << "\tlambda:" << lambda << "; \n"
                      << "\texpansivity:" << expansivity << "; \n"
                      << "\tstep_coefficient:" << step_coefficient << "; \n"
                      << "\ttrans_eps:" << trans_eps << "; \n"
                      << "\trot_eps:" << rot_eps << "; \n"
                      << "\tmol_file:" << mol_file.c_str() << "; \n"
                      << "\tout_file:" << out_file.c_str() << "; \n"
                      << "\tout_format:" << out_format.c_str() << "; \n"
                      << "\tsubstance: " << substance.c_str() << "; \n"
                      << "\tlj_epsilon: " << lj_epsilon << "; \n"
                      << "\tlj_sigma: " << lj_sigma << "; \n"
                      << "\tcell_length: " << cell_length << "; \n"
                      << "\tdistance_func: " << distance_func.c_str()
                      << "\n}";
            return ss.str();
        }
    };

    class HJPacker {
    public:
        HJPacker(HJParams params);
        void Pack();
        void Save(std::string out_file);
        void Save();
    private:
        int exploringSearch(ublas::vector<float> &x_, bool change_step);
        int patternSearch(ublas::vector<float> &x1_, ublas::vector<float> &x2_);
        void init();
        float eps(int coord_number);
        float objectiveFunc();
        float totalDist;

        std::shared_ptr<CellLinkedLists> cellLinkedLists;
        HJParams params;

        ublas::vector<float> coordVec;
        ublas::vector<float> step;
    };

}