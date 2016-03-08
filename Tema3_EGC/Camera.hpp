#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\quaternion.hpp>

namespace glg
{
	class Projection
	{
	protected:
		glm::mat4 projection;
	public:
		virtual const glm::mat4& getProjection() { return projection; }
		virtual void computeProjection() = 0;
	};

	struct Orthographic : public Projection
	{
		float left, right;
		float bottom, top;
		float zNear, zFar;

		Orthographic(float left, float right, float bottom, float top, float zNear, float zFar) {
			set(left, right, bottom, top, zNear, zFar);
		}

		void set(float left, float right, float bottom, float top, float zNear, float zFar);
		void computeProjection() {
			projection = glm::ortho(left, right, bottom, top, zNear, zFar);
		}
	};

	struct Perspective : public Projection
	{
		float FoV, aspectRatio;
		float zNear, zFar;

		Perspective(float FoV, float aspectRatio, float zNear, float zFar) {
			set(FoV, aspectRatio, zNear, zFar);
		}

		void set(float FoV, float aspectRatio, float zNear, float zFar);
		void computeProjection() {
			projection = glm::perspective(FoV, aspectRatio, zNear, zFar);
		}
	};

	class Camera
	{
	public:
		Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f), const glm::vec3& target = glm::vec3(0.0f)) {
			setPosition(position, target, WORLD_UP);
		}

		void setPosition(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up = WORLD_UP);
		void setTarget(const glm::vec3& target) { setPosition(position, target, WORLD_UP); }
		void translateForward(float ds);
		void translateRight(float ds);
		void translateUpward(float ds);
		void rotateFPS_X(float angle);
		void rotateFPS_Y(float angle);
		void rotateFPS_Z(float angle);
		void lookAround(float angX, float angY);

		const glm::mat4& getView() { return view; }
		const glm::vec3& getPosition() { return position; }
		void computeView() {
			view = glm::lookAt(position, position + forward, up);
		}
		static const glm::vec3 WORLD_UP;
		static const glm::vec3 WORLD_RIGHT;
	private:
		float distanceToTarget;
		glm::mat4 view;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 up;
		glm::vec3 right;
	};
}