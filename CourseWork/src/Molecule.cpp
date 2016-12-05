//
// Created by alex on 08.11.16.
//

#include "../include/Molecule.h"
#include <cmath>
#include <openbabel/math/vector3.h>

std::string vec_to_string(glm::vec3 vec) {
    return "(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")";
}

Molecule::Molecule(std::string file_path) {
    std::cout << "loading molecule" << std::endl;
    OpenBabel::OBConversion conversion;
    conversion.SetInFormat("sdf");
    OpenBabel::OBElementTable table;
    mol = OpenBabel::OBMol();

    conversion.ReadFile(&mol, file_path);
    std::cout << "molecule successfully red" << std::endl;
    std::cout << "molecule's formula: " << mol.GetFormula() << std::endl;

    std::cout << "data allocation ..." << std::endl;
    atoms_count = mol.NumAtoms();
    bonds_count = mol.NumBonds();
    std::cout << "atoms count: " << atoms_count << "\tbonds count: " << bonds_count << std::endl;

    atoms = new Atom[atoms_count];
    bonds = new Bond[bonds_count];

    std::cout << "allocation successfuly finished" << std::endl;

    std::cout << "atoms filling" << std::endl;
    FOR_ATOMS_OF_MOL(atom, mol) {
        int idx = atom->GetIdx() - 1;
        int atomic_num = atom->GetAtomicNum();
        std::cout << "atom #" << idx << "\tatomic number: " << atomic_num << std::endl;
        atoms[idx].color = Colorf(table.GetRGB(atomic_num));
        atoms[idx].symbol = table.GetSymbol(atomic_num);
        atoms[idx].coord =  glm::vec3(atom->GetX(), atom->GetY(), atom->GetZ());
        atoms[idx].vdw_radius = (float)table.GetVdwRad(atomic_num);
        atoms[idx].radius = atoms[idx].vdw_radius / 5.0f;
        atoms[idx].print();
    }
    std::cout << "atoms filling - OK" << std::endl;

    int i = 0;
    std::cout << "bonds filling" << std::endl;
    FOR_BONDS_OF_MOL(bond, mol) {
        int begin_idx = bond->GetBeginAtom()->GetIdx() - 1;
        int end_idx = bond->GetEndAtom()->GetIdx() - 1;

        bonds[i].begin = &atoms[begin_idx];
        bonds[i].end = &atoms[end_idx];
        std::cout << "____________" << std::endl;
        std::cout << "Bond #" << i << std::endl << std::endl;
        std::cout << "begin: " << std::endl;
        bonds[i].begin->print();
        std::cout << "end: " << std::endl;
        bonds[i].end->print();
        std::cout << "____________" << std::endl;
        bonds[i].length = (float)bond->GetLength();
        i++;
    }
    std::cout << "bonds filling - OK" << std::endl;

    bar_vec = GetBarycenter();
}

int Molecule::AtomsCount() {
    return atoms_count;
}

int Molecule::BondsCount() {
    return bonds_count;
}

Atom &Molecule::GetAtom(int idx) {
    if(idx < 0)
        throw std::out_of_range("index can't be negative");
    else if(idx > atoms_count)
        throw std::out_of_range("idx: " + std::to_string(idx) + "\tatoms count: " + std::to_string(atoms_count));
    return atoms[idx];
}

Bond &Molecule::GetBond(int idx) {
    if(idx < 0)
        throw std::out_of_range("index can't be negative");
    else if(idx > bonds_count)
        throw std::out_of_range("idx: " + std::to_string(idx) + "\tatoms count: " + std::to_string(atoms_count));
    return bonds[idx];
}

Molecule::~Molecule() {
    if (atoms != NULL)
        delete []atoms;

    if (bonds != NULL)
        delete []bonds;
}

std::string Molecule::GetFormula() {
    return mol.GetFormula();
}

glm::vec3 Molecule::GetBarycenter() {
    glm::vec3 sum_p(0, 0, 0);

    for (int i = 0; i < atoms_count; ++i) {
        sum_p.x += atoms[i].coord.x;
        sum_p.y += atoms[i].coord.y;
        sum_p.z += atoms[i].coord.z;
    }

    return glm::vec3(sum_p.x/atoms_count, sum_p.y/atoms_count, sum_p.z/atoms_count);
}

void Molecule::rotateX(float alpha) {
    rotateOn(bar_vec, alpha, glm::vec3(1, 0, 0));
}

void Molecule::rotateY(float beta) {
    rotateOn(bar_vec, beta, glm::vec3(0, 1, 0));
}

void Molecule::rotateZ(float gamma) {
    rotateOn(bar_vec, gamma, glm::vec3(0, 0, 1));
}

void Molecule::translate(glm::vec3 v) {
    for (int i = 0; i < atoms_count; ++i) {
        atoms[i].coord.x += v.x;
        atoms[i].coord.y += v.y;
        atoms[i].coord.z += v.z;
    }
    bar_vec.x += v.x;
    bar_vec.y += v.y;
    bar_vec.z += v.z;
}

void Molecule::rotateOn(glm::vec3 point, float angle, glm::vec3 dir) {
    glm::mat4 m_rot(1.0);
    m_rot = glm::rotate(m_rot, angle, dir);

    for (int i = 0; i < atoms_count; ++i) {
        if(atoms[i].coord != point) {
            glm::vec4 cur_vec4(atoms[i].coord.x - point.x, atoms[i].coord.y - point.y, atoms[i].coord.z - point.z, 0);
            cur_vec4 = cur_vec4 * m_rot;
            atoms[i].coord.x = point.x + cur_vec4.x;
            atoms[i].coord.y = point.y + cur_vec4.y;
            atoms[i].coord.z = point.z + cur_vec4.z;
        }
    }
}

Molecule::Molecule(const Molecule &other) {
    // TODO: copy atoms ans bounds arrays
    atoms_count = other.atoms_count;
    bonds_count = other.bonds_count;
    bar_vec = other.bar_vec;
    mol = other.mol;

    atoms = new Atom[atoms_count];
    bonds = new Bond[bonds_count];

    /*memcpy(atoms, molecule.atoms, atoms_count*sizeof(molecule.atoms[0]));
    memcpy(bonds, molecule.bonds, bonds_count*sizeof(molecule.bonds[0]));*/
    for (int i = 0; i < atoms_count; ++i) {
        atoms[i] = other.atoms[i];
    }

    for (int i = 0; i < bonds_count; ++i) {
        bonds[i] = other.bonds[i];
    }
}

OpenBabel::OBMol Molecule::OBMol() {
    OpenBabel::OBAtom *cur_atom;
    for (int i = 0; i < atoms_count; ++i) {
        cur_atom = mol.GetAtom(i + 1);
        cur_atom->SetVector(atoms[i].coord.x, atoms[i].coord.y, atoms[i].coord.z);
    }
    return mol;
}


void Atom::print() {
    std::cout << "color: (" << color.red << ", " << color.green << ", " << color.blue << ")" << std::endl;
    std::cout << "symbol: " << symbol << std::endl;
    std::cout << "vdw_radius: " << vdw_radius << std::endl;
    std::cout << "coord: (" << coord.x << ", " <<  coord.y << ", " << coord.z << ")" << std::endl;
    std::cout << "radius: " << radius << std::endl;
}


void Bond::print() {
    std:: cout << begin->symbol << " -- " << end->symbol << ": " << length << std::endl;
}


bool Colorf::operator==(const Colorf &other) {
    return this->red == other.red && this->blue == other.blue && this->green == other.green;
}

