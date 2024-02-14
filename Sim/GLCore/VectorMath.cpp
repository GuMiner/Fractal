#include <string>
#include <igl/per_vertex_normals.h>
#include <igl/per_face_normals.h>
#include <igl/readOFF.h>
#include "../Data/BinaryModel.h"
#include "../Telemetry/Logger.h"
#include "VectorMath.h"

bool VectorMath::ConvertOffToBinaryOff(std::string inputFile, std::string outputFile) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> faces;
    Eigen::MatrixXf V;
    Eigen::MatrixXi F;

    if (!igl::readOFF(inputFile, V, F)) {
        Logger::LogError("Unable to read input file to convert, cannot continue");
        return false;
    }

    for (int i = 0; i < V.rows(); i++) {
        vertices.push_back(glm::vec3(V(i, 0), V(i, 1), V(i, 2)));
    }
    for (int i = 0; i < F.rows(); i++) {
        faces.push_back(glm::ivec3(F(i, 0), F(i, 1), F(i, 2)));
    }

    BinaryModel::Save(outputFile, vertices, faces);

    return true;
}

void VectorMath::ComputeVertexNormals(
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

void VectorMath::ComputeFaceNormals(
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

    igl::per_face_normals(V, F, N);

    for (int i = 0; i < N.rows(); i++) {
        normals.push_back(glm::vec3(N(i, 0), N(i, 1), N(i, 2)));
    }
}