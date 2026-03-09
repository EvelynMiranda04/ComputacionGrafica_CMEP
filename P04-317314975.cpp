/*Práctica 4: Modelado Jerárquico.
Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere 
heredar entre diversas instancias para que estén unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
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
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z
using std::vector;
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

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
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	// -----------------------------------------------------------
	// CAMBIO 1: Para tener un mejor cilindro como llanta
	CrearCilindro(20, 1.0f);//índice 2 en MeshList
	// -----------------------------------------------------------
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();
	
	

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarquía

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		// Recibir eventos del usuario y actualizar cámara
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Limpiar la ventana
		glClearColor(0.588f, 0.557f, 0.612f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activar Shader y obtener locaciones de uniforms
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();


		// Mandar las matrices de proyección y vista (solo se necesita hacer una vez por frame)
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// ****************************************************************************************************
		// *********************************  MODELADO JERARQUICO DE CATNAP  **********************************
		// ****************************************************************************************************
		
		// ====================================================================================================
		// 1. PALETA DE COLORES DE CATNAP
		// ====================================================================================================
		glm::vec3 colorArticulacion = glm::vec3(0.369f, 0.263f, 0.431f);
		glm::vec3 colorPelaje = glm::vec3(0.349f, 0.231f, 0.420f);
		glm::vec3 colorOrejas = glm::vec3(0.310f, 0.224f, 0.361f);
		glm::vec3 colorOrejasInteriores = glm::vec3(0.420f, 0.333f, 0.471f);
		glm::vec3 colorPatasSup = glm::vec3(0.278f, 0.192f, 0.329f);
		glm::vec3 colorPatasInf = glm::vec3(0.216f, 0.161f, 0.251f);
		glm::vec3 colorGarras = glm::vec3(0.188f, 0.157f, 0.212f);
		glm::vec3 colorNegro = glm::vec3(0.125f, 0.118f, 0.129f);
		glm::vec3 colorBlanco = glm::vec3(0.950f, 0.950f, 0.950f);



		// ====================================================================================================
		// 2. NODO RAÍZ: TORSO DE CATNAP
		// ====================================================================================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 6.0f, 0.0f));
		// Rotación global de la escena (Con las teclas E, R, T)
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 modelaux = model; // GUARDAMOS: Centro del torso (Nodo Padre absoluto)
		// Dibujo del torso
		model = glm::scale(model, glm::vec3(1.4f, 1.5f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPelaje));
		sp.render();	 //(Esfera estirada)



		// ====================================================================================================
		// 3. CUELLO, CABEZA Y DETALLES FACIALES (Hijos del Torso)
		// ====================================================================================================
		// ---- ARTICULACIÓN 0 (Cuello) -----------------------------------------------------------------------
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, 3.7f));
		// Articulación: Rotación de la cabeza (Teclas Z y X)
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion00()), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 modelCuello = model; // GUARDAMOS: Pivote del cuello 
		// Dibujo del cuello
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		sp.render();
		// ---- CABEZA ----------------------------------------------------------------------------------------
		model = modelCuello; // Hereda rotación del cuello
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 1.0f));
		glm::mat4 modelCabeza = model; // GUARDAMOS: Centro de la cabeza (Padre de rostro y orejas)
		// Dibujo de la cabeza
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPelaje));
		sp.render();
		// --------------------- DETALLES FACIALES: OREJAS, OJOS Y BOCA (Hijos de la cabeza) ------------------
		// ---- OREJAS EXTERIORES -----------------------------------------------------------------------------
		// Izquierda
		model = modelCabeza;
		model = glm::translate(model, glm::vec3(1.0f, 1.6f, 0.0f));
		model = glm::rotate(model, glm::radians(-28.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorOrejas));
		meshList[4]->RenderMeshGeometry();
		// Derecha
		model = modelCabeza;
		model = glm::translate(model, glm::vec3(-1.0f, 1.6f, 0.0f));
		model = glm::rotate(model, glm::radians(28.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMeshGeometry();
		// ---- OREJAS INTERIORES -----------------------------------------------------------------------------
		// Izquierda
		model = modelCabeza;
		model = glm::translate(model, glm::vec3(1.0f, 1.6f, 0.4f));
		model = glm::rotate(model, glm::radians(-28.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8f, 1.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorOrejasInteriores)); // Aplicamos nuevo color
		meshList[4]->RenderMeshGeometry();
		// Derecha
		model = modelCabeza;
		model = glm::translate(model, glm::vec3(-1.0f, 1.6f, 0.4f));
		model = glm::rotate(model, glm::radians(28.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8f, 1.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMeshGeometry();
		// ---- BOCA (Sonrisa de media luna) ------------------------------------------------------------------
		// Fondo negro
		model = modelCabeza;
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 1.15f));
		model = glm::scale(model, glm::vec3(1.1f, 0.7f, 0.20f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorNegro));
		sp.render();
		// Máscara (Tapa la mitad superior)
		model = modelCabeza;
		model = glm::translate(model, glm::vec3(0.0f, 0.1f, 1.20f));
		model = glm::scale(model, glm::vec3(1.2f, 0.6f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPelaje));
		sp.render();
		// ---- OJOS (Cuencas y Pupilas) ----------------------------------------------------------------------
		// Cuenca Izquierda
		model = modelCabeza;
		model = glm::translate(model, glm::vec3(0.6f, 0.4f, 1.25f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorNegro));
		sp.render();
		// Pupila Izquierda
		model = modelCabeza;
		model = glm::translate(model, glm::vec3(0.6f, 0.4f, 1.35f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorBlanco));
		sp.render();
		// Cuenca Derecha
		model = modelCabeza;
		model = glm::translate(model, glm::vec3(-0.6f, 0.4f, 1.25f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorNegro));
		sp.render();
		// Pupila Derecha
		model = modelCabeza;
		model = glm::translate(model, glm::vec3(-0.6f, 0.4f, 1.35f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorBlanco));
		sp.render();
		// ---- NARIZ (Pirámide invertida)
		model = modelCabeza;
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, 1.35f)); // Centro de la cara
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorNegro));
		meshList[4]->RenderMeshGeometry();



		// ====================================================================================================
		// 4. COLA (Hija del Torso)
		// ====================================================================================================
		// ---- ARTICULACIÓN 1 (Base) -------------------------------------------------------------------------
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -4.5f));
		// Articulación: Sube y baja (Teclas I-K)
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion01()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Inclinación natural
		glm::mat4 modelCola1 = model; // GUARDAMOS: Pivote base cola
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		sp.render();
		// ---- SEGMENTO COLA 1 -------------------------------------------------------------------------------
		model = modelCola1;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatasSup)); // Aplicamos nuevo color
		sp.render();
		// ---- ARTICULACIÓN 2 (Medio) ------------------------------------------------------------------------
		model = modelCola1; // Hereda de la base
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.0f));
		// Articulación: Sube y baja (Teclas O-L)
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion02()), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 modelCola2 = model; // GUARDAMOS: Pivote medio cola
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		sp.render();
		// ---- SEGMENTO COLA 2 -------------------------------------------------------------------------------
		model = modelCola2;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatasInf)); // Aplicamos nuevo color
		sp.render();



		// ====================================================================================================
		// 5. PATA 1: DELANTERA IZQUIERDA
		// ====================================================================================================
		// ---- ARTICULACION 11 (HOMBRO: Teclas 7-8) ----------------------------------------------------------
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.8f, -0.4f, 3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 modelHombro1 = model;
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		sp.render();
		// ---- PIERNA SUPERIOR 1 ----------------------------------------------------------------------------
		model = modelHombro1;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 3.0f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatasSup)); // Aplicamos nuevo color
		meshList[2]->RenderMeshGeometry();
		// ---- ARTICULACION 12 (RODILLA: Teclas F-V) --------------------------------------------------------
		model = modelHombro1;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion12()), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 modelRodilla1 = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		sp.render();
		// ---- PIERNA INFERIOR 1 ----------------------------------------------------------------------------
		model = modelRodilla1;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 3.0f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatasInf)); // Aplicamos nuevo color
		meshList[2]->RenderMeshGeometry();
		// ---- PLANTA DEL PIE 1 -----------------------------------------------------------------------------
		model = modelRodilla1;
		model = glm::translate(model, glm::vec3(0.0f, -3.2f, 0.4f));
		glm::mat4 modelPie1 = model;
		model = glm::scale(model, glm::vec3(0.8f, 0.4f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();
		// ---- GARRAS 1 -------------------------------------------------------------------------------------
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorNegro));
		// Izquierda
		model = modelPie1;
		model = glm::translate(model, glm::vec3(0.3f, -0.1f, 0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.4f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry();
		// Centro
		model = modelPie1;
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.4f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry();
		// Derecha
		model = modelPie1;
		model = glm::translate(model, glm::vec3(-0.3f, -0.1f, 0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.4f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry();



		// ====================================================================================================
		// 6. PATA 2: DELANTERA DERECHA
		// ====================================================================================================
		// ---- ARTICULACION 21 (HOMBRO: Teclas 4-5) ----------------------------------------------------------
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.8f, -0.4f, 3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion21()), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 modelHombro2 = model;
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		sp.render();
		// ---- PIERNA SUPERIOR 2 ----------------------------------------------------------------------------
		model = modelHombro2;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 3.0f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatasSup)); // Aplicamos nuevo color
		meshList[2]->RenderMeshGeometry();
		// ---- ARTICULACION 22 (RODILLA: Teclas G-B) ---------------------------------------------------------
		model = modelHombro2;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion22()), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 modelRodilla2 = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		sp.render();
		// ---- PIERNA INFERIOR 2 ----------------------------------------------------------------------------
		model = modelRodilla2;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 3.0f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatasInf)); // Aplicamos nuevo color
		meshList[2]->RenderMeshGeometry();
		// ---- PLANTA DEL PIE 2 -----------------------------------------------------------------------------
		model = modelRodilla2;
		model = glm::translate(model, glm::vec3(0.0f, -3.2f, 0.4f));
		glm::mat4 modelPie2 = model;
		model = glm::scale(model, glm::vec3(0.8f, 0.4f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();
		// ---- GARRAS 2 -------------------------------------------------------------------------------------
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorNegro));
		// Izquierda
		model = modelPie2;
		model = glm::translate(model, glm::vec3(0.3f, -0.1f, 0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.4f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry();
		// Centro
		model = modelPie2;
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.4f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry();
		// Derecha
		model = modelPie2;
		model = glm::translate(model, glm::vec3(-0.3f, -0.1f, 0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.4f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry();



		// ====================================================================================================
		// 7. PATA 3: TRASERA IZQUIERDA
		// ====================================================================================================
		// ---- ARTICULACION 31 (CADERA: Teclas 1-2) ----------------------------------------------------------
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.8f, -0.4f, -3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion31()), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 modelCadera3 = model;
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		sp.render();
		// ---- PIERNA SUPERIOR 3 ----------------------------------------------------------------------------
		model = modelCadera3;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 3.0f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatasSup)); // Aplicamos nuevo color
		meshList[2]->RenderMeshGeometry();
		// ---- ARTICULACION 32 (RODILLA: Teclas H-N) --------------------------------------------------------- 
		model = modelCadera3;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion32()), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 modelRodilla3 = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		sp.render();
		// ---- PIERNA INFERIOR 3 ----------------------------------------------------------------------------
		model = modelRodilla3;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 3.0f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatasInf)); // Aplicamos nuevo color
		meshList[2]->RenderMeshGeometry();
		// ---- PLANTA DEL PIE 3 -----------------------------------------------------------------------------
		model = modelRodilla3;
		model = glm::translate(model, glm::vec3(0.0f, -3.2f, 0.4f));
		glm::mat4 modelPie3 = model;
		model = glm::scale(model, glm::vec3(0.8f, 0.4f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();
		// ---- GARRAS 3 -------------------------------------------------------------------------------------
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorNegro));
		// Izquierda
		model = modelPie3;
		model = glm::translate(model, glm::vec3(0.3f, -0.1f, 0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.4f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry();
		// Centro
		model = modelPie3;
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.4f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry();
		// Derecha
		model = modelPie3;
		model = glm::translate(model, glm::vec3(-0.3f, -0.1f, 0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.4f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry();



		// ====================================================================================================
		// 8. PATA 4: TRASERA DERECHA
		// ====================================================================================================
		// ---- ARTICULACION 41 (CADERA: Teclas 9-0) ----------------------------------------------------------
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.8f, -0.4f, -3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion41()), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 modelCadera4 = model;
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		sp.render();
		// ---- PIERNA SUPERIOR 4 ----------------------------------------------------------------------------
		model = modelCadera4;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 3.0f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatasSup)); // Aplicamos nuevo color
		meshList[2]->RenderMeshGeometry();
		// ---- ARTICULACION 42 (RODILLA: Teclas J-M) ---------------------------------------------------------
		model = modelCadera4;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion42()), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 modelRodilla4 = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		sp.render();
		// ---- PIERNA INFERIOR 4 ----------------------------------------------------------------------------
		model = modelRodilla4;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 3.0f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatasInf)); // Aplicamos nuevo color
		meshList[2]->RenderMeshGeometry();
		// ---- PLANTA DEL PIE 4 -----------------------------------------------------------------------------
		model = modelRodilla4;
		model = glm::translate(model, glm::vec3(0.0f, -3.2f, 0.4f));
		glm::mat4 modelPie4 = model;
		model = glm::scale(model, glm::vec3(0.8f, 0.4f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();
		// ---- GARRAS 4 --------------------------------------------------------------------------------------
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorNegro));
		// Izquierda
		model = modelPie4;
		model = glm::translate(model, glm::vec3(0.3f, -0.1f, 0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.4f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry();
		// Centro
		model = modelPie4;
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.4f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry();
		// Derecha
		model = modelPie4;
		model = glm::translate(model, glm::vec3(-0.3f, -0.1f, 0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.4f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry();


		/*
		
		// *****************************************************************************************
		// *****************************  MODELADO JERÁRQUICO: GRUA  *******************************
		// *****************************************************************************************

		// ========================================================
		// CAMBIO 6: NODO RAÍZ: CABINA
		// ========================================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -4.0f));
		modelaux = model; // GUARDAMOS: Centro de la cabina (padre)
		model = glm::scale(model, glm::vec3(4.0f, 2.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.859f, 0.533f, 0.318f); // Naranja base
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		// --------------------------------------------------------
		// CAMBIO 7: BASE Y LLANTAS (Heredan de la Cabina)
		// --------------------------------------------------------
		// ----	BASE PIRÁMIDE (Hijo de Cabina)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f)); // Bajamos la base
		glm::mat4 modelBaseCab = model; // GUARDAMOS: Centro de la base (padre de llantas)
		model = glm::scale(model, glm::vec3(3.0f, 1.5f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.3f, 0.3f, 0.3f); // Gris oscuro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMeshGeometry();

		// ----	LLANTAS (Hijas de la Base)
		glm::vec3 escalaLlanta = glm::vec3(0.8f, 0.8f, 1.0f);
		glm::vec3 colorLlanta = glm::vec3(0.188f, 0.188f, 0.184f);
		//		Llanta 1: Delantera Izquierda (Tecla K)
		model = modelBaseCab;
		model = glm::translate(model, glm::vec3(-1.0f, -0.5f, 2.3f));
		model = glm::rotate(model, glm::radians(mainWindow.getLlanta1()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, escalaLlanta);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		meshList[2]->RenderMeshGeometry();
		//		Llanta 2: Delantera Derecha (Tecla L)
		model = modelBaseCab;
		model = glm::translate(model, glm::vec3(1.0f, -0.5f, 2.3f));
		model = glm::rotate(model, glm::radians(mainWindow.getLlanta2()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, escalaLlanta);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		meshList[2]->RenderMeshGeometry();
		//		Llanta 3: Trasera Izquierda (Tecla I)
		model = modelBaseCab;
		model = glm::translate(model, glm::vec3(-1.0f, -0.5f, -2.3f));
		model = glm::rotate(model, glm::radians(mainWindow.getLlanta3()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, escalaLlanta);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		meshList[2]->RenderMeshGeometry();
		//		Llanta 4: Trasera Derecha (Tecla O)
		model = modelBaseCab;
		model = glm::translate(model, glm::vec3(1.0f, -0.5f, -2.3f));
		model = glm::rotate(model, glm::radians(mainWindow.getLlanta4()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, escalaLlanta);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		meshList[2]->RenderMeshGeometry();


		// --------------------------------------------------------
		// CAMBIO 8: DETALLES DE LA CABINA (Heredan de la Cabina)
		// --------------------------------------------------------
		glm::vec3 colorVentana = glm::vec3(0.725f, 0.863f, 0.922f); // Azul vidrio
		// ----	CONTRAPESO TRASERO
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.5f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 2.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.388f, 0.345f, 0.314f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		// ----	VENTANA FRONTAL
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.0f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 1.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(colorVentana);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		// ----	VENTANA DERECHA
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, 1.51f));
		model = glm::scale(model, glm::vec3(3.5f, 1.0f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(colorVentana);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		// ----	VENTANA IZQUIERDA
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -1.51f));
		model = glm::scale(model, glm::vec3(3.5f, 1.0f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(colorVentana);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		// ----	CILINDRO FRONTAL (Motor)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.0f, -0.7f, 0.0f)); // Justo debajo de la ventana
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.6f, 3.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.859f, 0.533f, 0.318f); // Mismo color naranja de la cabina
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();


		// --------------------------------------------------------
		// CAMBIO 9: BRAZO ARTICULADO (Hijos)
		// --------------------------------------------------------
		// ----	ARTICULACIÓN 1 Y BRAZO 1 (Hijo de Cabina)
		model = modelaux; // Partimos nuevamente desde la cabina
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		modelaux = model; // GUARDAMOS: Centro Brazo 1
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.710f, 0.502f, 0.361f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		// ----	ARTICULACIÓN 2 (Hijo de Brazo 1)
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // GUARDAMOS: Pivote Articulación 2
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		// ----	BRAZO 2 (Hijo de Articulación 2)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		modelaux = model; // GUARDAMOS: Centro Brazo 2
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.788f, 0.639f, 0.482f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		// ----	ARTICULACIÓN 3 (Hijo de Brazo 2)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // GUARDAMOS: Pivote Articulación 3
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		// ----	BRAZO 3 (Hijo de Articulación 3)
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		modelaux = model; // GUARDAMOS: Centro Brazo 3
		model = glm::scale(model, glm::vec3(4.0f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.890f, 0.757f, 0.561f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		// ----	ARTICULACIÓN 4 (Hijo de Brazo 3)
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // GUARDAMOS: Pivote Articulación 4
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		// ----	CANASTA (Hijo de Articulación 4)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		modelaux = model; // GUARDAMOS: Centro Canasta 
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.541f, 0.455f, 0.325f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		*/

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}