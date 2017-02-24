//
// Created by alex on 18.12.16.
//

#include "pack_optimization.h"

#define HJ_LAMBDA 2
#define HJ_DSTEP 8.0f

class HJOptimizationHelper {
public:
    HJOptimizationHelper(std::vector<float> &displacement,
                   std::vector<float> &epsilon, pmols::OFuncCallback &func) :
            disp(displacement), eps(epsilon), F(func)
            { }

    int exploringSearch(ublas::vector<float> &x) {
        float f0, f1, f2, x0;
        bool changed = false;
        bool terminate = true;

        for (int i = 0; i < x.size(); ++i) {
            if(terminate && disp[i] >= eps[i])
                terminate = false;

            x0 = x[i];

            f0 = F(x, i); // on previous step all constraints was observed

            x[i] = x0 + disp[i];
            f1 = F(x, i);

            x[i] = x0 - disp[i];
            f2 = F(x, i);

            if((f0 < f1 && f0 < f2) || (f1 < 0 && f2 < 0)) {
                disp[i] /= HJ_DSTEP;
                x[i] = x0;
                if(!changed)
                    changed = true;
            }
            else if(f1 >= 0 && f1 < f0 && (f1 < f2 || f2 < 0)) {
                x[i] = x0 + disp[i];
                //F(x, i);
            }
            else if(f2 >= 0 && f2 < f0 && (f2 < f1 || f1 < 0)) {
                x[i] = x0 - disp[i];
                //F(x, i);
            }
        }

        if(terminate)
            return -1;
        if(!changed)
            return 0;
        return 1;
    }

    int patternSearch(ublas::vector<float> &x1, ublas::vector<float> &x2) {
        ublas::vector<float> x3 = x1 + HJ_LAMBDA * (x2 - x1);
        int exp_res = exploringSearch(x3);

        if(exp_res > 0) {
            x1 = std::move(x2);
            x2 = std::move(x3);
        }
        else if(exp_res == 0) {
            x1 = x2;
        }

        return exp_res;
    }

private:
    std::vector<float> &disp;
    std::vector<float> &eps;
    pmols::OFuncCallback &F;
};


ublas::vector<float> pmols::HookeJeevesOptimize(ublas::vector<float> &coord, std::vector<float> &displacement,
                                         std::vector<float> &epsilon, pmols::OFuncCallback &func) {
    HJOptimizationHelper hj_helper(displacement, epsilon, func);

    ublas::vector<float> x1 = coord;
    ublas::vector<float> x2(coord.size());
    int exp_res = 0;

    while(exp_res >=0 ) {
        if(exp_res == 0) {
            x2 = x1;
            exp_res = hj_helper.exploringSearch(x2);
        }
        else {
            exp_res = hj_helper.patternSearch(x1, x2);
        }
    }

    return x2;
}