//
// Created by alex on 26.02.17.
//

#ifndef COURSEWORK_CONF_PARSER_H
#define COURSEWORK_CONF_PARSER_H

#include <boost/program_options.hpp>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <hj_algo.h>
#include <iostream>
#include <iomanip>


std::string floatToString(float num, int precision);
std::tuple<std::string, std::string> parseFileName(std::string fileName);
bool prepareHJParams(pmols::HJParams &params);

class JSONConfigParser{
public:
    static void ParseFromFile(pmols::HJParams &params, std::string confFile);
    JSONConfigParser(pmols::HJParams &params, std::istream &jsonInputStream);
    void Parse();
private:
    void parseLattice();
    void parsePackingParams();
    void parseCellLinkedLists();

    void parseFloats(rapidjson::Value &obj, std::map<std::string, int> &keyToField);
    void setParamByIndex(float val, int idx);

    rapidjson::Document doc;
    std::string conf_file;
    pmols::HJParams& params;
};

class CommandLineParser {
public:
    CommandLineParser(int argc, const char **argv);
    void Parse();
    std::tuple<std::string, pmols::HJParams> GetParseResult();
private:
    void parseFloat(boost::program_options::variables_map vm, std::map<std::string, int> paramNameToFieldIdx);
    pmols::HJParams params;

    int argc;
    const char **argv;
    std::string conf_file;
};

#endif //COURSEWORK_CONF_PARSER_H
