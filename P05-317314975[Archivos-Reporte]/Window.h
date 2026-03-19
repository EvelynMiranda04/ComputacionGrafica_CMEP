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
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	// =================================================================================================
	// GETTERS: Animaciones del Coche
	// Movimiento principal
	GLfloat getarticulacion1() { return articulacion1; } // Rota las 4 llantas al mismo tiempo
	GLfloat getmovCoche() { return movCoche; }           // Traslación en el eje Z (Avance/Retroceso)
	// Carrocería
	GLfloat getarticulacion2() { return articulacion2; } // Abre y cierra el cofre
	// =================================================================================================
	
	// Extras
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	GLfloat getarticulacion5() { return articulacion5; }
	

	~Window();

private:
	GLFWwindow* mainWindow;
	GLint width, height;

	// ==================================================================
	// VARIABLES: Control de ángulos de rotación y traslación (Coche)
	GLfloat articulacion1, articulacion2,  movCoche;
	// ==================================================================

	GLfloat articulacion3, articulacion4, articulacion5;

	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};