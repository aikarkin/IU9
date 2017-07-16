#include <fstream>
#include <iostream>
#include <memory>
#include <boost/program_options.hpp>
<<<<<<< HEAD
#include <ctime>
=======
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
#include <conf_parser.h>

namespace po = boost::program_options;

<<<<<<< HEAD
const float EC_DIST_6 = 0.015625f;
const float EC_DIST_12 = 0.000244141f;

std::string dateTimeNow() {
    time_t _tm = time(NULL );

    struct tm * curtime = localtime ( &_tm );
    return std::string(asctime(curtime));
}


void printStatistics(pmols::HJStatistics &stat, std::ostream &outs) {
    glm::vec3 appos_point;
    std::tuple<float, float, float> box_size;

    outs << "Number of packed molecules: " << stat.PackedMolsNumber() << std::endl;
    std::tie(appos_point, box_size) = stat.GetBoundingBox();
    outs << "Result bounding box: apposition point - " << vec_to_string(appos_point)
         << ", length - " << std::get<0>(box_size)
         << ", width - " << std::get<1>(box_size)
         << ", height - " << std::get<2>(box_size) << std::endl;

=======
void printStatistics(pmols::HJStatistics &stat, std::ostream &outs) {
    outs << "Number of packed molecules: " << stat.PackedMolsNumber() << std::endl;
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
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
<<<<<<< HEAD
    outs << "Number of false alarm pattern search iterations: " << stat.PSFalsePositiveIterationsNumber() << std::endl;
=======
    outs << "Number of false alarm of pattern search: " << stat.PSFalsePositiveIterationsNumber() << std::endl;
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
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
<<<<<<< HEAD
            dist_6 = EC_DIST_6;
            dist_12 = EC_DIST_12;
=======
            dist_6 = 0.015625f;
            dist_12 = 0.000244141f;
            
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
        }
        else {
            dist_6 = std::pow((a->vdw_radius + b->vdw_radius)/(float)glm::distance(a->coord, b->coord), 6.f);
            dist_12 = std::pow(dist_6, 2.f);
        }
        
<<<<<<< HEAD
        return lj_eps_4 * std::abs(dist_12 - dist_6);
    };

    pmols::HJPacker packer(params);    
    packer.Pack();
    packer.Save();
    pmols::HJStatistics pack_stat = packer.GetStatistics();
    //std::ofstream logs("../logs/stat__" + dateTimeNow() + ".log");
    printStatistics(pack_stat, std::cout);

    //logs.close();
=======
        return lj_eps_4 * (dist_12 - dist_6);
    };

    // std::cout << params.toString() << std::endl;
    pmols::HJPacker packer(params);
    packer.Pack();
    packer.Save();
    pmols::HJStatistics pack_stat = packer.GetStatistics();
    std::cout << "Molecules have succesfully packed" << std::endl;
    std::cout << "_________________________________" << std::endl;
    printStatistics(pack_stat, std::cout);

>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
    return 0;
}
