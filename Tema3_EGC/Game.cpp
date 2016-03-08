#include "Game.hpp"

void glg::Game::setCallbacks()
{
	glfwSetFramebufferSizeCallback(mainWindow, frameBufferCallback);
	glfwSetKeyCallback(mainWindow, keyboardCallback);
	glfwSetMouseButtonCallback(mainWindow, mouseButtonCallback);
	glfwSetScrollCallback(mainWindow, mouseScrollCallback);
	glfwSetCursorPosCallback(mainWindow, cursorPosCallback);
	glfwSetCursorEnterCallback(mainWindow, cursorEnterCallback);
	glfwSetWindowCloseCallback(mainWindow, windowCloseCallback);
	glfwSetWindowRefreshCallback(mainWindow, windowRefreshCallback);
	glfwSetWindowPosCallback(mainWindow, windowPosCallback);
	glfwSetWindowSizeCallback(mainWindow, windowResizeCallback);
}

// Callback functions
#pragma region GLFW Callbacks
inline void glg::Game::frameBufferCallback(GLFWwindow * window, GLint width, GLint height)
{
	GameListener* listener = (GameListener*) glfwGetWindowUserPointer(window);
	listener->onFramebufferResize(width, height);
}

inline void glg::Game::keyboardCallback(GLFWwindow * window, GLint key, GLint scancode, GLint action, GLint mods)
{
	GameListener* listener = (GameListener*)glfwGetWindowUserPointer(window);
	if (action == GLFW_PRESS)
		listener->onKeyPress(key, scancode, mods);
	else if (action == GLFW_RELEASE)
		listener->onKeyRelease(key, scancode, mods);
}

inline void glg::Game::mouseButtonCallback(GLFWwindow * window, GLint button, GLint action, GLint mods)
{
	GameListener* listener = (GameListener*)glfwGetWindowUserPointer(window);
	if (action == GLFW_PRESS) 
		listener->onMousePress(button, mods);
	else if (action == GLFW_RELEASE)
		listener->onMouseRelease(button, mods);
}

inline void glg::Game::mouseScrollCallback(GLFWwindow * window, GLdouble xoffset, GLdouble yoffset)
{
	GameListener* listener = (GameListener*)glfwGetWindowUserPointer(window);
	listener->onMouseScroll(xoffset, yoffset);
}

inline void glg::Game::cursorPosCallback(GLFWwindow * window, GLdouble xpos, GLdouble ypos)
{
	GameListener* listener = (GameListener*)glfwGetWindowUserPointer(window);
	listener->onCursorPosChange(xpos, ypos);
}

inline void glg::Game::cursorEnterCallback(GLFWwindow * window, GLint flag)
{
	GameListener* listener = (GameListener*)glfwGetWindowUserPointer(window);
	listener->onCursorEnter(flag);
}

inline void glg::Game::windowCloseCallback(GLFWwindow * window)
{
	GameListener* listener = (GameListener*)glfwGetWindowUserPointer(window);
	listener->onWindowClose();
}

inline void glg::Game::windowRefreshCallback(GLFWwindow * window)
{
	GameListener* listener = (GameListener*)glfwGetWindowUserPointer(window);
	listener->onWindowRefresh();
}

inline void glg::Game::windowPosCallback(GLFWwindow * window, GLint xpos, GLint ypos)
{
	GameListener* listener = (GameListener*)glfwGetWindowUserPointer(window);
	listener->onWindowPosChange(xpos, ypos);
}

inline void glg::Game::windowResizeCallback(GLFWwindow * window, GLint width, GLint height)
{
	GameListener* listener = (GameListener*)glfwGetWindowUserPointer(window);
	listener->onWindowResize(width, height);
}
#pragma endregion

// Game class implementation
glg::Game::Game(GameListener* listener, GLint major, GLint minor) : listener(listener)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, listener->resizable);

	if (listener->fullscreen)
		primaryMonitor = glfwGetPrimaryMonitor();
	else
		primaryMonitor = nullptr;

	mainWindow = glfwCreateWindow(listener->width, listener->height, listener->title, primaryMonitor, nullptr);
	if (mainWindow == nullptr)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window!");
	}
	glfwMakeContextCurrent(mainWindow);
	glfwSetWindowPos(mainWindow, listener->wposx, listener->wposy);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Failed to initialize GLEW!");

	listener->mainWindow = mainWindow;
	// set listener as the user pointer; needed for the callbacks
	glfwSetWindowUserPointer(mainWindow, listener);
	setCallbacks();
	// initialize the listener
	listener->init();
}

void glg::Game::run()
{
	GLdouble lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(mainWindow))
	{
		GLdouble currentTime = glfwGetTime();
		GLdouble elapsed = currentTime - lastTime;
		lastTime = currentTime;
		// Process input
		glfwPollEvents();
		// AI and physics simulation
		listener->update(elapsed);
		// Do all the drawing
		listener->render(elapsed);
		// Swap buffers and draw the front buffer
		glfwSwapBuffers(mainWindow);
	}

	glfwTerminate();
}
