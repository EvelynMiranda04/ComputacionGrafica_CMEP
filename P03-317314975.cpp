//práctica 3: Modelado Geométrico y Cámara Sintética.
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
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
// ------------------------------------------------------------------------------------------------------------
// V2 ------ CAMBIO 1: Agregamos el vector MeshColor
vector<MeshColor*> meshColorList;
// ------------------------------------------------------------------------------------------------------------

//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

// CREACIÓN DE PRIMITIVAS GEOMÉTRICAS -----------------------------
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


// ------------------------------------------------------------------------------------------------------------
// V1 ------ CAMBIO 1: Modificamos los vertices de la piramide triangular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
	0, 1, 2, // Base
	1, 3, 2, // Cara lateral derecha
	3, 0, 2, // Cara lateral izquierda
	1, 0, 3  // Cara frontal
	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.2041f,  0.2886f, // 0: Base - Frente Izquierda
		 0.5f, -0.2041f,  0.2886f, // 1: Base - Frente Derecha
		 0.0f, -0.2041f, -0.5773f, // 2: Base - Atrás
		 0.0f,  0.6124f,  0.0f     // 3: Punta superior
	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);
}
// ------------------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------------------
// V2 ------ CAMBIO 2: Función para crear piezas del Pyraminx con colores personalizados
void CrearPiezaPyraminx(glm::vec3 cFrente, glm::vec3 cDer, glm::vec3 cIzq, glm::vec3 cBase)
{
	GLfloat vertices[] = {
		// CARA FRONTAL (cFrente)
		-0.5f, -0.2041f,  0.2886f,   cFrente.r, cFrente.g, cFrente.b,
		 0.5f, -0.2041f,  0.2886f,   cFrente.r, cFrente.g, cFrente.b,
		 0.0f,  0.6124f,  0.0f,      cFrente.r, cFrente.g, cFrente.b,

		 // CARA DERECHA (cDer)
		  0.5f, -0.2041f,  0.2886f,   cDer.r, cDer.g, cDer.b,
		  0.0f, -0.2041f, -0.5773f,   cDer.r, cDer.g, cDer.b,
		  0.0f,  0.6124f,  0.0f,      cDer.r, cDer.g, cDer.b,

		  // CARA IZQUIERDA (cIzq)
		   0.0f, -0.2041f, -0.5773f,   cIzq.r, cIzq.g, cIzq.b,
		  -0.5f, -0.2041f,  0.2886f,   cIzq.r, cIzq.g, cIzq.b,
		   0.0f,  0.6124f,  0.0f,      cIzq.r, cIzq.g, cIzq.b,

		   // BASE (cBase)
		   -0.5f, -0.2041f,  0.2886f,   cBase.r, cBase.g, cBase.b,
			0.0f, -0.2041f, -0.5773f,   cBase.r, cBase.g, cBase.b,
			0.5f, -0.2041f,  0.2886f,   cBase.r, cBase.g, cBase.b
	};

	MeshColor* pieza = new MeshColor();
	// 12 vértices * 6 datos (XYZ RGB) = 72 floats en total
	pieza->CreateMeshColor(vertices, 72); // <--- CAMBIA ESTO A 72
	meshColorList.push_back(pieza);
}
// ------------------------------------------------------------------------------------------------------------


//Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
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
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
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
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res, float R) {
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
	for (i = 0; i < res + 2; i++) indices.push_back(i);
	//se genera el mesh del cono
	Mesh* cono = new Mesh();
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
	Mesh* piramide = new Mesh();
	//Nuevo Mesh, ahora se usa el triangle_fan
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(15, 1.0f);//índice 2 en MeshList
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



	// ------------------------------------------------------------------------------------------------------------
	// V1/2 ------	CAMBIO 2/3:Se define la paleta de colores
	glm::vec3 cVerde = glm::vec3(0.141f, 0.310f, 0.227f);
	glm::vec3 cRojo = glm::vec3(0.310f, 0.141f, 0.184f);
	glm::vec3 cAzul = glm::vec3(0.141f, 0.204f, 0.310f);
	glm::vec3 cAmarillo = glm::vec3(0.471f, 0.455f, 0.208f);
	glm::vec3 cNegro = glm::vec3(0.110f, 0.102f, 0.102f); // Para las caras internas
	// ------------------------------------------------------------------------------------------------------------

	// ------------------------------------------------------------------------------------------------------------
	// V2 ------ CAMBIO 4: Creación de las piezas del Pyraminx con colores personalizados. Se crean 14 piezas: 
	//						4 esquinas, 6 aristas y 4 centros
	// GRUPO 1: LAS 4 ESQUINAS (PUNTAS): Orden de parámetros: Frente, Derecha, Izquierda, Base
	CrearPiezaPyraminx(cVerde, cRojo, cAzul, cNegro);		// 0. Punta Superior (Tiene Verde, Rojo y Azul)
	CrearPiezaPyraminx(cVerde, cNegro, cAzul, cAmarillo);	// 1. Esquina Inf Izq (Verde, Azul, Amarillo)
	CrearPiezaPyraminx(cVerde, cRojo, cNegro, cAmarillo);	// 2. Esquina Inf Der (Verde, Rojo, Amarillo)
	CrearPiezaPyraminx(cNegro, cRojo, cAzul, cAmarillo);	// 3. Esquina Trasera (Rojo, Azul, Amarillo)
	// GRUPO 2: LAS 6 ARISTAS (PIEZAS DE BORDE) ---
	CrearPiezaPyraminx(cVerde, cRojo, cNegro, cNegro);		// 4. Arista Frontal-Derecha (Verde y Rojo)
	CrearPiezaPyraminx(cVerde, cNegro, cAzul, cNegro);		// 5. Arista Frontal-Izquierda (Verde y Azul)
	CrearPiezaPyraminx(cNegro, cRojo, cAzul, cNegro);		// 6. Arista Trasera (Rojo y Azul)
	CrearPiezaPyraminx(cVerde, cNegro, cNegro, cAmarillo);	// 7. Arista Base-Frontal (Verde y Amarillo)
	CrearPiezaPyraminx(cNegro, cRojo, cNegro, cAmarillo);	// 8. Arista Base-Derecha (Rojo y Amarillo)
	CrearPiezaPyraminx(cNegro, cNegro, cAzul, cAmarillo);	// 9. Arista Base-Izquierda (Azul y Amarillo)
	// GRUPO 3: LOS 4 CENTROS (LA CARA VISIBLE SERÁ LA BASE) ---
	CrearPiezaPyraminx(cNegro, cNegro, cNegro, cVerde);		// 10. Centro Frente (El Verde va en la BASE)
	CrearPiezaPyraminx(cNegro, cNegro, cNegro, cRojo);		// 11. Centro Derecha (El Rojo va en la BASE)
	CrearPiezaPyraminx(cNegro, cNegro, cNegro, cAzul);		// 12. Centro Izquierda (El Azul va en la BASE)
	CrearPiezaPyraminx(cNegro, cNegro, cNegro, cAmarillo);	// 13. Centro Base (El Amarillo va en la BASE)
	CrearPiezaPyraminx(cNegro, cNegro, cNegro, cNegro);		// 14. NÚCLEO NEGRO SÓLIDO (Para rellenar el interior)
	// ------------------------------------------------------------------------------------------------------------


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
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader
	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;


	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		//Limpiar la ventana
		glClearColor(0.859f, 0.859f, 0.859f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad



		/*
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		// =======================================================================================================
		// ======================================== VERSION 1 - CAMBIO 3 =========================================

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));	//(Lente de la cámara)
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix())); //(Pos.Rot. d cámara)

		// PASO 1. Matriz BASE para todo el Pyraminx (Posición y Rotaciones, SIN ESCALA)
		glm::mat4 modelBase = glm::mat4(1.0f);
		modelBase = glm::translate(modelBase, glm::vec3(0.0f, 0.0f, -5.0f));
		modelBase = glm::rotate(modelBase, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelBase = glm::rotate(modelBase, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelBase = glm::rotate(modelBase, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

		// PASO 2: EL NÚCLEO NEGRO Para simular fondo
		glm::mat4 modelCore = glm::scale(modelBase, glm::vec3(3.0f, 3.0f, 3.0f));	//Escala de 3
		glUniform3fv(uniformColor, 1, glm::value_ptr(cNegro));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCore));
		meshList[1]->RenderMesh();

		// PASO 3: DIBUJANDO CARA FRONTAL (VERDE)
		float posXZ[9][2] = {
			{ 0.0f, -1.1547f}, {-0.5f, -0.2887f}, { 0.5f, -0.2887f}, // 3 Arriba
			{-1.0f,  0.5773f}, { 0.0f,  0.5773f}, { 1.0f,  0.5773f}, // 3 Abajo
			{ 0.0f, -0.5774f}, {-0.5f,  0.2887f}, { 0.5f,  0.2887f}  // 3 Invertidas
		};
		glUniform3fv(uniformColor, 1, glm::value_ptr(cVerde));
		// Usamos modelBase para que los triángulos conserven su tamaño 1.0 original
		glm::mat4 modelCaraFrontal = modelBase;
		modelCaraFrontal = glm::rotate(modelCaraFrontal, glm::radians(70.5288f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelCaraFrontal = glm::translate(modelCaraFrontal, glm::vec3(0.0f, 0.68f, 0.0f)); //0.61 es al ras
		// Repetimos proceso 9 veces
		for (int i = 0; i < 9; i++) {
			glm::mat4 modelPieza = modelCaraFrontal;
			modelPieza = glm::translate(modelPieza, glm::vec3(posXZ[i][0], 0.0f, posXZ[i][1]));
			modelPieza = glm::rotate(modelPieza, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			modelPieza = glm::translate(modelPieza, glm::vec3(0.0f, 0.2041f, 0.0f));
			if (i >= 6) {
				modelPieza = glm::rotate(modelPieza, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
			modelPieza = glm::scale(modelPieza, glm::vec3(0.92f, 0.92f, 0.92f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPieza));
			meshList[1]->RenderMesh();
		}

		// PASO 4: DIBUJANDO CARA DERECHA (ROJA)
		glUniform3fv(uniformColor, 1, glm::value_ptr(cRojo));
		glm::mat4 modelCaraDerecha = modelBase;
		// Rotamos la cara entera 120 grados para ponerla a un lado
		modelCaraDerecha = glm::rotate(modelCaraDerecha, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelCaraDerecha = glm::rotate(modelCaraDerecha, glm::radians(70.5288f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelCaraDerecha = glm::translate(modelCaraDerecha, glm::vec3(0.0f, 0.68f, 0.0f));
		for (int i = 0; i < 9; i++) {
			glm::mat4 modelPieza = modelCaraDerecha;
			modelPieza = glm::translate(modelPieza, glm::vec3(posXZ[i][0], 0.0f, posXZ[i][1]));
			modelPieza = glm::rotate(modelPieza, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			modelPieza = glm::translate(modelPieza, glm::vec3(0.0f, 0.2041f, 0.0f));
			if (i >= 6) {
				modelPieza = glm::rotate(modelPieza, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
			modelPieza = glm::scale(modelPieza, glm::vec3(0.92f, 0.92f, 0.92f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPieza));
			meshList[1]->RenderMesh();
		}

		// PASO 5: DIBUJANDO CARA IZQUIERDA (AZUL)
		glUniform3fv(uniformColor, 1, glm::value_ptr(cAzul));
		glm::mat4 modelCaraIzquierda = modelBase;
		// 240 grados para posicionarla a la izquierda
		modelCaraIzquierda = glm::rotate(modelCaraIzquierda, glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelCaraIzquierda = glm::rotate(modelCaraIzquierda, glm::radians(70.5288f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelCaraIzquierda = glm::translate(modelCaraIzquierda, glm::vec3(0.0f, 0.68f, 0.0f));
		for (int i = 0; i < 9; i++) {
			glm::mat4 modelPieza = modelCaraIzquierda;
			modelPieza = glm::translate(modelPieza, glm::vec3(posXZ[i][0], 0.0f, posXZ[i][1]));
			modelPieza = glm::rotate(modelPieza, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			modelPieza = glm::translate(modelPieza, glm::vec3(0.0f, 0.2041f, 0.0f));
			if (i >= 6) {
				modelPieza = glm::rotate(modelPieza, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
			modelPieza = glm::scale(modelPieza, glm::vec3(0.92f, 0.92f, 0.92f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPieza));
			meshList[1]->RenderMesh();
		}

		// PASO 6: DIBUJANDO CARA DE ABAJO (AMARILLA)
		glUniform3fv(uniformColor, 1, glm::value_ptr(cAmarillo));
		glm::mat4 modelCaraAbajo = modelBase;
		// Giramos 180 grados sobre el eje Z.
		modelCaraAbajo = glm::rotate(modelCaraAbajo, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelCaraAbajo = glm::translate(modelCaraAbajo, glm::vec3(0.0f, 0.68f, 0.0f));
		for (int i = 0; i < 9; i++) {
			glm::mat4 modelPieza = modelCaraAbajo;
			modelPieza = glm::translate(modelPieza, glm::vec3(posXZ[i][0], 0.0f, posXZ[i][1]));
			modelPieza = glm::rotate(modelPieza, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			modelPieza = glm::translate(modelPieza, glm::vec3(0.0f, 0.2041f, 0.0f));
			if (i >= 6) {
				modelPieza = glm::rotate(modelPieza, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
			modelPieza = glm::scale(modelPieza, glm::vec3(0.92f, 0.92f, 0.92f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPieza));
			meshList[1]->RenderMesh();
		}
		// =======================================================================================================
		*/






		/**/
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		// =======================================================================================================
		// ======================================== VERSION 2 - CAMBIO 5 =========================================
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));	//(Lente de la cámara)
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix())); //(Pos.Rot. d cámara)

		// PASO 1. Matriz BASE para todo el Pyraminx (Posición y Rotaciones, SIN ESCALA)
		glm::mat4 modelBase = glm::mat4(1.0f);
		modelBase = glm::translate(modelBase, glm::vec3(0.0f, -0.5f, -4.0f));
		modelBase = glm::rotate(modelBase, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelBase = glm::rotate(modelBase, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelBase = glm::rotate(modelBase, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

		// PASO 2. Creación de constantes matemáticas de apoyo para posicionar las piezas. 
		// Las definimos así para evitar repetir cálculos trigonométricos en cada pieza
		float H = 0.8165f;
		float dZ1 = 0.2887f;
		float dZ2 = 0.5774f;
		float s = 0.90f;

		// PASO 3. DIBUJANDO PIEZAS DEL PYRAMINX 
		// ------- GRUPO 0: NÚCLEO NEG
		glm::mat4 modelCore = modelBase;
		// Ajuste exacto para alinear el centro del núcleo con el centro de las 14 piezas
		modelCore = glm::translate(modelCore, glm::vec3(0.0f, -0.4083f, 0.0f));
		// Escala de 2.85 (El Pyraminx total mide 3.0)
		modelCore = glm::scale(modelCore, glm::vec3(2.85f, 2.85f, 2.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCore));
		meshColorList[14]->RenderMeshColor(); // Dibujamos la pieza 14 (Toda negra)
		// ------- GRUPO 1: 10 PIEZAS VERTICALES (ESQUINAS Y ARISTAS)
		glm::vec3 posiciones[10] = {
			glm::vec3(0.0f,  H, 0.0f),         // 0: Punta Superior
			glm::vec3(-1.0f, -H, dZ2),         // 1: Esq Inf Izq
			glm::vec3(1.0f, -H, dZ2),          // 2: Esq Inf Der
			glm::vec3(0.0f, -H, -1.1547f),     // 3: Esq Trasera
			glm::vec3(0.5f,  0.0f, dZ1),       // 4: Arista Front-Der 
			glm::vec3(-0.5f, 0.0f, dZ1),       // 5: Arista Front-Izq 
			glm::vec3(0.0f,  0.0f, -dZ2),      // 6: Arista Trasera
			glm::vec3(0.0f, -H, dZ2),          // 7: Arista Base Front
			glm::vec3(0.5f, -H, -dZ1),         // 8: Arista Base Der 
			glm::vec3(-0.5f, -H, -dZ1)         // 9: Arista Base Izq 
		};
		for (int i = 0; i < 10; i++) {
			glm::mat4 model = modelBase;
			model = glm::translate(model, posiciones[i]);
			model = glm::scale(model, glm::vec3(s, s, s)); // Escala desde el centro para el marco negro
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshColorList[i]->RenderMeshColor();
		}
		// ------- GRUPO 2: 12 PIEZAS DE RELLENO (HUECOS INVERTIDOS)
		//					A) HUECOS DE LAS 3 CARAS LATERALES
		glm::vec3 huecosCara[3] = {
			glm::vec3(0.0f, 0.2722f, 0.1925f),    // Hueco Superior
			glm::vec3(-0.5f, -0.5443f, 0.4811f),  // Hueco Inferior Izquierdo
			glm::vec3(0.5f, -0.5443f, 0.4811f)    // Hueco Inferior Derecho
		};
		float angulosY[3] = { 0.0f, 120.0f, -120.0f }; // Frente, Derecha, Izquierda
		int indicesColor[3] = { 10, 11, 12 };          // Reutilizamos tus piezas 10, 11 y 12
		for (int cara = 0; cara < 3; cara++) {
			for (int h = 0; h < 3; h++) {
				glm::mat4 modelC = modelBase;
				// Rotar la cara hacia su lado correspondiente
				modelC = glm::rotate(modelC, glm::radians(angulosY[cara]), glm::vec3(0.0f, 1.0f, 0.0f));
				modelC = glm::translate(modelC, huecosCara[h]);
				// Acostar la pieza para que la base embone en la cara
				modelC = glm::rotate(modelC, glm::radians(-109.471f), glm::vec3(1.0f, 0.0f, 0.0f));
				modelC = glm::scale(modelC, glm::vec3(s, s, s));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
				meshColorList[indicesColor[cara]]->RenderMeshColor();
			}
		}
		//				  B) HUECOS DE LA CARA BASE (Amarillo)
		glm::vec3 huecosBase[3] = {
			glm::vec3(-0.5f, -H, dZ1),    // Hueco Izquierdo
			glm::vec3(0.5f, -H, dZ1),     // Hueco Derecho
			glm::vec3(0.0f, -H, -dZ2)     // Hueco Trasero
		};
		for (int h = 0; h < 3; h++) {
			glm::mat4 modelC = modelBase;
			modelC = glm::translate(modelC, huecosBase[h]);
			// Rotar 180 en el eje Y para que los picos del triángulo encajen al revés
			modelC = glm::rotate(modelC, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelC = glm::scale(modelC, glm::vec3(s, s, s));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelC));
			meshColorList[13]->RenderMeshColor(); // Pieza 13 (Amarillo)
		}
		// =======================================================================================================
		

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}