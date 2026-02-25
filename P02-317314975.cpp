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
//shaders nuevos se crearían acá (El vShader será el mismo para todos)
static const char* fShaderRojo = "shaders/shaderROJO.frag";
static const char* fShaderVerde = "shaders/shaderVERDE.frag";
static const char* fShaderAzul = "shaders/shaderAZUL.frag";
static const char* fShaderCafe = "shaders/shaderCAFE.frag";
static const char* fShaderVerdeOscuro = "shaders/shaderVerdeO.frag";


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
// CREACION DE FIGURAS Y LETRAS
// ------------------------------------------------------------------------------
void CrearLetrasyFiguras()
{
	//Triangulo diagonal Azul
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			 1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			 1.0f,	 1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
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

	// ------------------------------------------------------------------
	// LETRAS [7,8,9] ---------------------------------------------------
	GLfloat vertices_LetraE[] = {
		// Rectangulo vertical
		-0.6f,	 0.1f,	0.0f,		0.251f,	0.020f,	0.078f,	//T1
		-0.6f,	-0.4f,	0.0f,		0.251f,	0.020f,	0.078f,
		-0.5f,	-0.4f,	0.0f,		0.251f,	0.020f,	0.078f,
		-0.6f,	 0.1f,	0.0f,		0.251f,	0.020f,	0.078f,	//T2
		-0.5f,	-0.4f,	0.0f,		0.251f,	0.020f,	0.078f,
		-0.5f,   0.1f,	0.0f,		0.251f,	0.020f,	0.078f,
		// Rectangulo superior
		-0.5f,   0.1f,	0.0f,		0.251f,	0.020f,	0.078f,	//T3
		-0.5f,   0.0f,	0.0f,		0.251f,	0.020f,	0.078f,
		-0.3f,   0.0f,	0.0f,		0.251f,	0.020f,	0.078f,
		-0.5f,   0.1f,	0.0f,		0.251f,	0.020f,	0.078f,	//T4
		-0.3f,   0.0f,	0.0f,		0.251f,	0.020f,	0.078f,
		-0.3f,   0.1f,	0.0f,		0.251f,	0.020f,	0.078f,
		// Rectangulo medio
		-0.5f,	-0.1f,	0.0f,		0.251f,	0.020f,	0.078f,	//T5
		-0.5f,	-0.2f,	0.0f,		0.251f,	0.020f,	0.078f,
		-0.4f,	-0.2f,	0.0f,		0.251f,	0.020f,	0.078f,
		-0.5f,	-0.1f,	0.0f,		0.251f,	0.020f,	0.078f,	//T6
		-0.4f,	-0.2f,	0.0f,		0.251f,	0.020f,	0.078f,
		-0.4f,	-0.1f,	0.0f,		0.251f,	0.020f,	0.078f,
		// Rectangulo inferior
		-0.5f,	-0.3f,	0.0f,		0.251f,	0.020f,	0.078f,	//T7
		-0.5f,	-0.4f,	0.0f,		0.251f,	0.020f,	0.078f,
		-0.3f,	-0.4f,	0.0f,		0.251f,	0.020f,	0.078f,
		-0.5f,	-0.3f,	0.0f,		0.251f,	0.020f,	0.078f,	//T8
		-0.3f,	-0.4f,	0.0f,		0.251f,	0.020f,	0.078f,
		-0.3f,	-0.3f,	0.0f,		0.251f,	0.020f,	0.078f
	};
	MeshColor* LetraE = new MeshColor();
	LetraE->CreateMeshColor(vertices_LetraE, 144);	// 24*6=144
	meshColorList.push_back(LetraE);

	GLfloat vertices_LetraC[] = {
		// Rectangulo vertical
		-0.2f,   0.3f,	0.0f,		0.529f, 0.118f, 0.220f,	//T1
		-0.2f,	-0.2f,	0.0f,		0.529f, 0.118f, 0.220f,
		-0.1f,	-0.2f,	0.0f,		0.529f, 0.118f, 0.220f,
		-0.2f,   0.3f,	0.0f,		0.529f, 0.118f, 0.220f,	//T2
		-0.1f,	-0.2f,	0.0f,		0.529f, 0.118f, 0.220f,
		-0.1f,   0.3f,	0.0f,		0.529f, 0.118f, 0.220f,
		// Rectangulo superior
		-0.1f,   0.3f,	0.0f,		0.529f, 0.118f, 0.220f,	//T3
		-0.1f,   0.2f,	0.0f,		0.529f, 0.118f, 0.220f,
		 0.1f,   0.2f,	0.0f,		0.529f, 0.118f, 0.220f,
		-0.1f,   0.3f,	0.0f,		0.529f, 0.118f, 0.220f,	//T4
		 0.1f,   0.2f,	0.0f,		0.529f, 0.118f, 0.220f,
		 0.1f,   0.3f,	0.0f,		0.529f, 0.118f, 0.220f,
		 // Rectangulo inferior
		-0.1f,	-0.1f,	0.0f,		0.529f, 0.118f, 0.220f,	//T5
		-0.1f,	-0.2f,	0.0f,		0.529f, 0.118f, 0.220f,
		 0.1f,	-0.2f,	0.0f,		0.529f, 0.118f, 0.220f,
		-0.1f,	-0.1f,	0.0f,		0.529f, 0.118f, 0.220f,	//T6
		 0.1f,	-0.2f,	0.0f,		0.529f, 0.118f, 0.220f,
		 0.1f,	-0.1f,	0.0f,		0.529f, 0.118f, 0.220f
	};
	MeshColor* LetraC = new MeshColor();
	LetraC->CreateMeshColor(vertices_LetraC, 108); // 18*6
	meshColorList.push_back(LetraC);

	GLfloat vertices_LetraM[] = {
		//Rectangulo vertical izquierdo
		0.2f,   0.5f,	0.0f,		0.749f,	0.361f, 0.459f,	//T1
		0.2f,   0.0f,	0.0f,		0.749f, 0.361f, 0.459f,
		0.3f,   0.0f,	0.0f,		0.749f, 0.361f, 0.459f,
		0.2f,   0.5f,	0.0f,		0.749f, 0.361f, 0.459f,	//T2
		0.3f,   0.0f,	0.0f,		0.749f, 0.361f, 0.459f,
		0.3f,   0.5f,	0.0f,		0.749f, 0.361f, 0.459f,
		//Rectangulo vertical derecho
		0.5f,   0.5f,	0.0f,		0.749f, 0.361f, 0.459f,	//T3
		0.5f,   0.0f,	0.0f,		0.749f, 0.361f, 0.459f,
		0.6f,   0.0f,	0.0f,		0.749f, 0.361f, 0.459f,
		0.5f,   0.5f,	0.0f,		0.749f, 0.361f, 0.459f,	//T4
		0.6f,   0.0f,	0.0f,		0.749f, 0.361f, 0.459f,
		0.6f,   0.5f,	0.0f,		0.749f, 0.361f, 0.459f,
		// Diagonales
		0.3f,   0.5f,	0.0f,		0.749f, 0.361f, 0.459f,	//T5
		0.4f,   0.4f,	0.0f,		0.749f, 0.361f, 0.459f,
		0.3f,   0.3f,	0.0f,		0.749f, 0.361f, 0.459f,
		0.3f,   0.3f,	0.0f,		0.749f, 0.361f, 0.459f,	//T6
		0.4f,   0.4f,	0.0f,		0.749f, 0.361f, 0.459f,
		0.4f,   0.2f,	0.0f,		0.749f, 0.361f, 0.459f,
		0.5f,   0.5f,	0.0f,		0.749f, 0.361f, 0.459f,	//T7
		0.4f,   0.4f,	0.0f,		0.749f, 0.361f, 0.459f,
		0.5f,   0.3f,	0.0f,		0.749f, 0.361f, 0.459f,
		0.5f,   0.3f,	0.0f,		0.749f, 0.361f, 0.459f,	//T8
		0.4f,   0.4f,	0.0f,		0.749f, 0.361f, 0.459f,
		0.4f,   0.2f,	0.0f,		0.749f, 0.361f, 0.459f
	};
	MeshColor* LetraM = new MeshColor();
	LetraM->CreateMeshColor(vertices_LetraM, 144); // 24*6
	meshColorList.push_back(LetraM);
}

void CreateShaders()
{
	Shader *shader1 = new Shader(); //shader: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
	Shader *shader2 = new Shader();//shader para VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	// NUEVOS SHADERS PARA LA CASA 3D
	Shader* shaderRojo = new Shader();
	shaderRojo->CreateFromFiles(vShader, fShaderRojo);
	shaderList.push_back(*shaderRojo); // Índice 2 (ROJO)

	Shader* shaderVerde = new Shader();
	shaderVerde->CreateFromFiles(vShader, fShaderVerde);
	shaderList.push_back(*shaderVerde); // Índice 3 (VERDE)

	Shader* shaderAzul = new Shader();
	shaderAzul->CreateFromFiles(vShader, fShaderAzul);
	shaderList.push_back(*shaderAzul); // Índice 4 (AZUL)

	Shader* shaderCafe = new Shader();
	shaderCafe->CreateFromFiles(vShader, fShaderCafe);
	shaderList.push_back(*shaderCafe); // Índice 5 (CAFE)

	Shader* shaderVerdeOscuro = new Shader();
	shaderVerdeOscuro->CreateFromFiles(vShader, fShaderVerdeOscuro);
	shaderList.push_back(*shaderVerdeOscuro); // Índice 6 (VERDE OSCURO)
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
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	// Forzamos el aspecto a 800.0f / 800.0f para garantizar que sea 1.0 y nunca divida entre cero.
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 800.0f, 0.1f, 100.0f);	
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
		shaderList[1].useShader();	//segundo set de shaders para las letras
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		/*
		// ------------------------------------------------------------------
		// DIBUJO DE LETRAS -------------------------------------------------
		// ------------------------------------------------------------------
		// --- Letra E
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.3f, -0.3f, -1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[7]->RenderMeshColor();
		// --- Letra C-
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[8]->RenderMeshColor();
		// --- Letra M
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.3f, 0.3f, -1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[9]->RenderMeshColor();
		*/


		/**/
		// ------------------------------------------------------------------
		// DIBUJO DE CASITA 3D ----------------------------------------------
		// ------------------------------------------------------------------
		// 1. BASE DE LA CASA -----------------------------------------------
		shaderList[2].useShader(); // Shader Rojo (Índice 2)
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		// Fachada
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(1.9f, 2.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();		// Cubo

		// 2. TECHO DE LA CASA ----------------------------------------------
		shaderList[4].useShader();		// Shader azul (Índice 4)
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		// Techo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.50f, -5.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.2f, 0.8f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();		// Pirámide

		// 3. VENTANAS Y PUERTA ---------------------------------------------
		shaderList[3].useShader();		// Shader Verde (Índice 3)
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		// Ventana 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.4f, 0.3f, -4.4f));
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();		// Cubo
		// Ventana 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.4f, 0.3f, -4.4f));
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();		// Cubo
		// Puerta Central
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.55f, -4.4f));
		model = glm::scale(model, glm::vec3(0.55f, 0.80f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();		// Cubo

		// 4. TRONCOS DE LOS ÁRBOLES ----------------------------------------
		shaderList[5].useShader(); // Shader Café (Índice 5)
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		// Tronco 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.68f, -0.695f, -4.8f));
		model = glm::scale(model, glm::vec3(0.55f, 0.70f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();		// Cubo
		// Tronco 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.68f, -0.695f, -4.8f));
		model = glm::scale(model, glm::vec3(0.55f, 0.70f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();		// Cubo

		// 5. HOJAS DE LOS ÁRBOLES ----------------------------------------
		shaderList[6].useShader();		// Shader Verde Oscuro (Índice 6)
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		// Tronco 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.68f, 0.450f, -4.8f));
		model = glm::scale(model, glm::vec3(1.0f, 1.6f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();		// Pirámide
		// Tronco 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.68f, 0.450f, -4.8f));
		model = glm::scale(model, glm::vec3(1.0f, 1.6f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();		// Pirámide



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
/*
// -------------------------------------------------------------------------
//OTROS COMANDOS UTILES ----------------------------------------------------
inicializar matriz: glm::mat4 model(1.0);
reestablecer matriz: model = glm::mat4(1.0);
//Traslación
model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
/////////////R+T//////////
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/