#pragma once

#pragma once
#include <string>
#include <unordered_map>
#include "glew/include/GL/glew.h"
#include "MathGeoLib/include/Math/float4x4.h"
struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

enum class ShaderType {
	NONE = -1, VERTEX = 0, FRAGMENT = 1
};

class Shader {

private:
	std::string m_FilePath;
	unsigned int m_RendererID;

	//chaching uniforms
	std::unordered_map <std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const float4x4& matrix);

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);

private:
	float4x4 matrix = float4x4::identity;
};