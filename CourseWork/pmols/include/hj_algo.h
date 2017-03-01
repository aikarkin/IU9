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
        float box_length = 0.f;    // (0)  !!!
        float box_width = 0.f;     // (1)  !!!
        float box_height = 0.f;    // (2)  !!!
        float expansivity = 1.f;   // (3)

        // initial step in matched directions
        float step_x = 1.f;         // (4)
        float step_y = 1.f;         // (5)
        float step_z = 1.f;         // (6)
        float step_alpha = 90.f;    // (7)
        float step_betta = 90.f;    // (8)
        float step_gamma = 90.f;    // (9)

        // Hooke Jeeves pattern search step coefficient
        float lambda = 2.0;             // (10)
        float step_coefficient = 0.25f; // (11)

        // precision of Hooke Jeeves algorithm
        float trans_eps = 0.1f;     // (12)
        float rot_eps = 0.1f;       // (13)
        float lj_epsilon = 0.648f;  // (14)
        float cell_length = 0.0f;   // (15) !!!

        // input/output file paths and output file format
        std::string substance = "";   // (16) !!
        std::string mol_file = "";    // (17) !!!
        std::string out_file = "";    // (18) !!!
        std::string out_format = "";  // (19) !!!
        DistFunc distanceFunc;        // (20)
        std::string log_dir = "";     // (21)

        std::string toString() {
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
                      << "\tcell_length: " << cell_length << "; \n"
                      << "\n}";
            return ss.str();
        }
        
        bool val_setted[21] = {0};
    };

    class HJStatistics {
    public:
        HJStatistics(CellLinkedLists *cellLinkedLists, int expSearchIt, int ptrSearchIt, int ptrSearchFpIt, std::tuple<float, float, float> boxSize);
        int ESIterationsNumber() { return es_it; }
        int PSIterationsNumber() {return ps_it; }
        int PSFalsePositiveIterationsNumber() { return ps_fp_it; }
        int TotalIterationsNumber() { return it; }

        int EmptyCellsNumber() { return empty_cells; }
        int IntersectionsNumber() {return inters; }
        int PackedMolsNumber() { return mols_count; }

        float TotalAtomDistance() { return atoms_sum; }
        float AvgAtomDistance() { return avg_atoms_sum; }
        float MinAtomDistance() { return min_atoms_dist; }
        float MaxAtomDistance() { return max_atoms_dist; }

        float TotalIntersection() { return inter_sum; }
        float AvgIntersection() { return avg_inter; }
        float MinIntersection() { return min_inter; }
        float MaxIntersection() { return max_inter; }
    private:
        void calcMolStatistics(CellLinkedLists *cll);
        float atomsEDist(Atom &a, Atom &b);

        int es_it;
        int ps_it;
        int ps_fp_it;
        int it;

        int empty_cells;
        int inters;
        int mols_count;


        float atoms_sum;
        float avg_atoms_sum;
        float max_atoms_dist;
        float min_atoms_dist;

        float inter_sum;
        float avg_inter;
        float min_inter;
        float max_inter;

        std::tuple<float, float, float> box_size;
    };

    class HJPacker {
    public:
        HJPacker(HJParams params);
        void Pack();
        void Save(std::string out_file);
        void Save();
        HJStatistics GetStatistics();
    private:
        int exploringSearch(ublas::vector<float> &x_, bool change_step);
        int patternSearch(ublas::vector<float> &x1_, ublas::vector<float> &x2_);
        void init();
        float eps(int coord_number);
        float objectiveFunc();
        float totalDist;
        float prevTotalDist;

        std::shared_ptr<CellLinkedLists> cellLinkedLists;
        glm::vec3 appos_point;
        HJParams params;

        ublas::vector<float> coordVec;
        ublas::vector<float> step;

        int expSearchItCount;
        int ptrSearchItCount;
        int ptrSearchFpItCount;
    };

}