#include "Shader.hpp"

glg::Shader::Shader(const GLchar * inGlsl)
{
	std::ifstream glslFile(inGlsl);
	if (!glslFile.good())
		throw std::runtime_error("Cannot open the GLSL file! The file does not exists or the application does not have access to it!");

	glslFile.seekg(0, glslFile.end);
	source.resize(glslFile.tellg());
	glslFile.seekg(0, glslFile.beg);
	glslFile.read(&source[0], source.size());
	glslFile.close();
}

void glg::Shader::compile()
{
	if (compiled)
		throw std::runtime_error("Shader object is already compiled!");
	if (source.empty())
		throw std::runtime_error("Shader source is empty!");

	createShader();
	GLint success;
	GLchar infoLog[INFO_BUFF_SIZE];

	const GLchar* src_ptr = source.c_str();
	const GLint src_siz = source.size();
	glShaderSource(shader, 1, &src_ptr, &src_siz);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(shader, INFO_BUFF_SIZE, nullptr, infoLog);
		std::string error("Failed to compile the vertex shader!\n");
		error.append(infoLog);
		throw std::runtime_error(error);
	}
	compiled = GL_TRUE;
}

void glg::VertexShader::createShader()
{
	shader = glCreateShader(GL_VERTEX_SHADER);
	if (shader == 0)
		throw std::runtime_error("Failed to create the vertex shader!");
}

void glg::FragmentShader::createShader()
{
	shader = glCreateShader(GL_FRAGMENT_SHADER);
	if (shader == 0)
		throw std::runtime_error("Failed to create the fragment shader!");
}

glg::ShaderProgram::ShaderProgram(std::initializer_list<Shader*> shader)
{
	createProgram();
	for (auto& shdr : shader)
		shaders.push_back(shdr->shader);
}

void glg::ShaderProgram::attachShader(Shader & shdr)
{
	if (linked)
		throw std::runtime_error("Cannot attach new shaders! This program has been linked!");
	shaders.push_back(shdr.shader);
}

void glg::ShaderProgram::link()
{
	if (shaders.empty())
		throw std::runtime_error("This program has no shaders attached to it or it has been linked!");

	GLint success;
	GLchar infoLog[INFO_BUFF_SIZE];

	for (auto& shader : shaders)
		glAttachShader(program, shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, INFO_BUFF_SIZE, nullptr, infoLog);
		std::string error("Failed to link the program!\n");
		error.append(infoLog);
		throw std::runtime_error(error);
	}
	
	for (auto& shdr : shaders)
		glDetachShader(program, shdr);
	
	linked = GL_TRUE;
	shaders.clear();
}

void glg::ShaderProgram::use()
{
	if (!linked)
		throw std::runtime_error("You must link the program first!");
	glUseProgram(program);
}

void glg::ShaderProgram::createProgram()
{
	program = glCreateProgram();
	if (program == 0)
		throw std::runtime_error("Failed to create program!");
}

GLint glg::ShaderProgram::getUniformLocation(const GLchar * name)
{
	GLint location = glGetUniformLocation(program, name);
	if (location == -1)
		throw std::runtime_error("Uniform location not found!");
	return location;
}
