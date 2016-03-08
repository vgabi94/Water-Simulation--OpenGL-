#include "Game.hpp"
#include "Shader.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include <iostream>
using namespace glg;

#define CAM_SPEED 40.0f
#define MOUSE_SENSITIVITY 0.01f
#define ZOOM_SENSITIVITY 0.02f
#define WAVE_AMPLITUDE 0.01f
#define WAVE_LENGTH 0.01f
#define WAVE_SPEED 0.01f
#define DAMPING 0.1f
#define STEEPNESS 1.f

struct Wave {
	GLfloat A = WAVE_AMPLITUDE;
	glm::vec2 D;
	glm::vec2 C;
	GLfloat w = 2.f * glm::pi<GLfloat>() / WAVE_LENGTH;
	GLfloat phi = WAVE_SPEED * w;
};

class Tema3 : public GameListener
{
	ShaderProgram *mainProgram, *lightProgram, *skyboxProgram;
	GameObject *grid, *light, *skybox;
	Perspective* proj;
	Camera* eye;
	Mesh* gridMesh;
	Wave w1, w2, w3;

	bool keys[1024];
	bool firstMouse = true;
	GLfloat lastX, lastY;
	bool firstRender = true;
	bool blinn = true;
	GLdouble firstRenderTime;

	void init() override;
	void onFramebufferResize(GLint width, GLint height) override;
	void onKeyPress(GLint key, GLint scancode, GLint mods) override;
	void onKeyRelease(GLint key, GLint scancode, GLint mods) override;
	void onMousePress(GLint button, GLint mods) override;
	void onMouseRelease(GLint button, GLint mods) override;
	void onMouseScroll(GLdouble xoff, GLdouble yoff) override;
	void onCursorPosChange(GLdouble xpos, GLdouble ypos) override;
	void update(GLdouble deltaTime) override;
	void render(GLdouble deltaTime) override;

	void sendWaves();
public:
	using GameListener::GameListener;
	~Tema3() 
	{
		delete mainProgram; delete lightProgram; delete skyboxProgram;
		delete grid; delete light; delete skybox;
		delete proj; delete eye;
	}
};

void Tema3::init()
{
	glfwGetFramebufferSize(mainWindow, &frameWidth, &frameHeight);
	glViewport(0, 0, frameWidth, frameHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);

	skybox = new GameObject;
	skybox->setMesh(new Mesh("skybox.obj"));
	skybox->setTexture(new CubeMap(std::vector<GLchar*> {"skybox/right.png", "skybox/left.png", "skybox/top.png", "skybox/bottom.png",
		"skybox/back.png", "skybox/front.png"}));

	grid = new GameObject;
	gridMesh = new Mesh("grid.obj", GL_TRUE);
	grid->setMesh(gridMesh);
	grid->setColor(glm::vec4(0.f, .5f, 1.f, 1.f));

	light = new GameObject(glm::vec3(0.f, 20.f, 90.f));
	light->setMesh(new Mesh("light.obj"));
	light->setColor(glm::vec4(1.f));

	proj = new Perspective(45.0f, (GLfloat)frameWidth / (GLfloat)frameHeight, 0.1f, 300.0f);
	eye = new Camera(glm::vec3(0.f, 10.0f, 20.f));

	VertexShader lightVert("light.vert");
	FragmentShader lightFrag("light.frag");
	lightVert.compile();
	lightFrag.compile();

	lightProgram = new ShaderProgram{ &lightVert, &lightFrag };
	lightProgram->link();
	lightProgram->use();
	lightProgram->uniform("myColor", glm::vec4(1.f, 1.f, .5f, 1.f));

	VertexShader vert("vertexShader.vert");
	FragmentShader frag("fragmentShader.frag");
	vert.compile();
	frag.compile();

	mainProgram = new ShaderProgram{ &vert, &frag };
	mainProgram->link();
	mainProgram->use();
	mainProgram->uniform("gridColor", glm::vec3(grid->getColor()));
	mainProgram->uniform("lightColor", glm::vec3(light->getColor()));
	mainProgram->uniform("E", glm::e<GLfloat>());
	mainProgram->uniform("damp", DAMPING);
	mainProgram->uniform("Q", STEEPNESS);

	VertexShader skyboxVert("skybox.vert");
	FragmentShader skyboxFrag("skybox.frag");
	skyboxVert.compile();
	skyboxFrag.compile();

	skyboxProgram = new ShaderProgram{ &skyboxVert, &skyboxFrag };
	skyboxProgram->link();

	// Set up waves
	// Directional
	w1.A *= 120; // 120
	w1.w *= 1.f / 150.f;
	w1.phi = 50.f * WAVE_SPEED * w1.w;
	w1.D = glm::normalize(glm::vec2(.5f, .5f));
	// Circular 1
	w2.A *= 110; // 110
	w2.w = 1.f / 1.5f;
	w2.phi = 500 * WAVE_SPEED * w2.w;
	w2.C = glm::vec2(40.f, 40.f);
	// Circular 2
	w3.A *= 130; // 130
	w3.w = 1.f / 2.5f;
	w3.phi = 600 * WAVE_SPEED * w3.w;
	w3.C = glm::vec2(-40.f, -40.f);
	sendWaves();
}

void Tema3::onFramebufferResize(GLint width, GLint height)
{
	GameListener::onFramebufferResize(width, height);
	glViewport(0, 0, width, height);
	proj->aspectRatio = (GLfloat)width / (GLfloat)height;
}

void Tema3::onKeyPress(GLint key, GLint scancode, GLint mods)
{
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(mainWindow, GL_TRUE);
	if (key == GLFW_KEY_F)
		gridMesh->invertWireframeMode();
	if (key == GLFW_KEY_B)
	{
		blinn = !blinn;
	}
	keys[key] = true;
}

void Tema3::onKeyRelease(GLint key, GLint scancode, GLint mods)
{
	keys[key] = false;
}

void Tema3::onMousePress(GLint button, GLint mods)
{
	keys[button] = true;
	if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		firstMouse = true;
}

void Tema3::onMouseRelease(GLint button, GLint mods)
{
	keys[button] = false;
}

void Tema3::onMouseScroll(GLdouble xoff, GLdouble yoff)
{
	if (proj->FoV >= 40.f && proj->FoV <= 45.f)
		proj->FoV -= yoff * ZOOM_SENSITIVITY;
	if (proj->FoV < 40.f)
		proj->FoV = 40.f;
	if (proj->FoV > 45.f)
		proj->FoV = 45.f;
}

void Tema3::onCursorPosChange(GLdouble xpos, GLdouble ypos)
{
	if (keys[GLFW_MOUSE_BUTTON_MIDDLE])
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		GLfloat xoff = lastX - xpos;
		GLfloat yoff = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		xoff *= MOUSE_SENSITIVITY;
		yoff *= MOUSE_SENSITIVITY;

		eye->lookAround(xoff, yoff);
	}
}

void Tema3::update(GLdouble deltaTime)
{
	if (keys[GLFW_KEY_A]) eye->translateRight(-deltaTime * CAM_SPEED);
	if (keys[GLFW_KEY_D]) eye->translateRight(deltaTime * CAM_SPEED);
	if (keys[GLFW_KEY_W]) eye->translateForward(deltaTime * CAM_SPEED);
	if (keys[GLFW_KEY_S]) eye->translateForward(-deltaTime * CAM_SPEED);
	if (keys[GLFW_KEY_Q]) eye->translateUpward(deltaTime * CAM_SPEED);
	if (keys[GLFW_KEY_E]) eye->translateUpward(-deltaTime * CAM_SPEED);
	
	if (keys[GLFW_KEY_LEFT]) light->translateRight(-deltaTime * CAM_SPEED);
	if (keys[GLFW_KEY_RIGHT]) light->translateRight(deltaTime * CAM_SPEED);
	if (keys[GLFW_KEY_UP]) light->translateForward(deltaTime * CAM_SPEED);
	if (keys[GLFW_KEY_DOWN]) light->translateForward(-deltaTime * CAM_SPEED);
	if (keys[GLFW_KEY_KP_ADD]) light->translateUpward(deltaTime * CAM_SPEED);
	if (keys[GLFW_KEY_KP_SUBTRACT]) light->translateUpward(-deltaTime * CAM_SPEED);

	eye->computeView();
	proj->computeProjection();
}

void Tema3::render(GLdouble deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainProgram->use();
	mainProgram->uniform("projection", proj->getProjection());
	mainProgram->uniform("model", grid->getModel());
	mainProgram->uniform("view", eye->getView());
	mainProgram->uniform("eyePos", eye->getPosition());
	mainProgram->uniform("lightPos", light->getPosition());
	mainProgram->uniform("blinn", blinn);
	if (firstRender)
	{
		firstRender = false;
		firstRenderTime = glfwGetTime();
	}
	GLfloat dt = (GLfloat)glfwGetTime() - (GLfloat)firstRenderTime;
	mainProgram->uniform("dt", dt);
	grid->draw();

	lightProgram->use();
	lightProgram->uniform("projection", proj->getProjection());
	lightProgram->uniform("model", light->getModel());
	lightProgram->uniform("view", eye->getView());
	light->draw();

	glDepthFunc(GL_LEQUAL);
	skyboxProgram->use();
	skyboxProgram->uniform("projection", proj->getProjection());
	glm::mat4 view = glm::mat4(glm::mat3(eye->getView()));
	skyboxProgram->uniform("view", view);
	skybox->draw();
	glDepthFunc(GL_LESS);
}

void Tema3::sendWaves()
{
	mainProgram->use();
	mainProgram->uniform("w1.A", w1.A);
	mainProgram->uniform("w1.C", w1.C);
	mainProgram->uniform("w1.D", w1.D);
	mainProgram->uniform("w1.w", w1.w);
	mainProgram->uniform("w1.phi", w1.phi);

	mainProgram->uniform("w2.A", w2.A);
	mainProgram->uniform("w2.C", w2.C);
	mainProgram->uniform("w2.D", w2.D);
	mainProgram->uniform("w2.w", w2.w);
	mainProgram->uniform("w2.phi", w2.phi);

	mainProgram->uniform("w3.A", w3.A);
	mainProgram->uniform("w3.C", w3.C);
	mainProgram->uniform("w3.D", w3.D);
	mainProgram->uniform("w3.w", w3.w);
	mainProgram->uniform("w3.phi", w3.phi);
}

int main()
{
	Tema3* tema = new Tema3(1336, 768, "Assignment 3: Water Simulation");
	tema->setFullscreen(GL_TRUE);

	Game* application = new Game(tema);
	application->run();

	delete tema;
	delete application;
	return 0;
}