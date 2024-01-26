#include <igl/per_vertex_normals.h>
#include "VectorMath.h"

void VectorMath::ComputeNormals(
	std::vector<glm::vec3>& vertices, std::vector<glm::ivec3>& faces,
	std::vector<glm::vec3>& normals) {

    Eigen::MatrixXf V;
    Eigen::MatrixXi F;
    Eigen::MatrixXf N;

    V.resize(vertices.size(), 3);
    F.resize(faces.size(), 3);

    for (int i = 0; i < V.rows(); i++) {
        V(i, 0) = vertices[i].x;
        V(i, 1) = vertices[i].y;
        V(i, 2) = vertices[i].z;
    }
    for (int i = 0; i < F.rows(); i++) {
        F(i, 0) = faces[i].x;
        F(i, 1) = faces[i].y;
        F(i, 2) = faces[i].z;
    }

    igl::per_vertex_normals(V, F, N);

    for (int i = 0; i < N.rows(); i++) {
        normals.push_back(glm::vec3(N(i, 0), N(i, 1), N(i, 2)));
    }
}