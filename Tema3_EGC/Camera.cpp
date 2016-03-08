#include "Camera.hpp"

const glm::vec3 glg::Camera::WORLD_UP = glm::vec3(0.f, 1.f, 0.f);
const glm::vec3 glg::Camera::WORLD_RIGHT = glm::vec3(1.f, 0.f, 0.f);

void glg::Camera::setPosition(const glm::vec3 & position, const glm::vec3 & target, const glm::vec3 & up)
{
	this->position = position;
	distanceToTarget = glm::length(target - position);
	forward = glm::normalize(target - position);
	right = glm::normalize(glm::cross(forward, up));
	this->up = glm::normalize(glm::cross(right, forward));
	computeView();
}

void glg::Camera::translateForward(float ds)
{
	position += ds * forward;
}

void glg::Camera::translateRight(float ds)
{
	position += ds * right;
}

void glg::Camera::translateUpward(float ds)
{
	position += ds * up;
}

void glg::Camera::rotateFPS_X(float angle)
{
	glm::quat q = glm::angleAxis(angle, right);
	forward = glm::normalize(glm::rotate(q, forward));
	up = glm::normalize(glm::cross(right, forward));
}

void glg::Camera::rotateFPS_Y(float angle)
{
	glm::quat q = glm::angleAxis(angle, WORLD_UP);
	forward = glm::normalize(glm::rotate(q, forward));
	right = glm::normalize(glm::rotate(q, right));
	up = glm::normalize(glm::cross(right, forward));
}

void glg::Camera::rotateFPS_Z(float angle)
{
	glm::quat q = glm::angleAxis(angle, forward);
	right = glm::normalize(glm::rotate(q, right));
	up = glm::normalize(glm::cross(right, forward));
}

void glg::Camera::lookAround(float pitch, float yaw)
{
	rotateFPS_X(yaw);
	rotateFPS_Y(pitch);
}

void glg::Orthographic::set(float left, float right, float bottom, float top, float zNear, float zFar)
{
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;
	this->zNear = zNear;
	this->zFar = zFar;
	projection = glm::ortho(left, right, bottom, top, zNear, zFar);
}

void glg::Perspective::set(float FoV, float aspectRatio, float zNear, float zFar)
{
	this->FoV = FoV;
	this->aspectRatio = aspectRatio;
	this->zNear = zNear;
	this->zFar = zFar;
	projection = glm::perspective(FoV, aspectRatio, zNear, zFar);
}
