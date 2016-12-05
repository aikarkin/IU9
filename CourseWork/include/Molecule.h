//
// Created by alex on 08.11.16.
//

#ifndef COURSEWORK_MOLECULE_H
#define COURSEWORK_MOLECULE_H

#include <string>
#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <openbabel/obiter.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

std::string vec_to_string(glm::vec3 vec);

struct Colorf {
    float red, green, blue, alpha;
    Colorf(float red_, float green_, float blue_) : red(red_), green(green_), blue(blue_), alpha(1.0) {};
    Colorf(float red_, float green_, float blue_, float alpha_) : red(red_), green(green_), blue(blue_), alpha(alpha_) {};
    Colorf(double red_, double green_, double blue_) :
            red((float)red_), green((float)green_), blue((float)blue_), alpha(1.0) {};
    Colorf(double red_, double green_, double blue_, float alpha_) :
            red((float)red_), green((float)green_), blue((float)blue_), alpha(alpha_) {};
    Colorf(std::vector<double> color) :
            red((float)color[0]), green((float)color[1]), blue((float)color[2]), alpha(1.0) {};
    bool operator==(const Colorf &other);
};

const Colorf BLACK(0.0, 0.0, 0.0);

struct Atom {
    Colorf color;
    std::string symbol;
    float vdw_radius;
    glm::vec3 coord;
    float radius;
    Atom() : color(Colorf(0.0, 0.0, 0.0)) {};
    Atom(const Atom &other) : color(other.color) {
        symbol = other.symbol;
        vdw_radius = other.vdw_radius;
        coord = other.coord;
        radius = other.radius;
    }
    void print();
};

struct Bond {
    Atom *begin;
    Atom *end;
    float length;
    void print();
};

class Molecule {
public:
    Molecule(std::string file_path);
    Molecule(const Molecule &other);
    OpenBabel::OBMol OBMol();
    int AtomsCount();
    int BondsCount();
    Atom &GetAtom(int idx);
    Bond &GetBond(int idx);
    std::string GetFormula();
    glm::vec3 GetBarycenter();
    void rotateX(float alpha);
    void rotateY(float beta);
    void rotateZ(float gamma);
    void rotateOn(glm::vec3, float, glm::vec3);
    void translate(glm::vec3);
    ~Molecule();
private:
    glm::vec3 bar_vec;
    int atoms_count;
    int bonds_count;
    OpenBabel::OBMol mol;
    Atom *atoms;
    Bond *bonds;
};


#endif //COURSEWORK_MOLECULE_H
