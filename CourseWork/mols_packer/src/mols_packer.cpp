#include <fstream>
#include <iostream>
#include <memory>
#include <boost/program_options.hpp>
#include <conf_parser.h>

namespace po = boost::program_options;


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
                dist_6 = std::pow((float) glm::distance(a->coord, b->coord), 6.0f);
                dist_12 = std::pow(dist_6, 2.0f);
            }
            return lj_a1/dist_12 - lj_a2/dist_6;
    };
    std::cout << params.to_string() << std::endl;

    // initialize Hooke-Jeeves packer
    pmols::HJPacker packer(params);
    // pack and save result to file
    packer.Pack();
    packer.Save();

    return 0;
}
