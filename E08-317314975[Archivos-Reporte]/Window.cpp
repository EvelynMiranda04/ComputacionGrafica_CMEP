#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
	muevex = 2.0f;
	// ====================================================================================
	RotLlantas = 0.0f;
	ApertCofre = 0.0f;
	MovCoche = 0.0f;
	CambioLuces = false;	// CAMBIO DE ARREGLO DE LUCES
	movHelicoptero = 0.0f;	// MOVIMIENTO PARA EL HELICOPTERO

	rotacionDado = 0.0f;	// Inicialización
	apagarLampara = false;	// Inicialización
	colorFaroCoche = 5;		// Inicialización
	// ====================================================================================
	
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	// ====================================================================================
	RotLlantas = 0.0f;
	ApertCofre = 0.0f;
	MovCoche = 0.0f;
	CambioLuces = false;	// CAMBIO DE ARREGLO DE LUCES
	movHelicoptero = 0.0f;	// MOVIMIENTO PARA EL HELICOPTERO
	rotacionDado = 0.0f;	// Inicialización
	apagarLampara = false;	// Inicialización
	colorFaroCoche = 5;		// Inicialización

	// ====================================================================================
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
	mainWindow = glfwCreateWindow(width, height, "CMEP - Ejercicio 08: Iluminacion 2", NULL, NULL);

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

	// --- Teclas originales que se mantienen ---
	if (key == GLFW_KEY_Y)
	{
		theWindow->muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->muevex -= 1.0;
	}

	// ========================================================================
	// ---------------- NUEVO MAPEO DE TECLAS PRÁCTICA 8 ----------------

	// 1 y 2: Control del Cofre
	if (key == GLFW_KEY_1) {
		theWindow->ApertCofre += 2.0f;
		if (theWindow->ApertCofre > 60.0f) theWindow->ApertCofre = 60.0f;
	}
	if (key == GLFW_KEY_2) {
		theWindow->ApertCofre -= 2.0f;
		if (theWindow->ApertCofre < 0.0f) theWindow->ApertCofre = 0.0f;
	}

	// 3 y 4: Movimiento del Coche (Z y rotación de llantas)
	if (key == GLFW_KEY_3) {
		theWindow->RotLlantas += 5.0f;
		theWindow->MovCoche -= 0.5f;
	}
	if (key == GLFW_KEY_4) {
		theWindow->RotLlantas -= 5.0f;
		theWindow->MovCoche += 0.5f;
	}

	// 5 y 6: Movimiento del Helicóptero
	if (key == GLFW_KEY_5) {
		theWindow->movHelicoptero -= 0.5f; // Resta en Z (avanza hacia el fondo)
	}
	if (key == GLFW_KEY_6) {
		theWindow->movHelicoptero += 0.5f; // Suma en Z (viene hacia la cámara)
	}

	// 7 y 8: Rotación del Dado
	if (key == GLFW_KEY_7) {
		theWindow->rotacionDado += 2.0f;
	}
	if (key == GLFW_KEY_8) {
		theWindow->rotacionDado -= 2.0f;
	}

	// 9: Interruptor Lámpara
	if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
		theWindow->apagarLampara = !theWindow->apagarLampara;
	}


	// 0: Ciclar colores del faro del coche
	if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		theWindow->colorFaroCoche++; // Sumamos 1
		if (theWindow->colorFaroCoche > 5) { // Si pasa de 5 (Magenta), regresa a 0 (Rojo)
			theWindow->colorFaroCoche = 0;
		}
	}

	// ---------------- CAMBIO DE ARREGLO DE LUCES ----------------
	// Cambiar arreglos de luces - Tecla C (Se mantiene)
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		// Invierte el valor (de true a false y viceversa)
		theWindow->CambioLuces = !theWindow->CambioLuces;
	}
	// ========================================================================

	// Registro de estado general (Aquí se detectan W, A, S, D para la cámara)
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
