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
#include <boost/tuple/tuple.hpp>

std::string vec_to_string(glm::vec3 vec);

namespace pmols {
    class Molecule;

<<<<<<< HEAD
    // цвет соответствующий модели RGBA, каждая составляющая - целое число от 0 до 1
    struct Colorf {
        float red; // красная составляющая
        float green; // зеленая составляющая
        float blue; // синяя составляющая
        float alpha; // альфа канал

        Colorf(float red_, float green_, float blue_) : red(red_), green(green_), blue(blue_), alpha(1.0) {};

        Colorf(float red_, float green_, float blue_, float alpha_) : red(red_),
                                                                      green(green_),
                                                                      blue(blue_),
=======
    struct Colorf {
        float red, green, blue, alpha;

        Colorf(float red_, float green_, float blue_) : red(red_), green(green_), blue(blue_), alpha(1.0) {};

        Colorf(float red_, float green_, float blue_, float alpha_) : red(red_), green(green_), blue(blue_),
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
                                                                      alpha(alpha_) {};

        Colorf(double red_, double green_, double blue_) :
                red((float) red_), green((float) green_), blue((float) blue_), alpha(1.0) {};

        Colorf(double red_, double green_, double blue_, float alpha_) :
                red((float) red_), green((float) green_), blue((float) blue_), alpha(alpha_) {};

        Colorf(std::vector<double> color) :
                red((float) color[0]), green((float) color[1]), blue((float) color[2]), alpha(1.0) {};

        bool operator==(const Colorf &other);
    };

    struct Atom {
<<<<<<< HEAD
        Atom &operator=(Atom const &other) {
            if(this != &other) {
                color = other.color;
                atomic_number = other.atomic_number;
                symbol = other.symbol;
                vdw_radius = other.vdw_radius;
                coord = other.coord;
                radius = other.radius;
                parent_mol_id = other.parent_mol_id;
                atom_idx = other.atom_idx;
            }
            return *this;
        }
        static int obatom_count;
        static int atoms_count;

        int atomic_number; // атомное число
        int atom_idx; // индекс атома в молекуле
        int atom_id; // уникальный идентификатор атома

        int parent_mol_id; // идентификатор родительсокой молекулы
        Colorf color; // цвет атома, который используется во время визуализации
        std::string symbol; // химическая формула атома
        float vdw_radius; // ван-дер-ваальсов радиус
        glm::vec3 coord; // координаты центра атома
        float radius; // радиус атома, необходимый для визуализации

        Atom() : color(Colorf(0.0, 0.0, 0.0)) {

=======
        static int obatom_count;
        static int atoms_count;

        int atomic_number;
        int atom_idx;
        int atom_id;

        int parent_mol_id;
        Colorf color;
        std::string symbol;
        float vdw_radius;
        glm::vec3 coord;
        float radius;

        Atom() : color(Colorf(0.0, 0.0, 0.0)) {
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
            atoms_count++;
            atom_idx = atoms_count;
            parent_mol_id = 0;
        };

        Atom(const Atom &other) : color(other.color) {
            atomic_number = other.atomic_number;
<<<<<<< HEAD
//            symbol = other.symbol;
=======
            symbol = other.symbol;
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
            vdw_radius = other.vdw_radius;
            coord = other.coord;
            radius = other.radius;
            parent_mol_id = other.parent_mol_id;
            atom_idx = other.atom_idx;
        }

        std::string toString() {
            std::string atom_info = "{" + std::to_string(parent_mol_id) + ", " + symbol + ", " + vec_to_string(coord) + "}";
            return atom_info;
        }

        bool operator==(const Atom &other);

        OpenBabel::OBAtom OBAtom() {
            OpenBabel::OBAtom atom;
            atom.SetVector(coord.x, coord.y, coord.z);
<<<<<<< HEAD
=======
//            atom.SetType(symbol);
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
            obatom_count++;
            atom_id = obatom_count;
            atom.SetIdx(obatom_count);
            atom.SetId((unsigned int) (obatom_count + 1));
            atom.SetAtomicNum(atomic_number);

            return atom;
        }
    };


    struct Bond {
        Atom *begin;
        Atom *end;
        float length;
    };

    class Molecule {
    public:
        Molecule(std::string file_path);
        Molecule(const Molecule &other);
        Molecule &operator=(Molecule const &other);
        OpenBabel::OBMol OBMol();
        int AtomsCount();
        int BondsCount();
        Atom &GetAtom(int idx);
        Bond &GetBond(int idx);
        std::string GetFormula();
        glm::vec3 GetBarycenter();
        void RotateX(float alpha);
        void RotateY(float beta);
        void RotateZ(float gamma);
        void RotateOn(glm::vec3 point, float angle, glm::vec3 dir);
        void Translate(glm::vec3 v);
        std::tuple<glm::vec3, std::tuple<float, float, float>> GetRectangularShell();
        ~Molecule();
    private:
        glm::vec3 bar_vec;
        int atoms_count;
        int bonds_count;
<<<<<<< HEAD
        std::string formula;
        Atom *atoms; 
=======
        OpenBabel::OBMol mol;
        Atom *atoms;
>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
        Bond *bonds;
    };

    bool AtomInsideBox(Atom atom, glm::vec3 apposPoint, std::tuple<float, float, float> boxSize);
    bool MolInsideBox(Molecule mol, glm::vec3 apposPoint, std::tuple<float, float, float> boxSize);
<<<<<<< HEAD
    bool MolPartlyInsideBox(Molecule mol, glm::vec3 appos_point, std::tuple<float, float, float> boxSize);
=======

>>>>>>> 038e334a388126d42b0fb0c2c05aa260f5dd3043
}
#endif //COURSEWORK_MOLECULE_H