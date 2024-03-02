#include "ShaderUniforms.h"

ShaderUniforms::ShaderUniforms(): programId(-1), uniformMapping() {

}

void ShaderUniforms::SetProgramId(GLuint programId) {
	this->programId = programId;
}

void ShaderUniforms::EnsureUniformCached(const std::string& name) {
	if (uniformMapping.count(name) == 0) {
		uniformMapping[name] = glGetUniformLocation(programId, name.c_str());
	}
}

void ShaderUniforms::SetInt(const std::string& name, int value) {
	EnsureUniformCached(name);
	glUniform1i(uniformMapping[name], value);
}

void ShaderUniforms::SetFloat(const std::string& name, float value) {
	EnsureUniformCached(name);
	glUniform1f(uniformMapping[name], value);
}

void ShaderUniforms::SetVec3(const std::string& name, const glm::vec3& value) {
	EnsureUniformCached(name);
	glUniform3f(uniformMapping[name], value.x, value.y, value.z);
}

void ShaderUniforms::SetMat4(const std::string& name, const glm::mat4& value) {
	EnsureUniformCached(name);
	glUniformMatrix4fv(uniformMapping[name], 1, GL_FALSE, &value[0][0]);
}

void ShaderUniforms::SetMaterialLighting(
	const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float specularMult) {
	EnsureUniformCached("ambient");
	EnsureUniformCached("diffuse");
	EnsureUniformCached("specular");
	EnsureUniformCached("specularMult");

	glUniform3f(uniformMapping["ambient"], ambient.x, ambient.y, ambient.z);
	glUniform3f(uniformMapping["diffuse"], diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(uniformMapping["specular"], specular.x, specular.y, specular.z);

	glUniform1f(uniformMapping["specularMult"], specularMult);
}

void ShaderUniforms::SetDirectionalLighting(
	const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse) {
	EnsureUniformCached("dLightDirection");
	EnsureUniformCached("dLightAmbient");
	EnsureUniformCached("dLightDiffuse");

	glUniform3f(uniformMapping["dLightDirection"], direction.x, direction.y, direction.z);
	glUniform3f(uniformMapping["dLightAmbient"], ambient.x, ambient.y, ambient.z);
	glUniform3f(uniformMapping["dLightDiffuse"], diffuse.x, diffuse.y, diffuse.z);
}

void ShaderUniforms::SetPointOneLighting(
	const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse) {
	EnsureUniformCached("pLightPosition");
	EnsureUniformCached("pLightAmbient");
	EnsureUniformCached("pLightDiffuse");

	glUniform3f(uniformMapping["pLightPosition"], position.x, position.y, position.z);
	glUniform3f(uniformMapping["pLightAmbient"], ambient.x, ambient.y, ambient.z);
	glUniform3f(uniformMapping["pLightDiffuse"], diffuse.x, diffuse.y, diffuse.z);
}