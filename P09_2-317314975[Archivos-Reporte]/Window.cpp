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
	// ====================================================================================
	MovCoche = 0.0f;		//Coche
	RotLlantas = 0.0f;		//Coche
	ApertCofre = 0.0f;		//Coche
	colorFaroCoche = 0;		//Coche
	movHelicoptero = 0.0f;	//Heli
	faroHeliActual = 7;		//Heli
	rotacionDado = 0.0f;	//Dado
	apagarLampara = false;	//Lampara
	movPez = 0.0f;			//Pez
	rotPez = 0.0f;			//Pez
	apagarLuzPez = false;	//Pez
	rotSpotX = 0.0f;		//Pez
	rotSpotY = 0.0f;		//Pez
	rotSpotZ = 0.0f;		//Pez

	CambioLuces = false;	//Previo7
	animacionFuego = false; // El fuego inicia apagado
	// ====================================================================================
	
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	mouseFirstMoved = true;
	// ====================================================================================
	MovCoche = 0.0f;		//Coche
	RotLlantas = 0.0f;		//Coche
	ApertCofre = 0.0f;		//Coche
	colorFaroCoche = 0;		//Coche
	movHelicoptero = 0.0f;	//Heli
	faroHeliActual = 7;		//Heli
	rotacionDado = 0.0f;	//Dado
	apagarLampara = false;	//Lampara
	movPez = 0.0f;			//Pez
	rotPez = 0.0f;			//Pez
	apagarLuzPez = false;	//Pez
	rotSpotX = 0.0f;		//Pez
	rotSpotY = 0.0f;		//Pez
	rotSpotZ = 0.0f;		//Pez

	CambioLuces = false;	//Previo7
	animacionFuego = false; // El fuego inicia apagado
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
	mainWindow = glfwCreateWindow(width, height, "CMEP - Practica 09.2: Animacion avanzada y textura animada", NULL, NULL);

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

	// ====================================================================================
	// 1. CONTROLES DEL SISTEMA Y VENTANA
	// ====================================================================================
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// ====================================================================================
	// 2. ANIMACIÓN Y CONTROL DE MODELOS
	// ====================================================================================

	// --- COCHE ---
	// [1 y 2]: Apertura y cierre del cofre (Límites: 0.0 a 60.0 grados)
	if (key == GLFW_KEY_1) {
		theWindow->ApertCofre += 2.0f;
		if (theWindow->ApertCofre > 60.0f) theWindow->ApertCofre = 60.0f;
	}
	if (key == GLFW_KEY_2) {
		theWindow->ApertCofre -= 2.0f;
		if (theWindow->ApertCofre < 0.0f) theWindow->ApertCofre = 0.0f;
	}

	// [3 y 4]: Movimiento traslacional en Z y rotación de llantas simultánea
	if (key == GLFW_KEY_3) {
		theWindow->RotLlantas += 5.0f;
		theWindow->MovCoche -= 0.5f;
	}
	if (key == GLFW_KEY_4) {
		theWindow->RotLlantas -= 5.0f;
		theWindow->MovCoche += 0.5f;
	}

	// --- HELICÓPTERO ---
	// [5 y 6]: Movimiento en Z y cambio direccional del faro (hacia adelante o atrás)
	if (key == GLFW_KEY_5) {
		theWindow->movHelicoptero -= 0.5f;
		theWindow->faroHeliActual = 7;
	}
	if (key == GLFW_KEY_6) {
		theWindow->movHelicoptero += 0.5f;
		theWindow->faroHeliActual = 6;
	}

	// --- DADO / OCTAEDRO ---
	// [7 y 8]: Rotación libre sobre su propio eje Y
	if (key == GLFW_KEY_7) {
		theWindow->rotacionDado += 2.0f;
	}
	if (key == GLFW_KEY_8) {
		theWindow->rotacionDado -= 2.0f;
	}

	// --- PEZ ABISAL ---
	// [O y P]: Movimiento diagonal (Z y Y combinados) con volteo direccional
	if (key == GLFW_KEY_O) {
		theWindow->movPez += 0.05f; // Avanza hacia el fondo y arriba
		if (theWindow->movPez > 3.0f) theWindow->movPez = 3.0f; // Límite superior
		theWindow->rotPez = 180.0f; // El modelo voltea hacia la dirección de avance
	}
	if (key == GLFW_KEY_P) {
		theWindow->movPez -= 0.05f; // Retrocede hacia el frente y abajo
		if (theWindow->movPez < -0.5f) theWindow->movPez = -0.5f; // Límite inferior
		theWindow->rotPez = 0.0f;   // El modelo voltea para no nadar de reversa
	}

	// ====================================================================================
	// 3. CONTROL DE ILUMINACIÓN Y EFECTOS
	// ====================================================================================

	// [9]: Interruptor de la Lámpara Hextech (PointLight)
	if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
		theWindow->apagarLampara = !theWindow->apagarLampara;
	}

	// [0]: Interruptor de la Luz del Pez Abisal (PointLight Azul)
	if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		theWindow->apagarLuzPez = !theWindow->apagarLuzPez;
	}

	// [I]: Ciclar colores del faro del coche (Arreglo de SpotLights)
	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		theWindow->colorFaroCoche++;
		if (theWindow->colorFaroCoche > 5) { // De Magenta (5) reinicia a Rojo (0)
			theWindow->colorFaroCoche = 0;
		}
	}

	// [C]: Interruptor general del arreglo de luces (Práctica 7)
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		theWindow->CambioLuces = !theWindow->CambioLuces;
	}

	// [T, Y, U]: Rotación independiente del Spotlight Amarillo del Pez
	if (key == GLFW_KEY_T) {
		theWindow->rotSpotX += 5.0f; // Rotación en Eje X
	}
	if (key == GLFW_KEY_Y) {
		theWindow->rotSpotY += 5.0f; // Rotación en Eje Y
	}
	if (key == GLFW_KEY_U) {
		theWindow->rotSpotZ += 5.0f; // Rotación en Eje Z
	}


	// [L]: Interruptor para encender/apagar el fuego animado
	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		theWindow->animacionFuego = !theWindow->animacionFuego;
	}


	// ====================================================================================
	// 4. REGISTRO DE ESTADO GENERAL (Cámara y teclas de pulsación continua)
	// ====================================================================================
	if (key >= 0 && key < 1024)
	{
		// Si se presiona, registramos como verdadero en el arreglo
		if (action == GLFW_PRESS) {
			theWindow->keys[key] = true;
		}
		// Si se suelta, registramos como falso
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
