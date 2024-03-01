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

void ShaderUniforms::SendMat4(const std::string& name, const glm::mat4& value) {
	EnsureUniformCached(name);
	glUniformMatrix4fv(uniformMapping[name], 1, GL_FALSE, &value[0][0]);
}