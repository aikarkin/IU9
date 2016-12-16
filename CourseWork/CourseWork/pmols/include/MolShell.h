//
// Created by alex on 03.12.16.
//

#ifndef COURSEWORK_MOLPACKER_H
#define COURSEWORK_MOLPACKER_H
#include <Molecule.h>
#include <functional>

// packs mol to a single cell
class IMolShell {
public:
    IMolShell() {}

    IMolShell(Molecule *mol) {};
    virtual void pack() = 0;
    virtual void translate(glm::vec3 vec) = 0;
    virtual Molecule getMolecule() = 0;
    virtual std::vector<glm::vec3> getShellPoints() = 0;
    virtual std::vector<glm::vec3> getDisplacementVectors() = 0; /* !!! size of returned vector should be 3 (displacement in 3d directions) */
    virtual glm::vec3 getAppositionPoint() = 0;
};

struct Plane {
    glm::vec3 C_point;
    glm::vec3 N_vec;
    glm::vec3 n_vec;
    float distanceFrom(glm::vec3 p) {
        return std::abs(glm::dot(p - C_point, N_vec))/glm::length(N_vec);
    }
    void normalize() {
        n_vec = glm::normalize(N_vec);
    }
    bool intersectsWithSphere(glm::vec3 center, float radius) {
        return distanceFrom(center) < radius;
    }
    Plane(glm::vec3 point, glm::vec3 normal_vec) : C_point(point), N_vec(normal_vec), n_vec(glm::normalize(normal_vec)) {}
    Plane(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
        C_point = p1;
        N_vec = glm::cross(p2 - p1, p3 - p1);
        n_vec = glm::normalize(N_vec);
    }
    Plane() : C_point(0, 0, 0), N_vec(1, 1, 1), n_vec(glm::normalize(N_vec)) {}
};


class MolCubeShell : public IMolShell {
public:
    typedef std::function<bool(float, float)> compareCubeEdgeLes;

    MolCubeShell(Molecule *molecule);
    MolCubeShell(Molecule *molecule, int partition);
    void pack();
    void pack(compareCubeEdgeLes compFunc);
    void translate(glm::vec3 vec);
    Molecule getMolecule();
    std::vector<glm::vec3> getShellPoints();
    std::vector<glm::vec3> getDisplacementVectors();
    glm::vec3 getAppositionPoint();

private:
    Atom &atom_a();
    Atom &atom_b();
    Atom &atom_c();

    float get_shell_len_a();
    float get_shell_len_b();
    float get_shell_len_c();

    void initShell();
    void superposePlanesWithAxis();
    void returnToOriginState();
    bool intersectsWithSpheres();

    Molecule *mol;

    bool packed;
    int part;

    Plane plane_bottom;
    Plane plane_front;
    Plane plane_left;

    glm::vec3 A;
    glm::vec3 B;
    glm::vec3 C;
    glm::vec3 M;

    glm::vec3 v_rot_alpha;
    glm::vec3 v_rot_beta;
    glm::vec3 v_rot_gamma;

    float cube_edge_len;
    std::vector<glm::vec3> displacementVectors;
};


#endif //COURSEWORK_MOLPACKER_H
