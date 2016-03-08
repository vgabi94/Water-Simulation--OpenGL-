#pragma once
#include "Mesh.hpp"
#include "Texture.hpp"

namespace glg
{
	class GameObject
	{
	public:
		GameObject(const glm::vec3& pos = glm::vec3(0.0f), Mesh* mesh = nullptr, Texture* texture = nullptr) 
			: position(glm::vec3(0.0f)), mesh(mesh), texture(texture)
		{
			setPos(pos);
		}
		~GameObject() {
			if (mesh != nullptr) delete mesh;
			if (texture != nullptr) delete texture;
		}

		void setMesh(Mesh* mesh) { this->mesh = mesh; }
		void setTexture(Texture* texture) { this->texture = texture; }
		void setColor(const glm::vec4& color) { this->color = color; }
		
		void setPos(const glm::vec3& pos);
		void rotateY(GLfloat angle);
		void scaleUniform(GLfloat ds);
		void translateForward(GLfloat ds);
		void translateRight(GLfloat ds);
		void translateUpward(GLfloat ds);
		
		const glm::mat4& getModel() { return model; }
		const glm::vec4& getColor() { return color; }
		const glm::vec3& getPosition() { return position; }
		virtual void draw();
	private:
		glm::vec3 position;
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec4 color;
		Mesh* mesh;
		Texture* texture;
	};
}