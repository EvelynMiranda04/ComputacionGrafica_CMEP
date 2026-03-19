#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	// ==================================================================
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	// ==================================================================


	muevex = 2.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "CMEP - Ejercicio 5: Optimización y Carga de Modelos", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
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

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow->muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->muevex -= 1.0;
	}

	// ==================================================================
	// ANIMACIONES DE GODDARD: (Límites: +45° a -45°)
	// Pata Delantera Derecha (D1) - Teclas F (Adelante) y V (Atrás)
	if (key == GLFW_KEY_F) {
		theWindow->articulacion1 += 2.0f; // Velocidad de rotación
		if (theWindow->articulacion1 > 45.0f) theWindow->articulacion1 = 45.0f;
	}
	if (key == GLFW_KEY_V) {
		theWindow->articulacion1 -= 2.0f;
		if (theWindow->articulacion1 < -45.0f) theWindow->articulacion1 = -45.0f;
	}

	// Pata Delantera Izquierda (D2) - Teclas G (Adelante) y B (Atrás)
	if (key == GLFW_KEY_G) {
		theWindow->articulacion2 += 2.0f;
		if (theWindow->articulacion2 > 45.0f) theWindow->articulacion2 = 45.0f;
	}
	if (key == GLFW_KEY_B) {
		theWindow->articulacion2 -= 2.0f;
		if (theWindow->articulacion2 < -45.0f) theWindow->articulacion2 = -45.0f;
	}

	// Pata Trasera Derecha (T1) - Teclas H (Adelante) y N (Atrás)
	if (key == GLFW_KEY_H) {
		theWindow->articulacion3 += 2.0f;
		if (theWindow->articulacion3 > 45.0f) theWindow->articulacion3 = 45.0f;
	}
	if (key == GLFW_KEY_N) {
		theWindow->articulacion3 -= 2.0f;
		if (theWindow->articulacion3 < -45.0f) theWindow->articulacion3 = -45.0f;
	}

	// Pata Trasera Izquierda (T2) - Teclas J (Adelante) y M (Atrás)
	if (key == GLFW_KEY_J) {
		theWindow->articulacion4 += 2.0f;
		if (theWindow->articulacion4 > 45.0f) theWindow->articulacion4 = 45.0f;
	}
	if (key == GLFW_KEY_M) {
		theWindow->articulacion4 -= 2.0f;
		if (theWindow->articulacion4 < -45.0f) theWindow->articulacion4 = -45.0f;
	}

	// Mandíbula (Hocico) - Teclas O (Abrir) y L (Cerrar)
	if (key == GLFW_KEY_O) {
		theWindow->articulacion5 += 2.0f;
		// Tope de apertura máxima configurada en 20 grados
		if (theWindow->articulacion5 > 20.0f) theWindow->articulacion5 = 20.0f;
	}
	if (key == GLFW_KEY_L) {
		theWindow->articulacion5 -= 2.0f;
		// Tope de cierre (evita que traspase el cráneo, límite en -25 grados)
		if (theWindow->articulacion5 < -25.0f) theWindow->articulacion5 = -25.0f;
	}
	// ==================================================================

	
	
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
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
