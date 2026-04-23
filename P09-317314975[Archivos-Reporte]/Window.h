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
	// --- VARIABLES PARA EL COCHE ---
	GLfloat getRotacionLlantas() { return RotLlantas; } // Rotación Llantas
	GLfloat getAperturaCofre() { return ApertCofre; }	// Apertura Cofre
	GLfloat getMovCoche() { return MovCoche; }			// Traslación Coche en Z
	int getColorFaroCoche() { return colorFaroCoche; }

	// --- VARIABLES PARA EL HELICÓPTERO ---
	GLfloat getMovHelicoptero() { return movHelicoptero; } // Getter para el helicóptero
	int getFaroHeli() { return faroHeliActual; }		// Para cambio de faros
	
	// --- VARIABLES PARA EL DADO 'ESTATICO' Y LÁMPARAS POINT --
	GLfloat getRotacionDado() { return rotacionDado; }
	bool getApagarLampara() { return apagarLampara; }
	bool getApagarLuzPez() { return apagarLuzPez; }

	// --- VARIABLES PARA EL PEZ ---
	GLfloat getMovPez() { return movPez; }
	GLfloat getRotPez() { return rotPez; }
	// --- VARIABLES PARA EL SPOTLIGHT DEL PEZ ---
	GLfloat getRotSpotX() { return rotSpotX; }
	GLfloat getRotSpotY() { return rotSpotY; }
	GLfloat getRotSpotZ() { return rotSpotZ; }

	// --- ANIMACION COCHE ---
	bool getReiniciaCoche() { return reiniciaCoche; }
	void setReiniciaCoche(bool estado) { reiniciaCoche = estado; } // Para apagar la señal después de usarla

	// --- TIRO DE DADO ---
	bool getTiroD8() { return tiroD8; }
	void setTiroD8(bool estado) { tiroD8 = estado; }

	// -- VARIABLES EXTRA ----
	bool getCambioLuces() { return CambioLuces; }



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
	// ====================================================================================
	// --- VARIABLES DE ANIMACIÓN DEL COCHE ---
	GLfloat RotLlantas;
	GLfloat ApertCofre;
	GLfloat MovCoche;
	int colorFaroCoche;

	// --- VARIABLES PARA EL HELICÓPTERO ---
	GLfloat movHelicoptero;
	int faroHeliActual;

	// --- VARIABLES PARA EL DADO Y LÁMPARAS POINT ---
	GLfloat rotacionDado;
	bool apagarLampara;
	bool apagarLuzPez;

	// --- VARIABLES PARA EL PEZ ---
	GLfloat movPez;
	GLfloat rotPez;
	GLfloat rotSpotX;
	GLfloat rotSpotY;
	GLfloat rotSpotZ;


	// -- VARIABLES EXTRA ----
	bool CambioLuces;


	// --- NUEVAS VARIABLES ---
	bool tiroD8;
	bool reiniciaCoche;
	
	// ====================================================================================
	
	
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

