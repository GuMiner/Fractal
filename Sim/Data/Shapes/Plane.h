#pragma once
#include <vector>
#include <glm\vec3.hpp>

class Plane {
public:
	// Creates a plane Z+ oriented (visible from Z+ looking towards Z-)
	static void Create(std::vector<glm::vec3>& vertices, float scale);
	static void CreateNormals(std::vector<glm::vec3>& normals);
};