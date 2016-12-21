//
// Created by alex on 18.12.16.
//

#ifndef COURSEWORK_HOOKEJEEVESOPRIMIZATION_H
#define COURSEWORK_HOOKEJEEVESOPRIMIZATION_H

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>


namespace ublas = boost::numeric::ublas;
typedef std::function<float(ublas::vector<float>&,int)> OFuncCallback;

ublas::vector<float> &HookeJeevesOptimize(ublas::vector<float> &coord, std::vector<float> &displacement,
                                          std::vector<float> &epsilon, OFuncCallback &func);

#endif //COURSEWORK_HOOKEJEEVESOPRIMIZATION_H
