#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
	mouseFirstMoved = true;

	// Inicialización exclusiva de la Máquina Steampunk
	rotEngranajeG1 = 0.0f;
	rotEngranajeG2 = 0.0f;
	rotEngranajeC1 = 0.0f;
	rotPendulo = 0.0f;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	mouseFirstMoved = true;

	// Inicialización exclusiva de la Máquina Steampunk
	rotEngranajeG1 = 0.0f;
	rotEngranajeG2 = 0.0f;
	rotEngranajeC1 = 0.0f;
	rotPendulo = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::Initialise()
{
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "CMEP Practica 10: Animacion por Keyframes", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(mainWindow);
	createCallbacks();

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, bufferWidth, bufferHeight);
	glfwSetWindowUserPointer(mainWindow, this);

	return 0; // Faltaba este return en tu código original
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// ====================================================================================
	// --- MÁQUINA STEAMPUNK: CONTROLES DE ANIMACIÓN MANUAL ---
	// ====================================================================================

	// [1 y 2]: Engranaje Grande 1 (+ y -)
	if (key == GLFW_KEY_1) { theWindow->rotEngranajeG1 += 5.0f; }
	if (key == GLFW_KEY_2) { theWindow->rotEngranajeG1 -= 5.0f; }

	// [3 y 4]: Engranaje Grande 2 (+ y -)
	if (key == GLFW_KEY_3) { theWindow->rotEngranajeG2 += 5.0f; }
	if (key == GLFW_KEY_4) { theWindow->rotEngranajeG2 -= 5.0f; }

	// [5 y 6]: Engranaje Chico 1 (+ y -)
	if (key == GLFW_KEY_5) { theWindow->rotEngranajeC1 += 5.0f; }
	if (key == GLFW_KEY_6) { theWindow->rotEngranajeC1 -= 5.0f; }

	// [7 y 8]: Péndulo (+ y - respetando límites de -10 a 10 grados)
	if (key == GLFW_KEY_7) {
		theWindow->rotPendulo += 1.0f;
		if (theWindow->rotPendulo > 10.0f) theWindow->rotPendulo = 10.0f;
	}
	if (key == GLFW_KEY_8) {
		theWindow->rotPendulo -= 1.0f;
		if (theWindow->rotPendulo < -10.0f) theWindow->rotPendulo = -10.0f;
	}

	// Registro de pulsación continua
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) {
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			theWindow->keys[key] = false;
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}