#pragma once
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <stdexcept>
#include "GameListener.hpp"

namespace glg
{
	class Game
	{
	private:
		void setCallbacks();
		static void frameBufferCallback(GLFWwindow* window, GLint width, GLint height);
		static void keyboardCallback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mods);
		static void mouseButtonCallback(GLFWwindow* window, GLint button, GLint action, GLint mods);
		static void mouseScrollCallback(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset);
		static void cursorPosCallback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);
		static void cursorEnterCallback(GLFWwindow* window, GLint flag);
		static void windowCloseCallback(GLFWwindow* window);
		static void windowRefreshCallback(GLFWwindow* window);
		static void windowPosCallback(GLFWwindow* window, GLint xpos, GLint ypos);
		static void windowResizeCallback(GLFWwindow* window, GLint width, GLint height);
	protected:
		GLFWwindow* mainWindow;
		GLFWmonitor* primaryMonitor;
		GLFWvidmode* videoMode;
		GameListener* listener;

	public:
		Game(GameListener* listener, GLint major = 3, GLint minor = 3);
		void run();
	};
}