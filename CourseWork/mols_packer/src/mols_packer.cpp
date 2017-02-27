#include <fstream>
#include <iostream>
#include <memory>
#include <boost/program_options.hpp>
#include <conf_parser.h>

namespace po = boost::program_options;


int main(int argc, const char *argv[]) {
    pmols::HJParams params;
    std::string conf_file;

    CommandLineParser parser(argc, argv);
    parser.Parse();
    std::tie(conf_file, params) = parser.GetParseResult();

    if(conf_file != "")
        JSONConfigParser::ParseFromFile(params, conf_file);

    prepareHJParams(params);
    params.distanceFunc = [eps = params.lj_epsilon, sigma = params.lj_sigma] (pmols::Atom *a, pmols::Atom *b) -> float {
            float dist = (float)glm::distance(a->coord, b->coord);
            float energy = 4 * eps * (std::pow(sigma/dist, 12.f) - std::pow(sigma/dist, 6.f));

            if (a == NULL || b == NULL)
                return 10.0f;
            else
                return energy;
    };
    std::cout << params.to_string() << std::endl;
    pmols::HJPacker packer(params);

    packer.Pack();
    packer.Save();

    return 0;
}
