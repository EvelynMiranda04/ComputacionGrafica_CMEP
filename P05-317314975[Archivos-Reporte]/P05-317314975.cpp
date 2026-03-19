/*
Práctica 5: Optimización y Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// ==================================================================
Camera camera;
// Modelos del Coche
Model Carroceria;
Model Cofre;
Model Llanta1;
Model Llanta2;
Model Llanta3;
Model Llanta4;
// ==================================================================

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);


	// ==================================================================
	// CARGA DE MODELOS DEL COCHE
	Carroceria = Model();
	Carroceria.LoadModel("Models/coche.obj");
	Cofre = Model();
	Cofre.LoadModel("Models/cofre.obj");
	Llanta1 = Model();
	Llanta1.LoadModel("Models/Llanta1.obj");
	Llanta2 = Model();
	Llanta2.LoadModel("Models/Llanta2.obj");
	Llanta3 = Model();
	Llanta3.LoadModel("Models/Llanta3.obj");
	Llanta4 = Model();
	Llanta4.LoadModel("Models/Llanta4.obj");
	// ==================================================================


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);

	// ==================================================================
	// OPTIMIZACIÓN: Instanciamos los colores UNA SOLA VEZ 
	glm::vec3 colorPiso = glm::vec3(0.310f, 0.314f, 0.329f);
	glm::vec3 colorChasis = glm::vec3(0.471f, 0.365f, 0.353f);
	glm::vec3 colorLlantas = glm::vec3(0.169f, 0.145f, 0.141f);
	glm::vec3 colorCofre = glm::vec3(0.471f, 0.310f, 0.290f);
	// ==================================================================

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// ====================================================================================================
		// INICIA DIBUJO DEL PISO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPiso)); // Llamamos a la variable optimizada
		meshList[2]->RenderMesh();

		// ====================================================================================================
		// JERARQUÍA DEL COCHE
		// ====================================================================================================

		// --------------------------------------------------------------------------------------------------
		// 1. NODO PADRE: Chasis / Carrocería
		model = glm::mat4(1.0);
		// Traslación base. La variable getmovCoche() se suma a Z para hacer avanzar o retroceder todo el modelo.
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -3.0f + mainWindow.getmovCoche()));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		modelaux = model; // GUARDAMOS EL PIVOTE CENTRAL para que lo hereden las llantas y el cofre
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorChasis));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carroceria.RenderModel();
		// --------------------------------------------------------------------------------------------------
	
		// --------------------------------------------------------------------------------------------------
		// NODOS HIJOS (LLANTAS)
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlantas)); // El color aplica para las 4 llantas
		// NODO HIJO 1: Llanta Trasera Izquierda (Z Positiva)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.0f, 0.3f, 4.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta1.RenderModel();
		// NODO HIJO 2: Llanta Trasera Derecha (Z Positiva)
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.0f, 0.3f, 4.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta2.RenderModel();
		// NODO HIJO 3: Llanta Delantera Izquierda (Z Negativa)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.0f, 0.3f, -6.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta3.RenderModel();
		// NODO HIJO 4: Llanta Delantera Derecha (Z Negativa)
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.0f, 0.3f, -6.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta4.RenderModel();
		// NODO HIJO 5: Cofre
		model = modelaux;
		// Se traslada el cofre para que su pivote de rotación coincida con las bisagras
		model = glm::translate(model, glm::vec3(0.0f, 2.7f, -4.2f));
		// Rotación para abrir y cerrar el cofre (controlado con getarticulacion2)
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorCofre));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre.RenderModel();
		// --------------------------------------------------------------------------------------------------
		// ====================================================================================================


		glUseProgram(0);

		mainWindow.swapBuffers();


	}

	return 0;
}
