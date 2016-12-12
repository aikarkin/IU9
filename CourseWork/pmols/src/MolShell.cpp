//
// Created by alex on 03.12.16.
//

#include <MolShell.h>

MolCubeShell::MolCubeShell(Molecule *molecule) : mol(molecule) {
    part = 90;
    initShell();
}

void MolCubeShell::pack() {
    float alpha = 0;
    float beta = 0;
    float gamma = 0;

    float proper_alpha = 0;
    float proper_beta = 0;
    float proper_gamma = 0;

    float d_phi = 45.f/part;
    float cur_len, min_len = 100;
    float a, b, c;

    int i = 0, j = 0, k = 0;

    while(i < part && !intersectsWithSpheres()) {
        mol->rotateOn(A, -alpha, v_rot_alpha);
        alpha = i * d_phi;
        mol->rotateOn(A, alpha, v_rot_alpha);
        j = 0;
        while(j < part && !intersectsWithSpheres()) {
            mol->rotateOn(A, -beta, v_rot_beta);
            beta = -j * d_phi;
            mol->rotateOn(A, beta, v_rot_beta);
            k = 0;
            while(k < part && !intersectsWithSpheres()) {
                mol->rotateOn(A, -gamma, v_rot_gamma);
                gamma  = -k * d_phi;
                mol->rotateOn(A, gamma, v_rot_gamma);

                a = std::max(
                        plane_left.distanceFrom(atom_b().coord) + atom_b().vdw_radius,
                        plane_left.distanceFrom(atom_c().coord) + atom_c().vdw_radius
                );
                b = std::max(
                        plane_bottom.distanceFrom(atom_b().coord) + atom_b().vdw_radius,
                        plane_bottom.distanceFrom(atom_c().coord) + atom_c().vdw_radius
                );
                c = std::max(
                        plane_front.distanceFrom(atom_b().coord) + atom_b().vdw_radius,
                        plane_front.distanceFrom(atom_c().coord) + atom_c().vdw_radius
                );

                cur_len = std::max(a, std::max(b, c));
                if (cur_len < min_len) {
                    min_len = cur_len;
                    proper_alpha = alpha;
                    proper_beta = beta;
                    proper_gamma = gamma;
                }
                k++;
            }
            mol->rotateOn(A, -gamma, v_rot_gamma);
            gamma = 0;
            j++;
        }
        mol->rotateOn(A, -beta, v_rot_beta);
        beta = 0;
        i++;
    }

    returnToOriginState();
    mol->rotateOn(A, proper_alpha, v_rot_alpha);
    mol->rotateOn(A, proper_beta, v_rot_beta);
    mol->rotateOn(A, proper_gamma, v_rot_gamma);

    if (min_len == 100) {
        std::cerr << "error: unable to packMax mol in cube!" << std::endl;
        min_len = -1;
    }

    packed = true;
    cube_edge_len = min_len;
    superposePlanesWithAxis();
}


std::vector<glm::vec3> MolCubeShell::getShellPoints() {
    if(!packed)
        pack();

    glm::vec3 N = M + cube_edge_len * -plane_left.n_vec;
    glm::vec3 Q = M + cube_edge_len * -plane_bottom.n_vec;
    glm::vec3 R = M + cube_edge_len * -plane_front.n_vec;

    std::vector<glm::vec3> cubePoints;

    cubePoints.push_back(R);
    cubePoints.push_back(R + (Q - M));
    cubePoints.push_back(cubePoints.back() + (N - M));
    cubePoints.push_back(cubePoints.back() + (M - Q));

    cubePoints.push_back(N);
    cubePoints.push_back(N + (Q - M));
    cubePoints.push_back(Q);
    cubePoints.push_back(M);

    return cubePoints;
}

Atom &MolCubeShell::atom_a() {
    return mol->GetAtom(1);
}

Atom &MolCubeShell::atom_b() {
    return mol->GetAtom(0);
}

Atom &MolCubeShell::atom_c() {
    return mol->GetAtom(2);
}

bool MolCubeShell::intersectsWithSpheres() {
    return plane_left.intersectsWithSphere(atom_c().coord, atom_c().vdw_radius) ||
           plane_bottom.intersectsWithSphere(atom_c().coord, atom_c().vdw_radius) ||
           plane_front.intersectsWithSphere(atom_c().coord, atom_c().vdw_radius) ||
           plane_left.intersectsWithSphere(atom_b().coord, atom_b().vdw_radius);
}

void MolCubeShell::returnToOriginState() {
    atom_a().coord = A;
    atom_b().coord = B;
    atom_c().coord = C;
}

glm::vec3 rotate_vec(glm::vec3 vec, float angle_in_degrees, glm::vec3 rot_axis) {
    glm::vec4 tmp_vec = glm::vec4(vec.x, vec.y, vec.z, 0);
    glm::mat4 rot_mat(1);
    rot_mat = glm::rotate(rot_mat, angle_in_degrees, rot_axis);
    tmp_vec = tmp_vec*rot_mat;
    return glm::vec3(tmp_vec.x, tmp_vec.y, tmp_vec.z);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "IncompatibleTypes"
void MolCubeShell::superposePlanesWithAxis() {
    glm::vec3 vec_i(1, 0, 0);
    glm::vec3 vec_j(0, 1, 0);
    glm::vec3 vec_k(0, 0, 1);

    float phi = -glm::angle(plane_bottom.n_vec, vec_i);
    glm::vec3 v_rot_phi = (glm::cross(plane_bottom.n_vec, vec_i));

    plane_left.n_vec = rotate_vec(plane_left.n_vec, phi, v_rot_phi);
    plane_left.N_vec = rotate_vec(plane_left.N_vec, phi, v_rot_phi);

    float psi = -(glm::angle(plane_left.n_vec, vec_k));
    glm::vec3 v_rot_psi = glm::cross(plane_left.n_vec, vec_k);

    // planes' rotation
    plane_front.N_vec = rotate_vec(plane_front.N_vec, phi, v_rot_phi);
    plane_front.N_vec = rotate_vec(plane_front.N_vec, psi, v_rot_psi);
    plane_front.normalize();

    plane_left.N_vec = rotate_vec(plane_left.N_vec, psi, v_rot_psi);
    plane_left.normalize();

    plane_bottom.N_vec = rotate_vec(plane_bottom.N_vec, phi, v_rot_phi);
    plane_bottom.N_vec = rotate_vec(plane_bottom.N_vec, psi, v_rot_psi);
    plane_bottom.normalize();

    M = rotate_vec(M, phi, v_rot_phi);
    M = rotate_vec(M, psi, v_rot_psi);

    // rotation vectors
    v_rot_alpha = rotate_vec(v_rot_alpha, phi, v_rot_phi);
    v_rot_alpha = rotate_vec(v_rot_alpha, psi, v_rot_psi);

    v_rot_beta = rotate_vec(v_rot_beta, phi, v_rot_phi);
    v_rot_beta = rotate_vec(v_rot_beta, psi, v_rot_psi);

    v_rot_gamma = rotate_vec(v_rot_gamma, phi, v_rot_phi);
    v_rot_gamma = rotate_vec(v_rot_gamma, psi, v_rot_psi);

    // molecule rotation
    for (int i = 0; i < mol->AtomsCount(); ++i) {
        mol->GetAtom(i).coord = rotate_vec(mol->GetAtom(i).coord, phi, v_rot_phi);
        mol->GetAtom(i).coord = rotate_vec(mol->GetAtom(i).coord, psi, v_rot_psi);
    }
}
#pragma clang diagnostic pop

Molecule MolCubeShell::getMolecule() {
    return *mol;
}

std::vector<glm::vec3> MolCubeShell::getDisplacementVectors() {
    if (displacementVectors.size() == 0) {
        displacementVectors.push_back(cube_edge_len*glm::normalize(plane_front.N_vec));
        displacementVectors.push_back(cube_edge_len*glm::normalize(plane_left.N_vec));
        displacementVectors.push_back(cube_edge_len*glm::normalize(plane_bottom.N_vec));
    }
    return displacementVectors;
}

glm::vec3 MolCubeShell::getAppositionPoint() {
    return M;
}

void MolCubeShell::translate(glm::vec3 vec) {
    M += vec;
    A += vec;
    B += vec;
    C += vec;
    for (int i = 0; i < mol->AtomsCount(); ++i) {
        mol->GetAtom(i).coord += vec;
    }
}

MolCubeShell::MolCubeShell(Molecule *molecule, int partition) : mol(molecule) {
    part = partition;
    initShell();
}

void MolCubeShell::initShell() {
    packed = false;

    if (mol->AtomsCount() != 3) {
        std::cout << "Result: failed" << std::endl;
        std::cerr << "Error: unable to packMax mol. Atoms count should be 3" << std::endl;
        throw;
    }

    A = atom_a().coord;
    B = atom_b().coord;
    C = atom_c().coord;

    v_rot_alpha = B - A;
    v_rot_beta = glm::cross(B - A, C - A);
    v_rot_gamma = glm::cross(v_rot_beta, v_rot_alpha);

    glm::vec3 n_alpha = glm::normalize(v_rot_alpha);
    glm::vec3 n_beta = glm::normalize(v_rot_beta);
    glm::vec3 n_gamma = glm::normalize(v_rot_gamma);


    glm::vec3 A_f = A + atom_a().vdw_radius*n_beta;
    glm::vec3 B_f = B + atom_b().vdw_radius*n_beta;
    glm::vec3 C_f = C + atom_c().vdw_radius*n_beta;


    plane_front = Plane(A_f, B_f, C_f);

    glm::vec3 A_b = A + atom_a().vdw_radius*-n_gamma;
    glm::vec3 B_b = B + atom_b().vdw_radius*-n_gamma;
    glm::vec3 C_b = A_b + atom_a().vdw_radius*plane_front.n_vec;

    plane_bottom = Plane(A_b, B_b, C_b);

    glm::vec3 N_left = glm::cross(plane_bottom.N_vec, plane_front.N_vec);

    M = A + atom_a().vdw_radius * (plane_front.n_vec + plane_bottom.n_vec + glm::normalize(N_left));
    plane_left = Plane(M, N_left);

    plane_front.C_point = M;
    plane_bottom.C_point = M;
}








