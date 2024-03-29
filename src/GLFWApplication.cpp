#include "GLFWApplication.h"
#include <sstream>


int GLFWApplication::SCREEN_WIDTH = 0;
int GLFWApplication::SCREEN_HEIGHT = 0;

// Camera
Camera GLFWApplication::cam = Camera::Camera(glm::vec3(0.0f, 0.0f, 0.0f));
double GLFWApplication::m_lastX = 0.0;
double GLFWApplication::m_lastY = 0.0;
bool GLFWApplication::m_firstMouse = true;

bool GLFWApplication::m_keys[1024];
bool GLFWApplication::m_oldKeys[1024];

GLFWApplication::GLFWApplication(const char *windowTitle, int width, int height) {
	m_windowTitle = windowTitle; // set window title
	// set application window dimensions
	m_width = width;
	m_height = height;

	GLFWApplication::m_lastX = width / 2.0;
	GLFWApplication::m_lastY = height / 2.0;

	GLFWApplication::m_firstMouse = true;

	cam.setCameraPosition(glm::vec3(0.0f, 5.0f, 0.0f));

	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	m_window = glfwCreateWindow(width, height, windowTitle, nullptr, nullptr);
}

bool GLFWApplication::shouldRun() {
	return !glfwWindowShouldClose(getWindow());
}


// call back functions
// Is called whenever a key is pressed / released via GLFW
static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	//std::cout << "keyboard event" << std::endl;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		// 4: draw in wireframe mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		// 5: draw in shaded mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		//glfwSetWindowShouldClose(window, GL_TRUE);
		//glm::translate(m_view, glm::vec3(0.0f, 0.0f, 0.01f));
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			GLFWApplication::m_keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			GLFWApplication::m_keys[key] = false;
		}
	}
}

void GLFWApplication::HandleKeyPresses(float dt)
{
	if (GLFWApplication::m_keys[GLFW_KEY_A])
	{
		GLFWApplication::cam.ProcessKeyboard(LEFT, dt);
	}
	if (GLFWApplication::m_keys[GLFW_KEY_D])
	{
		GLFWApplication::cam.ProcessKeyboard(RIGHT, dt);
	}

	if (GLFWApplication::m_keys[GLFW_KEY_W])
	{
		GLFWApplication::cam.ProcessKeyboard(FORWARD, dt);
	}
	if (GLFWApplication::m_keys[GLFW_KEY_S])
	{
		GLFWApplication::cam.ProcessKeyboard(BACKWARD, dt);
	}
}

// called whenever a mouse event is triggered
static void mouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	//std::cout << "Mouse event";// << std::endl;

	if (GLFWApplication::m_firstMouse)
	{
		GLFWApplication::m_lastX = xPos;
		GLFWApplication::m_lastY = yPos;
		GLFWApplication::m_firstMouse = false;
	}

	double xOffset = xPos - GLFWApplication::m_lastX;
	double yOffset = GLFWApplication::m_lastY - yPos;

	GLFWApplication::m_lastX = xPos;
	GLFWApplication::m_lastY = yPos;

	GLFWApplication::cam.ProcessMouseMovement((GLfloat)xOffset, (GLfloat)yOffset);
}


static void scrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
	//std::cout << "scroll event" << std::endl;
	GLFWApplication::cam.ProcessMouseScroll((GLfloat)yOffset);
}




// Renderer initialisation
void GLFWApplication::init() {

	if (nullptr == m_window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return;
	}

	// make the created window the curent window
	glfwMakeContextCurrent(m_window);
	glfwGetFramebufferSize(m_window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(m_window, keyCallback);
	glfwSetCursorPosCallback(m_window, mouseCallback);
	glfwSetScrollCallback(m_window, scrollCallback);

	// remove the mouse cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// modern GLEW approach
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return;
}

void GLFWApplication::pollEvents() {
	
	glfwPollEvents();
}

void GLFWApplication::showFPS() {
	static double previousSeconds = 0.0;
	static int frameCount;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime();

	elapsedSeconds = currentSeconds - previousSeconds;

	// limit FPS refresh rate to 4 times per second
	if (elapsedSeconds > 0.25) {
		previousSeconds = currentSeconds;
		double FPS = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / FPS;

		std::ostringstream outs;
		outs.precision(3);
		outs << std::fixed
			<< m_windowTitle << "  "
			<< "FPS: " << FPS << "  "
			<< "Frame time: " << msPerFrame << "ms" << std::endl;
		glfwSetWindowTitle(m_window, outs.str().c_str());

		frameCount = 0;
	}
	frameCount++;

}

// clear buffer
void GLFWApplication::clear() {
	// Clear the colorbuffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLFWApplication::display() {
	glBindVertexArray(0);
	// Swap the buffers
	glfwSwapBuffers(m_window);

	for (int i = 0; i < 1024; i++)
	{
		GLFWApplication::m_oldKeys[i] = GLFWApplication::m_keys[i];
	}
}

void GLFWApplication::terminate() {
	glfwTerminate();
}
