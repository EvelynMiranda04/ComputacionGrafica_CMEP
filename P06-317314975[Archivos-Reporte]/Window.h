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
	// ====================================================================================
	GLfloat getRotacionDado() { return rotacionDado; } // AGREGAMOS LA ROTACIÓN DEL DADO
	// --- VARIABLES DE ANIMACIÓN DEL COCHE ---
	GLfloat getarticulacion1() { return articulacion1; } // Rotación Llantas
	GLfloat getarticulacion2() { return articulacion2; } // Apertura Cofre
	GLfloat getmovCoche() { return movCoche; }           // Traslación Coche en Z
	// ====================================================================================

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	
	
	~Window();
private: 
	
    
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;

	// ====================================================================================
	// --- VARIABLES DE ANIMACIÓN DEL COCHE ---
	GLfloat articulacion1;
	GLfloat articulacion2;
	GLfloat movCoche;
	GLfloat rotacionDado; // AGREGAMOS LA ROTACIÓN DEL DADO
	// ====================================================================================


	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

