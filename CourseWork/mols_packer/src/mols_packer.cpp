#include <fstream>
#include <iostream>
#include <memory>
#include <boost/program_options.hpp>
#include <conf_parser.h>

namespace po = boost::program_options;

void printStatistics(pmols::HJStatistics &stat, std::ostream &outs) {
    outs << "Number of packed molecules: " << stat.PackedMolsNumber() << std::endl;
    outs << "Total sum of distances between molecules: " << stat.TotalAtomDistance() << std::endl;

    outs << "Average distance between atoms: " << stat.AvgAtomDistance() << std::endl;
    outs << "Minimum distance between atoms: " << stat.MinAtomDistance() << std::endl;
    outs << "Maximum distance between atoms: " << stat.MaxAtomDistance() << std::endl;

    outs << "Total intersection between atoms: " << stat.TotalIntersection() << std::endl;
    outs << "Number of intersections between atoms: " << stat.IntersectionsNumber() << std::endl;
    outs << "Average intersection between atoms: " << stat.AvgIntersection() << std::endl;
    outs << "Maximum intersection between atoms: " << stat.MaxIntersection() << std::endl;
    outs << "Minimum intersection between atoms: " << stat.MinIntersection() << std::endl;

    outs << "Number of empty cells: " << stat.EmptyCellsNumber() << std::endl;

    outs << "Total number of iterations: " << stat.TotalIterationsNumber() << std::endl;
    outs << "Number of exploring search iterations (with step change): " << stat.ESIterationsNumber() << std::endl;
    outs << "Number of pattern search iterations: " << stat.PSIterationsNumber() << std::endl;
    outs << "Number of false alarm of pattern search: " << stat.PSFalsePositiveIterationsNumber() << std::endl;
}

int main(int argc, const char *argv[]) {
    pmols::HJParams params;
    std::string conf_file;

    // parse command line arguments
    CommandLineParser parser(argc, argv);
    parser.Parse();
    std::tie(conf_file, params) = parser.GetParseResult();

    // parse it it specified, configuration file
    if(conf_file != "")
        JSONConfigParser::ParseFromFile(params, conf_file);

    prepareHJParams(params);

    // define Lennard-Jones potential as distance function
    // LJ coefficients
    float lj_a1 = 4 * params.lj_epsilon * std::pow(params.lj_sigma, 12.f);
    float lj_a2 = 4 * params.lj_epsilon * std::pow(params.lj_sigma, 6.f);

    params.distanceFunc = [lj_a1, lj_a2] (pmols::Atom *a, pmols::Atom *b) -> float {
            float dist_6, dist_12;

            // if one of cells is empty, consider actual distance equals 2.0
            if (a == NULL || b == NULL) {
                dist_6 = 64.0f;
                dist_12 = 4096.f;
            }
            else {
                dist_6 = std::pow((float)std::fabs(glm::distance(a->coord, b->coord)), 6.0f);
                dist_12 = std::pow(dist_6, 2.0f);
            }
            return lj_a1/dist_12 - lj_a2/dist_6;
    };
    std::cout << params.to_string() << std::endl;

    // initialize Hooke-Jeeves packer
    pmols::HJPacker packer(params);
    // pack and save result to file
    pmols::HJStatistics before_pack = packer.GetStatistics();
    printStatistics(before_pack, std::cout);
    packer.Pack();
    packer.Save();
    pmols::HJStatistics pack_stat = packer.GetStatistics();
//    std::ofstream stat_file("../logs/ttt/statistics.txt");
    printStatistics(pack_stat, std::cout);

    return 0;
}
