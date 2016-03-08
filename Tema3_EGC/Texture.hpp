#pragma once
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL\glew.h>
#include <SOIL\SOIL.h>
#include <vector>

namespace glg
{
	class Texture
	{
	public:
		Texture() { }
		~Texture() {
			glDeleteTextures(1, &texture);
		}
		
		virtual void bind() {
			glBindTexture(target, texture);
		}
	protected:
		virtual void loadTexture(const GLchar* imageFile, GLboolean withAlpha = GL_FALSE);
		virtual void createTexture() = 0; // Must implement!
		GLenum target;
		GLuint texture;
	};

	class Texture2D : public Texture
	{
		void createTexture() override;
	public:
		Texture2D(const GLchar* imageFile, GLboolean withAlpha = GL_FALSE) {
			createTexture();
			loadTexture(imageFile, withAlpha);
		}
	};

	class CubeMap : public Texture
	{
		void createTexture() override;
		void loadCubemap(std::vector<GLchar*>& faces);
	public:
		// right, left, top, bottom, back, front
		CubeMap(std::vector<GLchar*>& faces) {
			createTexture();
			loadCubemap(faces);
		}
	};
}