#include "GameObject.hpp"

void glg::GameObject::setPos(const glm::vec3 & pos)
{
	glm::vec3 delta = pos - position;
	model = glm::translate(model, delta);
	position = pos;
}

void glg::GameObject::rotateY(GLfloat angle)
{
	model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void glg::GameObject::scaleUniform(GLfloat ds)
{
	model = glm::scale(model, glm::vec3(ds));
}

void glg::GameObject::translateForward(GLfloat ds)
{
	glm::vec3 dir = glm::vec3(0.f, 0.f, 1.f) * ds;
	model = glm::translate(model, dir);
	position += dir;
}

void glg::GameObject::translateRight(GLfloat ds)
{
	glm::vec3 dir = glm::vec3(1.f, 0.f, 0.f) * ds;
	model = glm::translate(model, dir);
	position += dir;
}

void glg::GameObject::translateUpward(GLfloat ds)
{
	glm::vec3 dir = glm::vec3(0.f, 1.f, 0.f) * ds;
	model = glm::translate(model, dir);
	position += dir;
}

void glg::GameObject::draw()
{
	if (texture != nullptr)
		texture->bind();
	
	if (mesh == nullptr)
		throw std::runtime_error("The object does not have a mesh attached to it!");
	
	mesh->draw();
}
