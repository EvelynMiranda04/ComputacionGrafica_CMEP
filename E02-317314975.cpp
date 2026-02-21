//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

// ------------------------------------------------------------------------------
// CREACION DE PIRAMIDES, CUBOS (X,Y,Z)
// ------------------------------------------------------------------------------
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}
// ------------------------------------------------------------------------------
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};
GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}
// ------------------------------------------------------------------------------
void CrearLetrasyFiguras()
{
	//Triangulo diagonal Azul
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			 1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			 1.0f,	 1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			/*1.0f,	 1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,   1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,*/	
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,18);
	meshColorList.push_back(letras);

	// ------------------------------------------------------------------
	// TRIANGULOS RGB [1,2,3] -------------------------------------------
	GLfloat vertices_trianguloROJO[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		 1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		 0.0f,	 1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
	};
	MeshColor* trianguloROJO = new MeshColor();
	trianguloROJO->CreateMeshColor(vertices_trianguloROJO, 18);
	meshColorList.push_back(trianguloROJO);

	GLfloat vertices_trianguloVERDE[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.224f,	0.310f,	0.208f,
		 1.0f,	-1.0f,		0.5f,			0.224f,	0.310f,	0.208f,
		 0.0f,	 1.0f,		0.5f,			0.224f,	0.310f,	0.208f,
	};
	MeshColor* trianguloVERDE = new MeshColor();
	trianguloVERDE->CreateMeshColor(vertices_trianguloVERDE, 18);
	meshColorList.push_back(trianguloVERDE);

	GLfloat vertices_trianguloAZUL[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.137f,	0.412f,	0.510f,
		 1.0f,	-1.0f,		0.5f,			0.137f,	0.412f,	0.510f,
		 0.0f,	 1.0f,		0.5f,			0.137f,	0.412f,	0.510f,
	};
	MeshColor* trianguloAZUL = new MeshColor();
	trianguloAZUL->CreateMeshColor(vertices_trianguloAZUL, 18);
	meshColorList.push_back(trianguloAZUL);

	// ------------------------------------------------------------------
	// CUADRADOS RG-CAFE [4,5,6] ----------------------------------------
	GLfloat vertices_cuadradoROJO[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.620f,	0.290f,	0.302f,
		 0.5f,	-0.5f,		0.5f,			0.620f,	0.290f,	0.302f,
		 0.5f,	 0.5f,		0.5f,			0.620f,	0.290f,	0.302f,
		-0.5f,	-0.5f,		0.5f,			0.620f,	0.290f,	0.302f,
		 0.5f,	 0.5f,		0.5f,			0.620f,	0.290f,	0.302f,
		-0.5f,	 0.5f,		0.5f,			0.620f,	0.290f,	0.302f,
	};
	MeshColor* cuadradoROJO = new MeshColor();
	cuadradoROJO->CreateMeshColor(vertices_cuadradoROJO, 36);
	meshColorList.push_back(cuadradoROJO);

	GLfloat vertices_cuadradoVERDE[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.671f,	0.800f,	0.647f,
		 0.5f,	-0.5f,		0.5f,			0.671f,	0.800f,	0.647f,
		 0.5f,	 0.5f,		0.5f,			0.671f,	0.800f,	0.647f,
		-0.5f,	-0.5f,		0.5f,			0.671f,	0.800f,	0.647f,
		 0.5f,	 0.5f,		0.5f,			0.671f,	0.800f,	0.647f,
		-0.5f,	 0.5f,		0.5f,			0.671f,	0.800f,	0.647f,
	};
	MeshColor* cuadradoVERDE = new MeshColor();
	cuadradoVERDE->CreateMeshColor(vertices_cuadradoVERDE, 36);
	meshColorList.push_back(cuadradoVERDE);

	GLfloat vertices_cuadradoCAFE[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.369f,	0.286f,	0.227f,
		 0.5f,	-0.5f,		0.5f,			0.369f,	0.286f,	0.227f,
		 0.5f,	 0.5f,		0.5f,			0.369f,	0.286f,	0.227f,
		-0.5f,	-0.5f,		0.5f,			0.369f,	0.286f,	0.227f,
		 0.5f,	 0.5f,		0.5f,			0.369f,	0.286f,	0.227f,
		-0.5f,	 0.5f,		0.5f,			0.369f,	0.286f,	0.227f,
	};
	MeshColor* cuadradoCAFE = new MeshColor();
	cuadradoCAFE->CreateMeshColor(vertices_cuadradoCAFE, 36);
	meshColorList.push_back(cuadradoCAFE);
}

void CreateShaders()
{
	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	// ------------------------------------------------------------------------------
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	// ------------------------------------------------------------------------------
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	// ------------------------------------------------------------------------------


	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.910f, 0.910f, 0.910f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
/*		
		// ------------------------------------------------------------------
		// DEMOSTRACIÓN DE TODAS LAS FIGURAS --------------------------------
		// ------------------------------------------------------------------
		// TRIANGULO ROJO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();
		// TRIANGULO VERDE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor();
		// TRIANGULO AZUL
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[3]->RenderMeshColor();
		// CUADRADO ROJO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 1.5f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[4]->RenderMeshColor();
		// CUADRADO VERDE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[5]->RenderMeshColor();
		// CUADRADO CAFE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 1.5f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[6]->RenderMeshColor();
*/


		// ------------------------------------------------------------------
		// DIBUJO DE CASITA -------------------------------------------------
		// ------------------------------------------------------------------
		// CUADRADO ROJO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.2f));
		model = glm::scale(model, glm::vec3(0.9f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[4]->RenderMeshColor();
		// TRIANGULO AZUL
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.72f, -2.2f));
		model = glm::scale(model, glm::vec3(0.5f, 0.22f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[3]->RenderMeshColor();
		// CUADRADO VERDE 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.2f, 0.1f, -2.1f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[5]->RenderMeshColor();
		// CUADRADO VERDE 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.2f, 0.1f, -2.1f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[5]->RenderMeshColor();
		// CUADRADO VERDE 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.33f, -2.1f));
		model = glm::scale(model, glm::vec3(0.25f, 0.35f, 1.0f));;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[5]->RenderMeshColor();
		// CUADRADO CAFE 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.68f, -0.395f, -2.1f));
		model = glm::scale(model, glm::vec3(0.15f, 0.20f, 1.0f));;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[6]->RenderMeshColor();
		// CUADRADO CAFE 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.68f, -0.395f, -2.1f));
		model = glm::scale(model, glm::vec3(0.15f, 0.20f, 1.0f));;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[6]->RenderMeshColor();
		// TRIANGULO VERDE 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.68f, 0.0f, -2.1f));
		model = glm::scale(model, glm::vec3(0.15f, 0.3f, 1.0f));;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor();
		// TRIANGULO VERDE 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.68f, 0.0f, -2.1f));
		model = glm::scale(model, glm::vec3(0.15f, 0.3f, 1.0f));;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor();
/**/
/*
		// ------------------------------------------------------------------
		// CUBO Y PIRAMIDE --------------------------------------------------
		// ------------------------------------------------------------------
		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		shaderList[0].useShader(); 
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.05;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		// CAMBIO DE ROTACIÓN AKI
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();		// CAMBIO ENTRE PIRAMIDE Y CUBO 
*/

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}
// --------------------------------------------------------------------------
// OTROS COMANDOS UTILES ----------------------------------------------------
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/