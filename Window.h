#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }

	/*
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	// -----------------------------------------------------------
	// CAMBIO 2: Getters para las llantas
	GLfloat getLlanta1() { return llanta1; }
	GLfloat getLlanta2() { return llanta2; }
	GLfloat getLlanta3() { return llanta3; }
	GLfloat getLlanta4() { return llanta4; }
	// -----------------------------------------------------------
	*/

	// -----------------------------------------------------------
	// CAMBIO 1 :Articulaciones de las patas
	GLfloat getarticulacion11() { return articulacion11; }
	GLfloat getarticulacion12() { return articulacion12; }
	GLfloat getarticulacion21() { return articulacion21; }
	GLfloat getarticulacion22() { return articulacion22; }
	GLfloat getarticulacion31() { return articulacion31; }
	GLfloat getarticulacion32() { return articulacion32; }
	GLfloat getarticulacion41() { return articulacion41; }
	GLfloat getarticulacion42() { return articulacion42; }
	// CAMBIO 2: Articulaciones de la cola y cabeza
	GLfloat getarticulacion01() { return articulacion01; }
	GLfloat getarticulacion02() { return articulacion02; }
	GLfloat getarticulacion00() { return articulacion00; }
	// -----------------------------------------------------------
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	
	/*
	// -----------------------------------------------------------
	// CAMBIO 3: Variables para almacenar articulaciones y llantas
	GLfloat rotax, rotay, rotaz, articulacion1, articulacion2, articulacion3, articulacion4, llanta1, llanta2, llanta3, llanta4;
	// -----------------------------------------------------------
	*/

	// -----------------------------------------------------------
	// CAMBIO 3: Variables para almacenar articulaciones
	GLfloat rotax, rotay, rotaz, 
		articulacion11, articulacion12, articulacion21, articulacion22, 
		articulacion31, articulacion32, articulacion41, articulacion42,
		articulacion00, articulacion01, articulacion02;
	// -----------------------------------------------------------

	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

