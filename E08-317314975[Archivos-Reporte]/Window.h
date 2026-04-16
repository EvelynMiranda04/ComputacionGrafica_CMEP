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
	// --- VARIABLES DE ANIMACIÓN DEL COCHE ---
	GLfloat getRotacionLlantas() { return RotLlantas; } // Rotación Llantas
	GLfloat getAperturaCofre() { return ApertCofre; }	// Apertura Cofre
	GLfloat getMovCoche() { return MovCoche; }			// Traslación Coche en Z
	
	// MOVIMIENTO PARA EL HELICOPTERO;
	GLfloat getMovHelicoptero() { return movHelicoptero; } // Getter para el helicóptero
	// CAMBIO DE ARREGLO DE LUCES: (Previo 7)
	bool getCambioLuces() { return CambioLuces; }

	// --- NUEVOS GETTERS PARA EL EJERCICIO 8 ---
	GLfloat getRotacionDado() { return rotacionDado; }
	bool getApagarLampara() { return apagarLampara; }
	int getColorFaroCoche() { return colorFaroCoche; }
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
	GLfloat RotLlantas;
	GLfloat ApertCofre;
	GLfloat MovCoche;
	// CAMBIO DE ARREGLO DE LUCES:
	bool CambioLuces;
	// MOVIMIENTO PARA EL HELICOPTERO
	GLfloat movHelicoptero;

	GLfloat rotacionDado;	// Prac6
	bool apagarLampara;		// NuevaPrac8
	int colorFaroCoche;		// NuevaPrac8
	// ====================================================================================
	
	
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

