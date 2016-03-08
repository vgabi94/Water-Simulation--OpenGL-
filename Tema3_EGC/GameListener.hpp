#pragma once
#include "Game.hpp"

namespace glg
{
	class GameListener
	{
		friend class Game;	
		const GLchar* title;
		GLboolean resizable, fullscreen;
		GLint width, height;
		// the starting position for this window
		GLint wposx, wposy;
	protected:
		GLint frameWidth, frameHeight;
		GLFWwindow* mainWindow;
		// Do all the initialization here! Do not use the constructor!
		virtual void init() { }
		// process keyboard input
		virtual void onKeyPress(GLint key, GLint scancode, GLint mods) { }
		virtual void onKeyRelease(GLint key, GLint scancode, GLint mods) { }
		// process mouse input
		virtual void onMousePress(GLint button, GLint mods) { }
		virtual void onMouseRelease(GLint button, GLint mods) { }
		virtual void onMouseScroll(GLdouble xoffset, GLdouble yoffset) { }
		// called when the cursor enters the window's client area
		// GLTrue if entered, GLFalse otherwise
		virtual void onCursorEnter(GLint flag) { }
		// called when the cursors changes position
		virtual void onCursorPosChange(GLdouble xpos, GLdouble ypos) { }
		// window callbacks
		virtual void onWindowRefresh() { }
		virtual void onWindowPosChange(GLint xpos, GLint ypos) { }
		virtual void onWindowResize(GLint width, GLint height) { this->width = width; this->height = height; }
		// called when the closing flag has been set
		virtual void onWindowClose() { }
		// framebuffer callbacks
		virtual void onFramebufferResize(GLint width, GLint height) { frameWidth = width; frameHeight = height; }
		// this is called before the rendering takes place
		virtual void update(GLdouble deltaTime) { }
		// this is where all the rendering takes place
		virtual void render(GLdouble deltaTime) { }
	public:
		void setTitle(GLchar* title) { 
			this->title = title; 
		}
		
		void setResizable(GLboolean flag) {
			this->resizable = flag;
		}
		
		void setFullscreen(GLboolean flag) {
			this->fullscreen = flag;
		}
		
		void setResolution(GLint width, GLint height) {
			this->width = width;
			this->height = height;
		}
		
		void setWindowStartingPos(GLint x, GLint y) {
			this->wposx = x;
			this->wposy = y;
		}

		GameListener(GLint w = 800, GLint h = 600, const GLchar* t = "Default Game", 
					 GLboolean r = GL_TRUE, GLboolean f = GL_FALSE, 
					 GLint xpos = 100, GLint ypos = 100) 
			: width(w), height(h), title(t), 
			  resizable(r), fullscreen(f),
			  wposx(xpos), wposy(ypos),
			  mainWindow(nullptr) { }
	};
}