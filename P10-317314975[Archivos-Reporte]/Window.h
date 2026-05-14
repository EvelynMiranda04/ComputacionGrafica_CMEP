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

	// ====================================================================================
	// --- VARIABLES PARA LA MÁQUINA STEAMPUNK ---
	GLfloat getRotEngranajeG1() { return rotEngranajeG1; }
	GLfloat getRotEngranajeG2() { return rotEngranajeG2; }
	GLfloat getRotEngranajeC1() { return rotEngranajeC1; }
	GLfloat getRotPendulo() { return rotPendulo; }

	// Setters (Necesarios para que la animación automática modifique los valores)
	void setRotEngranajeG1(GLfloat val) { rotEngranajeG1 = val; }
	void setRotEngranajeG2(GLfloat val) { rotEngranajeG2 = val; }
	void setRotEngranajeC1(GLfloat val) { rotEngranajeC1 = val; }
	void setRotPendulo(GLfloat val) { rotPendulo = val; }
	// ====================================================================================

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	bool* getsKeys() { return keys; }
	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;

	// ====================================================================================
	// --- VARIABLES DE LA MÁQUINA STEAMPUNK --- 
	GLfloat rotEngranajeG1;
	GLfloat rotEngranajeG2;
	GLfloat rotEngranajeC1;
	GLfloat rotPendulo;
	// ====================================================================================

	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};