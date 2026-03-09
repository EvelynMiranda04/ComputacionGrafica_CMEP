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
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;

	/*
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	// -----------------------------------------------------------
	// CAMBIO 4: Inicializar variables para las llantas
	llanta1 = 0.0f;
	llanta2 = 0.0f;
	llanta3 = 0.0f;
	llanta4 = 0.0f;
	// -----------------------------------------------------------
	*/


	// -----------------------------------------------------------
	// CAMBIO 4: Inicializar variables para articulaciones
	articulacion11 = 0.0f; articulacion12 = 0.0f; // Pata 1
	articulacion21 = 0.0f; articulacion22 = 0.0f; // Pata 2
	articulacion31 = 0.0f; articulacion32 = 0.0f; // Pata 3
	articulacion41 = 0.0f; articulacion42 = 0.0f; // Pata 4
	articulacion01 = 0.0f; // Cola 1
	articulacion02 = 0.0f; // Cola 2
	articulacion00 = 0.0f; // Cabeza (Rotación)
	// -----------------------------------------------------------
	
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
	mainWindow = glfwCreateWindow(width, height, "CMEP - Practica 04: Modelado Jerárquico", NULL, NULL);

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

	
	if (key == GLFW_KEY_E) { theWindow->rotax += 5.0; }
	if (key == GLFW_KEY_R) { theWindow->rotay += 5.0; }
	if (key == GLFW_KEY_T) { theWindow->rotaz += 5.0; }


	// ========================================================
	// CONTROLES DE CATNAP
	// ========================================================

	// --- PATA 1 (Delantera Izquierda) ---
	// Superior (7-8)
	if (key == GLFW_KEY_7) { theWindow->articulacion11 += 2.0f; }
	if (key == GLFW_KEY_8) { theWindow->articulacion11 -= 2.0f; }
	// Inferior (F-V)
	if (key == GLFW_KEY_F) { theWindow->articulacion12 += 2.0f; }
	if (key == GLFW_KEY_V) { theWindow->articulacion12 -= 2.0f; }

	// --- PATA 2 (Delantera Derecha) ---
	// Superior (4-5)
	if (key == GLFW_KEY_4) { theWindow->articulacion21 += 2.0f; }
	if (key == GLFW_KEY_5) { theWindow->articulacion21 -= 2.0f; }
	// Inferior (G-B)
	if (key == GLFW_KEY_G) { theWindow->articulacion22 += 2.0f; }
	if (key == GLFW_KEY_B) { theWindow->articulacion22 -= 2.0f; }

	// --- PATA 3 (Trasera Izquierda) ---
	// Superior (1-2)
	if (key == GLFW_KEY_1) { theWindow->articulacion31 += 2.0f; }
	if (key == GLFW_KEY_2) { theWindow->articulacion31 -= 2.0f; }
	// Inferior (H-N)
	if (key == GLFW_KEY_H) { theWindow->articulacion32 += 2.0f; }
	if (key == GLFW_KEY_N) { theWindow->articulacion32 -= 2.0f; }

	// --- PATA 4 (Trasera Derecha) ---
	// Superior (9-0) -> Ajustado como acordamos
	if (key == GLFW_KEY_9) { theWindow->articulacion41 += 2.0f; }
	if (key == GLFW_KEY_0) { theWindow->articulacion41 -= 2.0f; }
	// Inferior (J-M)
	if (key == GLFW_KEY_J) { theWindow->articulacion42 += 2.0f; }
	if (key == GLFW_KEY_M) { theWindow->articulacion42 -= 2.0f; }

	// --- COLA ---
	// Segmento 1 (I-K)
	if (key == GLFW_KEY_I) { theWindow->articulacion01 += 2.0f; }
	if (key == GLFW_KEY_K) { theWindow->articulacion01 -= 2.0f; }
	// Segmento 2 (O-L)
	if (key == GLFW_KEY_O) { theWindow->articulacion02 += 2.0f; }
	if (key == GLFW_KEY_L) { theWindow->articulacion02 -= 2.0f; }

	// --- CABEZA ---
	// Lados (Z-X)
	if (key == GLFW_KEY_X) { theWindow->articulacion00 += 2.0f; }
	if (key == GLFW_KEY_Z) { theWindow->articulacion00 -= 2.0f; }

	// ========================================================




	/*
	
	// ============= CAMBIO 5: Controles de grua ==============
	// ========================================================
	
	// 1.- BRAZO ARTICULADO: ARTICULACIÓN 1 
	if (key == GLFW_KEY_F){	// Tecla F: Movimiento hacia adelante
		theWindow->articulacion1 += 5.0f;
		if (theWindow->articulacion1 > 10.0f){ // LimSup
			theWindow->articulacion1 = 10.0f;}
	}
	if (key == GLFW_KEY_V){	// Tecla V: Movimiento hacia atrás
		theWindow->articulacion1 -= 5.0f;
		if (theWindow->articulacion1 < -110.0f) {// LimInf
			theWindow->articulacion1 = -110.0f; }
	}

	// 1.- BRAZO ARTICULADO: ARTICULACIÓN 2 
	if (key == GLFW_KEY_G) { // Tecla G: Hacia adelante
		theWindow->articulacion2 += 5.0f;
		if (theWindow->articulacion2 > 250.0f) {
			theWindow->articulacion2 = 250.0f;}
	}
	if (key == GLFW_KEY_B){	// Tecla B: Hacia atrás
		theWindow->articulacion2 -= 5.0f;
		if (theWindow->articulacion2 < -60.0f) { // LimInf
			theWindow->articulacion2 = -60.0f;}
	}

	// 1.- BRAZO ARTICULADO: ARTICULACIÓN 3
	if (key == GLFW_KEY_H){	// Tecla H: Hacia adelante
		theWindow->articulacion3 += 5.0f;
		if (theWindow->articulacion3 > 65.0f) { // LimSup
			theWindow->articulacion3 = 65.0f;}
	}
	if (key == GLFW_KEY_N){	// Tecla N: Hacia atrás
		theWindow->articulacion3 -= 5.0f;
		if (theWindow->articulacion3 < -220.0f){ // LimInf
			theWindow->articulacion3 = -220.0f;}
	}

	// 1.- BRAZO ARTICULADO: ARTICULACIÓN 4
	if (key == GLFW_KEY_J){	// Tecla J: Hacia adelante
		theWindow->articulacion4 += 5.0f;
		if (theWindow->articulacion4 > 180.0f){ // LimSup
			theWindow->articulacion4 = 180.0f;}
	}
	if (key == GLFW_KEY_M){	// Tecla M: Hacia atrás
		theWindow->articulacion4 -= 5.0f;
		if (theWindow->articulacion4 < -2.0f){ // LimInf
			theWindow->articulacion4 = -2.0f;}
	}

	// ========================================================
	
	// 2.- CILINDROS: Llanta 1 (Delantera Izquierda)
	if (key == GLFW_KEY_K){
		theWindow->llanta1 += 5.0f;}
	// 2.- CILINDROS: Llanta 2 (Delantera Derecha)
	if (key == GLFW_KEY_L){
		theWindow->llanta2 += 5.0f;}
	// 2.- CILINDROS: Llanta 3 (Trasera Izquierda)
	if (key == GLFW_KEY_I){
		theWindow->llanta3 += 5.0f;}
	// 2.- CILINDROS: Llanta 4 (Trasera Derecha)
	if (key == GLFW_KEY_O){
		theWindow->llanta4 += 5.0f;}
	// ========================================================

	*/




	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}

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
