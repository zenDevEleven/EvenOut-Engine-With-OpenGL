#pragma once

namespace Engine {
	class ShaderRenderer
	{
	public:
		static void CreateVertexShader();
		static void CreateFragmentShader();
		static void CreateProgram();

		static void Bind();
		static void UnBind();

		static void SetMat4(const std::string& name, const glm::mat4& matrix);
		static void SetInt(const std::string& name, int value);
		static void SetIntArray(const std::string& name, int* values, uint32_t count);
		static inline GLuint GetShaderProgram() { return shaderProgram; }


	private:
		static std::string readFromFile(const GLchar* path);
		static GLuint vertexShader;
		static GLuint fragmentShader;
		static GLuint shaderProgram;


	};
}


