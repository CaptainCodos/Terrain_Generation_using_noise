#pragma once

#include "Application.h"
#include "Camera.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>



class GLFWApplication : public Application {

	static int SCREEN_WIDTH, SCREEN_HEIGHT;

public:
	// constructors
	GLFWApplication() {}
	GLFWApplication(const char *windowTitle, int width, int height);
	~GLFWApplication() {}

	// render methods
	void init() override; //initialises render
	void clear() override; // clear buffer
	void display() override; // display buffer on window
	void terminate() override; // closes application

	// event management
	void pollEvents();
	void HandleKeyPresses(float dt);

	// other methods
	bool shouldRun();
	void showFPS();
	GLFWwindow* getWindow() { return m_window; }

	static Camera cam;

	static double m_lastX;
	static double m_lastY;

	static bool m_firstMouse;

	static bool m_keys[1024];
	static bool m_oldKeys[1024];
private:
	GLFWwindow* m_window = NULL;

	
};