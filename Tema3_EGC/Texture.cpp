#include "Texture.hpp"

void glg::Texture::loadTexture(const GLchar * imageFile, GLboolean withAlpha)
{
	GLsizei width, height;
	GLubyte* image;

	image = SOIL_load_image(imageFile, &width, &height, 0, withAlpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	if (image == nullptr)
		throw std::runtime_error("SOIL failed to load image!");
	GLuint format = withAlpha ? GL_RGBA : GL_RGB;
	glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(target);

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SOIL_free_image_data(image);
	glBindTexture(target, 0);
}

void glg::Texture2D::createTexture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	target = GL_TEXTURE_2D;
}

void glg::CubeMap::createTexture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	target = GL_TEXTURE_CUBE_MAP;
}

void glg::CubeMap::loadCubemap(std::vector<GLchar*>& faces)
{
	GLsizei width, height;
	GLubyte* image;

	for (GLuint i = 0; i < faces.size(); ++i)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		if (image == nullptr)
			throw std::runtime_error("SOIL failed to load image!");
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(target, 0);
}
