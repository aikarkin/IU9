#include <fstream>
#include <iostream>
#include <memory>
#include <boost/program_options.hpp>
#include <ctime>
#include <conf_parser.h>

namespace po = boost::program_options;


std::string dateTimeNow() {
    time_t _tm = time(NULL );

    struct tm * curtime = localtime ( &_tm );
    return std::string(asctime(curtime));
}

void printStatistics(pmols::HJStatistics &stat, std::ostream &outs) {
    outs << "Number of packed molecules: " << stat.PackedMolsNumber() << std::endl;
    outs << "Total sum of distances between molecules: " << stat.TotalAtomDistance() << std::endl;
    outs << std::endl;
    outs << "Average distance between atoms: " << stat.AvgAtomDistance() << std::endl;
    outs << "Minimum distance between atoms: " << stat.MinAtomDistance() << std::endl;
    outs << "Maximum distance between atoms: " << stat.MaxAtomDistance() << std::endl;
    outs << std::endl;
    outs << "Total intersection between atoms: " << stat.TotalIntersection() << std::endl;
    outs << "Number of intersections between atoms: " << stat.IntersectionsNumber() << std::endl;
    outs << "Average intersection between atoms: " << stat.AvgIntersection() << std::endl;
    outs << "Maximum intersection between atoms: " << stat.MaxIntersection() << std::endl;
    outs << "Minimum intersection between atoms: " << stat.MinIntersection() << std::endl;
    outs << std::endl;
    outs << "Number of empty cells: " << stat.EmptyCellsNumber() << std::endl;
    outs << std::endl;
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

    float lj_eps_4 = 4.0f * params.lj_epsilon;
    params.distanceFunc = [lj_eps_4] (pmols::Atom *a, pmols::Atom *b) -> float {
        float dist_6, dist_12;

        if (a == NULL || b == NULL) {
            dist_6 = 0.015625f;
            dist_12 = 0.000244141f;
            
        }
        else {
            dist_6 = std::pow((a->vdw_radius + b->vdw_radius)/(float)glm::distance(a->coord, b->coord), 6.f);
            dist_12 = std::pow(dist_6, 2.f);
        }
        
        return lj_eps_4 * (dist_12 - dist_6);
    };

    pmols::HJPacker packer(params);    
    packer.Pack();
    packer.Save();
    pmols::HJStatistics pack_stat = packer.GetStatistics();
    std::ofstream logs("../logs/stat__" + dateTimeNow() + ".log");
    
    printStatistics(pack_stat, logs);

    logs.close();
    return 0;
}
