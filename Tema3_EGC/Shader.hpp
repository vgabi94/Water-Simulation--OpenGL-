#pragma once
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL\glew.h>
#include <string>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <glm\fwd.hpp>
#include <glm\gtc\type_ptr.hpp>

#define INFO_BUFF_SIZE 512

namespace glg
{
	class Shader
	{
		friend class ShaderProgram;
	public:
		Shader() { }
		Shader(const GLchar* inGlsl);

		~Shader() {
			if (shader != 0) glDeleteShader(shader);
		}

		virtual void compile();

		virtual void setSource(const GLchar* source) {
			this->source = source;
		}
	protected:
		virtual void createShader() = 0; // Must implement!
		GLboolean compiled = GL_FALSE;
		GLint shader = 0;
		std::string source;
	};
#pragma region Shaders definitions
	class VertexShader : public Shader
	{
	public: 
		using Shader::Shader;
		void createShader() override;
	};

	class FragmentShader : public Shader
	{
	public:
		using Shader::Shader;
		void createShader() override;
	};
#pragma endregion

	class ShaderProgram 
	{
	public:
		ShaderProgram() {
			createProgram();
		}
		ShaderProgram(std::initializer_list<Shader*> shader);
		~ShaderProgram() {
			glDeleteProgram(program);
		}

		void attachShader(Shader& shdr);
		void link();
		void use();
#pragma region Uniform wrappers
		void uniform(const GLchar* name, GLint v0) {
			glUniform1i(getUniformLocation(name), v0);
		}
		void uniform(const GLchar* name, GLfloat v0) {
			glUniform1f(getUniformLocation(name), v0);
		}
		void uniform(const GLchar* name, GLfloat v0, GLfloat v1) {
			glUniform2f(getUniformLocation(name), v0, v1);
		}
		void uniform(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2) {
			glUniform3f(getUniformLocation(name), v0, v1, v2);
		}
		void uniform(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
			glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
		}
		void uniform(const GLchar* name, const glm::vec2& value, GLsizei count = 1) {
			glUniform2fv(getUniformLocation(name), count, glm::value_ptr(value));
		}
		void uniform(const GLchar* name, const glm::vec3& value, GLsizei count = 1) {
			glUniform3fv(getUniformLocation(name), count, glm::value_ptr(value));
		}
		void uniform(const GLchar* name, const glm::vec4& value, GLsizei count = 1) {
			glUniform4fv(getUniformLocation(name), count, glm::value_ptr(value));
		}
		void uniform(const GLchar* name, const glm::mat2& value, GLboolean transpose = GL_FALSE, GLsizei count = 1) {
			glUniformMatrix2fv(getUniformLocation(name), count, transpose, glm::value_ptr(value));
		}
		void uniform(const GLchar* name, const glm::mat3& value, GLboolean transpose = GL_FALSE, GLsizei count = 1) {
			glUniformMatrix3fv(getUniformLocation(name), count, transpose, glm::value_ptr(value));
		}
		void uniform(const GLchar* name, const glm::mat4& value, GLboolean transpose = GL_FALSE, GLsizei count = 1) {
			glUniformMatrix4fv(getUniformLocation(name), count, transpose, glm::value_ptr(value));
		}
#pragma endregion
		GLint getProgram() { return program; }
	private:
		void createProgram();
		GLint getUniformLocation(const GLchar* name);
		GLboolean linked = GL_FALSE;
		GLint program;
		std::vector<GLint> shaders;
	};
}