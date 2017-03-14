//
// Created by alex on 25.02.17.
//

#include<iostream>
#include<ctime>
#include<conf_parser.h>

/* print vector of statistics to outs */
void printStatisticsVector(pmols::HJStatistics &stat, std::ostream &outs) {
    glm::vec3 appos_point;
    std::tuple<float, float, float> box_size;
    std::tie(appos_point, box_size) = stat.GetBoundingBox();

    outs << "\"" << stat.PackedMolsNumber() << "\"" << ", "         // number of packed mols (0)
         << "\"" << vec_to_string(appos_point) << "\"" << ", "      // apposition point of bounding box (1)

         << "\"" << std::get<0>(box_size) << "x" << std::get<1>(box_size) << "x" << std::get<0>(box_size) << "\"" << ", "
                                                                    // size of bounding box (2)

         << "\"" << stat.TotalAtomDistance() << "\"" << ", "        // total distance between atoms (3)
         << "\"" << stat.AvgAtomDistance() << "\"" << ", "          // average distance between atoms (4)

         << "\"" << stat.IntersectionsNumber() << "\"" << ", "      // total number of intersections between atoms (5)
         << "\"" << stat.TotalIntersection() << "\"" << ", "        // total value of intersections  ... (6)
         << "\"" << stat.AvgIntersection() << "\"" << ", "          // average value of intersections ... (7)
         << "\"" << stat.MaxIntersection() << "\"" << ", "          // max value of intersections ... (8)

         << "\"" << stat.EmptyCellsNumber() << "\"" << ", "         // number of empty cells ... (9)

         << "\"" << stat.TotalIterationsNumber() << "\"" << ", "    // total number of iterations (10)
         << "\"" << stat.ESIterationsNumber() << "\"" << ", "       // number of exploring search iterations (11)
         << "\"" << stat.PSIterationsNumber() << "\"" << ", "       // number of pattern search iterations (12)
         << "\"" << stat.PSFalsePositiveIterationsNumber() << "\""; // number of false positive iterations of pattern search (13)
}

int main(int argc, const char *argv[]) {
    pmols::HJParams params;
    std::string conf_file;

    // parse command line arguments
    CommandLineParser parser(argc, argv);
    parser.Parse();
    params = std::get<1>(parser.GetParseResult());

    prepareHJParams(params);

    float lj_eps_4 = 4.0f * params.lj_epsilon;
    float dist_6_0 = 0.015625f;
    float dist_12_0 = 0.0002441f;

    params.distanceFunc = [lj_eps_4, dist_6_0, dist_12_0] (pmols::Atom *a, pmols::Atom *b) -> float {
        float dist_6, dist_12;

        if (a == NULL || b == NULL) {
            dist_6 = dist_6_0;
            dist_12 = dist_12_0;

        }
        else {
            dist_6 = std::pow((a->vdw_radius + b->vdw_radius)/(float)glm::distance(a->coord, b->coord), 6.f);
            dist_12 = std::pow(dist_6, 2.f);
        }

        return lj_eps_4 * std::abs(dist_12 - dist_6);
    };

    pmols::HJPacker packer(params);
    packer.Pack();
    packer.Save();
    pmols::HJStatistics pack_stat = packer.GetStatistics();
    printStatisticsVector(pack_stat, std::cout);

    return 0;
}