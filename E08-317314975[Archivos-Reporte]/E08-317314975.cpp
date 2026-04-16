/*
Práctica 7: Iluminación 1
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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

Skybox skybox;

// ====================================================================================
// Modelos del Coche
Model LlantaT;
Model CofreT;
Model CocheT;
Model Lampara_M;
Texture OctaedroTexture;
// ====================================================================================

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

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

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {

		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// AQUI SE DEBE MODIFICAR, NUESTRA CARA QUE TIENE TRANSPARENCIA SE PONE SIEMPRE HASTA EL FINAL
		// En caso de que todo sea traslucido, deberemos aplicar blending, verificar bibliografia y seria a prueba y error
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};

	// ====================================================================================
	// Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
	// average normals
	GLfloat cubo_vertices[] = {
		// front [VACAS]
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.76f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.99f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.99f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.76f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
		// right [CATARINAS]
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.01f,  0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.24f,	0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.24f,	0.66f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.01f,	0.66f,		-1.0f,	0.0f,	0.0f,
		// back [PULPOS]
		-0.5f, -0.5f, -0.5f,	0.48f,  0.34f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.26f,	0.34f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.26f,	0.66f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.48f,	0.66f,		0.0f,	0.0f,	1.0f,

		// left [ELEFANTES]
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.51f,  0.34f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.74f,	0.34f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.74f,	0.66f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.51f,	0.66f,		1.0f,	0.0f,	0.0f,

		// bottom [CERDITOS]
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.74f,  0.32f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.74f,	0.01f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.51f,	0.01f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.51f,	0.32f,		0.0f,	1.0f,	0.0f,

		//UP [PERICOS]
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.74f,  0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.74f,	1.0f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.51f,	1.0f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.51f,	0.68f,		0.0f,	-1.0f,	0.0f,
	};
	// ====================================================================================

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

// ====================================================================================
void CrearOctaedro()
{
	unsigned int octa_indices[] = {
		// --- MITAD SUPERIOR ---
		0, 1, 2,   // Cara 1: Superior Frontal-Derecha
		3, 4, 5,   // Cara 2: Superior Trasera-Derecha
		6, 7, 8,   // Cara 3: Superior Trasera-Izquierda
		9, 10, 11, // Cara 4: Superior Frontal-Izquierda

		// --- MITAD INFERIOR ---
		12, 13, 14, // Cara 5: Inferior Frontal-Derecha
		15, 16, 17, // Cara 6: Inferior Trasera-Derecha
		18, 19, 20, // Cara 7: Inferior Trasera-Izquierda
		21, 22, 23  // Cara 8: Inferior Frontal-Izquierda
	};

	GLfloat octa_vertices[] = {
		// x       y       z         S       T          NX        NY        NZ
		// ================= MITAD SUPERIOR (Triángulos apuntando hacia arriba) =================
		// Cara 1: Superior Frontal-Derecha (+, +, +)
		0.0f,   0.5f,   0.0f,		0.32f,	0.20f,      0.577f,   0.577f,   0.577f,
		0.0f,   0.0f,   0.5f,		0.01f,	0.39f,      0.577f,   0.577f,   0.577f,
		0.5f,   0.0f,   0.0f,		0.01f,  0.01f,      0.577f,   0.577f,   0.577f,

		// Cara 2: Superior Trasera-Derecha (+, +, -)
		0.0f,   0.5f,   0.0f,		0.33f,	0.21f,      0.577f,   0.577f,  -0.577f,
		0.5f,   0.0f,   0.0f,		0.33f,  0.59f,      0.577f,   0.577f,  -0.577f,
		0.0f,   0.0f,  -0.5f,		0.01f,	0.41f,      0.577f,   0.577f,  -0.577f,

		// Cara 3: Superior Trasera-Izquierda (-, +, -)
		0.0f,   0.5f,   0.0f,		0.34f,	0.21f,     -0.577f,   0.577f,  -0.577f,
		0.0f,   0.0f,  -0.5f,		0.65f,	0.40f,     -0.577f,   0.577f,  -0.577f,
		-0.5f,   0.0f,   0.0f,		0.34f,	0.59f,     -0.577f,   0.577f,  -0.577f,

		// Cara 4: Superior Frontal-Izquierda (-, +, +)
		0.0f,   0.5f,   0.0f,		0.35f,	0.21f,     -0.577f,   0.577f,   0.577f,
		-0.5f,   0.0f,   0.0f,		0.67f,	0.01f,     -0.577f,   0.577f,   0.577f,
		0.0f,   0.0f,   0.5f,		0.67f,	0.40f,     -0.577f,   0.577f,   0.577f,

		// ================= MITAD INFERIOR (Triángulos apuntando hacia abajo) =================

		// Cara 5: Inferior Frontal-Izquierda (-, -, +)
		0.0f,  -0.5f,   0.0f,		0.66f,	0.80f,     -0.577f,  -0.577f,   0.577f,
		0.0f,   0.0f,   0.5f,		0.33f,	0.99f,     -0.577f,  -0.577f,   0.577f,
		-0.5f,   0.0f,   0.0f,		0.33f,	0.61f,     -0.577f,  -0.577f,   0.577f,

		// Cara 7: Inferior Trasera-Derecha (+, -, -)
		0.0f,  -0.5f,   0.0f,		0.67f,	0.79f,      0.577f,  -0.577f,  -0.577f,
		0.0f,   0.0f,  -0.5f,		0.67f,	0.41f,      0.577f,  -0.577f,  -0.577f,
		0.5f,   0.0f,   0.0f,		0.98f,	0.60f,      0.577f,  -0.577f,  -0.577f,

		// Cara 6: Inferior Frontal-Derecha (+, -, +)
		0.0f,  -0.5f,   0.0f,		0.66f,	0.79f,      0.577f,  -0.577f,   0.577f,
		0.5f,   0.0f,   0.0f,		0.34f,	0.61f,      0.577f,  -0.577f,   0.577f,
		0.0f,   0.0f,   0.5f,		0.66f,	0.41f,      0.577f,  -0.577f,   0.577f,

		// Cara 8: Inferior Trasera-Izquierda (-, -, -)
		0.0f,  -0.5f,   0.0f,		0.67f,	0.80f,     -0.577f,  -0.577f,  -0.577f,
		-0.5f,   0.0f,   0.0f,		0.99f,	0.61f,     -0.577f,  -0.577f,  -0.577f,
		0.0f,   0.0f,  -0.5f,		0.99f,	0.99f,     -0.577f,  -0.577f,  -0.577f
	};

	Mesh* octaedro = new Mesh();
	// Son 24 vértices en total, por 8 datos cada uno (x,y,z, s,t, nx,ny,nz) = 192.
	// Y 24 índices que conforman los 8 triángulos.
	octaedro->CreateMesh(octa_vertices, octa_indices, 192, 24);
	meshList.push_back(octaedro); // Este será el índice [5] en tu meshList
}
// ====================================================================================

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CreateShaders();
	// ====================================================================================
	CrearOctaedro();
	// ====================================================================================


	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado_animales2.tga");
	dadoTexture.LoadTextureA();
	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	// ====================================================================================
	// CARGA DE MODELOS
	LlantaT = Model();
	LlantaT.LoadModel("Models/llantaT1.obj");
	CocheT = Model();
	CocheT.LoadModel("Models/CocheT.obj");
	CofreT = Model();
	CofreT.LoadModel("Models/CofreT.obj");
	Lampara_M = Model();
	Lampara_M.LoadModel("Models/LamparaOBJ.obj");

	OctaedroTexture = Texture("Textures/OCTAEDRO.tga");
	OctaedroTexture.LoadTextureA();
	// ====================================================================================

	// SKYBOX Y MATERIALES
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	// ====================================================================================
	// CONFIGURACIÓN DE LUCES (Ordenadas: Sol -> Puntuales -> Spot)
	// ====================================================================================

	// 1. LUZ DE "SOL" (Direccional - sólo 1 y siempre debe de existir)
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.1f, 0.1f,						// Bajamos intensidad
		0.0f, -1.0f, 0.0f);

	// ----------------------------------------------------------
	// 2. LUCES PUNTUALES (Point Lights)
	unsigned int pointLightCount = 0;
	// Luz de la Lámpara Hextech (Luz puntual BLANCA)
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f, 		// Color Blanco (RGB)
		1.0f, 1.0f,										// Intensidad (alta para que ilumine bien)
		0.0f, 5.75f, -7.0f,								// Posición: Misma X, Y, Z
		1.0f, 0.09f, 0.032f);							// Atenuación para difuminado realista
	pointLightCount++;


	// ----------------------------------------------------------
	// 3. LUCES SPOT (Linternas y Faros)
	unsigned int spotLightCount = 0;
	// Índice 0: Luz tipo linterna/Cono color blanco (Vinculada a cámara)
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	// Índice 1: Faro AZUL (Coche) - Este será sobreescrito en el while
	spotLights[1] = SpotLight(0.035f, 0.318f, 0.600f, 	// Color Azul (RGB)
		1.0f, 1.0f,										// Intensidad máxima en 1.0f
		0.0f, 0.0f, 0.0f,								// Posición inicial (se actualiza en while)
		1.0f, -0.2f, 0.0f,								// Dirección
		1.0f, 0.045f, 0.0075f,							// Valores de atenuación
		90.0f);											// Ángulo de apertura
	spotLightCount++;

	// Índice 2: Faro AMARILLO (Helicóptero)
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f, 		// Color Amarillo (RGB)
		1.0f, 1.0f,										// Intensidad
		0.0f, 0.0f, 0.0f,								// Posición inicial (se actualiza en while)
		0.0f, -1.0f, 0.0f,								// Dirección hacia abajo (piso)
		1.0f, 0.045f, 0.0075f,							// Atenuación
		50.0f);											// Ángulo de apertura cerrado
	spotLightCount++;


	/*
	// ============================================================
	// PREVIO 07: PAR DE ARREGLOS DE LUCES PUNTUALES CAMBIANTES
	// OTRA FORMA DE HACERLO ERA PONER EN EL ARREGLO 2 DIRECTAMENTE LOS VALORES DEL ARREGLO 1 PERO EN DIFERENTE ORDEN
	PointLight pointLights_Arr1[4];
	PointLight pointLights_Arr2[4];
	// Separación amplia y atenuación suave
	GLfloat pY = 2.0f, pZ = 5.0f;
	GLfloat intAmb = 0.2f; // Intensidad ambiente más baja
	GLfloat intDif = 0.6f; // Intensidad difusa más baja
	GLfloat con = 1.0f, lin = 0.09f, exp = 0.032f; // Atenuación realista
	// ARREGLO 1 (Verde, Azul, Roja, Blanca)
	pointLights_Arr1[0] = PointLight(0.0f, 1.0f, 0.0f, intAmb, intDif, -15.0f, pY, pZ, con, lin, exp); // Verde
	pointLights_Arr1[1] = PointLight(0.0f, 0.0f, 1.0f, intAmb, intDif, -5.0f, pY, pZ, con, lin, exp); // Azul
	pointLights_Arr1[2] = PointLight(1.0f, 0.0f, 0.0f, intAmb, intDif, 5.0f, pY, pZ, con, lin, exp); // Roja
	pointLights_Arr1[3] = PointLight(1.0f, 1.0f, 1.0f, intAmb, intDif, 15.0f, pY, pZ, con, lin, exp); // Blanca
	// ARREGLO 2 (Blanca, Verde, Azul, Roja)
	pointLights_Arr2[0] = PointLight(1.0f, 1.0f, 1.0f, intAmb, intDif, -15.0f, pY, pZ, con, lin, exp); // Blanca
	pointLights_Arr2[1] = PointLight(0.0f, 1.0f, 0.0f, intAmb, intDif, -5.0f, pY, pZ, con, lin, exp); // Verde
	pointLights_Arr2[2] = PointLight(0.0f, 0.0f, 1.0f, intAmb, intDif, 5.0f, pY, pZ, con, lin, exp); // Azul
	pointLights_Arr2[3] = PointLight(1.0f, 0.0f, 0.0f, intAmb, intDif, 15.0f, pY, pZ, con, lin, exp); // Roja
	pointLightCount = 4;
	// ============================================================
	*/

	// ====================================================================================
	// VARIABLES DE SHADER Y MATRICES
	// ====================================================================================
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	// ====================================================================================
	// DECLARACIONES MOVIDAS FUERA DEL CICLO (Recomendación del profesor)
	// ====================================================================================
	GLfloat now = 0.0f;

	glm::vec3 posHelicoptero;
	glm::vec3 posCoche;
	glm::mat4 matrizHeliLuz;
	// La posición local no va a cambiar nunca respecto a su centro, la definimos aquí
	glm::vec4 posLocalHeli = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 posGlobalHeli;

	glm::mat4 matrizCocheLuz;
	glm::vec4 posicionLocalFaroCoche = glm::vec4(8.8f, 2.1f, 0.0f, 1.0f);
	glm::vec4 posicionGlobalFaroCoche;

	glm::vec3 lowerLight;
	glm::mat4 model;
	glm::mat4 modelaux;
	glm::vec3 color;

	// ====================================================================================
	// ARREGLO AUXILIAR DE FAROS (Aqui ponemos distintas luces auxiliares para el faro)
	// ====================================================================================
	SpotLight farosAuxiliares[6];
	// 0: Faro ROJO (Cono cerrado, llega lejos)
	farosAuxiliares[0] = SpotLight(1.0f, 0.0f, 0.0f, 		// Color Rojo (RGB)
		1.0f, 1.0f,											// Intensidades
		0.0f, 0.0f, 0.0f, 1.0f, -0.2f, 0.0f,				// Posición y Dirección 
		1.0f, 0.045f, 0.0075f, 								// Atenuación 
		30.0f);												// Ángulo de apertura 
	// 1: Faro AMARILLO (Cono medio)
	farosAuxiliares[1] = SpotLight(1.0f, 1.0f, 0.0f, 		// Color Amarillo (RGB)
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, -0.2f, 0.0f,
		1.0f, 0.045f, 0.0075f,
		45.0f);
	// 2: Faro VERDE (Cono amplio, se atenúa más rápido)
	farosAuxiliares[2] = SpotLight(0.0f, 1.0f, 0.0f, 		// Color Verde (RGB)
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, -0.2f, 0.0f,
		1.0f, 0.09f, 0.032f,
		60.0f);
	// 3: Faro CIAN 
	farosAuxiliares[3] = SpotLight(0.0f, 1.0f, 1.0f, 		// Color Cian (RGB)
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, -0.2f, 0.0f,
		1.0f, 0.045f, 0.0075f,
		75.0f);
	// 4: Faro AZUL (Original)
	farosAuxiliares[4] = SpotLight(0.0f, 0.0f, 1.0f, 		// Color Azul (RGB)
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, -0.2f, 0.0f,
		1.0f, 0.045f, 0.0075f,
		90.0f);
	// 5: Faro MAGENTA (Cono gigante)
	farosAuxiliares[5] = SpotLight(1.0f, 0.0f, 1.0f, 		// Color Magenta (RGB)
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, -0.2f, 0.0f,
		1.0f, 0.022f, 0.0019f,
		120.0f);


	// ====================================================================================
	// CICLO DE RENDERIZADO PRINCIPAL (WHILE)
	// ====================================================================================
	while (!mainWindow.getShouldClose())
	{
		now = glfwGetTime();
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
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();

		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// Luz ligada a la cámara de tipo flash (Comentada temporalmente)
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		// ====================================================================================
		// FASE 1: CÁLCULO DE JERARQUÍAS PARA LUCES EN MOVIMIENTO
		// ====================================================================================
		
		// Variable maestra del Coche
		posCoche = glm::vec3(0.0f + mainWindow.getMovCoche(), -1.1f, 0.0f);
		
		// JERARQUIA Y CAMBIO DE FAROS DEL COCHE (INDICE 1 en spothLights)
		// 1. Cambiamos la luz del faro a partir de la tecla 0 (rango [0,5])
		spotLights[1] = farosAuxiliares[mainWindow.getColorFaroCoche()];
		// 2. Le calculamos su posición global atada a la variable maestra del coche
		matrizCocheLuz = glm::mat4(1.0);
		matrizCocheLuz = glm::translate(matrizCocheLuz, posCoche); // Usamos la variable maestra
		posicionGlobalFaroCoche = matrizCocheLuz * posicionLocalFaroCoche;
		// 3. Le actualizamos la posición a nuestra luz 
		spotLights[1].SetPos(glm::vec3(posicionGlobalFaroCoche));


		// JERARQUÍA DEL FARO AMARILLO (INDICE 2 en spothLights)
		// Actualizamos la variable maestra del helicóptero
		posHelicoptero = glm::vec3(0.0f + mainWindow.getMovHelicoptero(), 5.0f, 8.0f);
		matrizHeliLuz = glm::mat4(1.0);
		matrizHeliLuz = glm::translate(matrizHeliLuz, posHelicoptero); // Usamos la variable maestra
		posGlobalHeli = matrizHeliLuz * posLocalHeli;
		spotLights[2].SetPos(glm::vec3(posGlobalHeli));


		// ====================================================================================
		// FASE 2: ENVÍO DE LUCES AL SHADER
		// ====================================================================================
		shaderList[0].SetDirectionalLight(&mainLight);
		// LÓGICA PARA APAGAR LA LÁMPARA HEXTECH
		if (mainWindow.getApagarLampara() == true) {
			// Con la tecla 9 (apagarLampara) decidimos si tenemos nuestro arreglo en 1 o en 0 
			shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
		}
		else {
			// Estado normal, le mandamos el total de luces (1) al shader. La luz brilla.
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		// PARA APAGAR ALGUNA LUZ, SOLO PUEDE SER POR AQUI, le debemos poner 
			// shaderList[0].SetPointLights(pointLights, pointLightCount-1);
			// Con esto eliminamos (apagamos) la ultima(s) luz(luces), pero debemos verificar que 
			// las que ocupamos apagar esten hasta el final del arreglo o sino
			// con condicionales cambiar nuestro arreglo activo, como en el previo. 


		// ====================================================================================
		// FASE 3: RENDERIZADO DE MODELOS EN ESCENA
		// ====================================================================================

		// PISO -------------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// COCHE Y CARROCERÍA -----------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, posCoche); // AQUÍ USAMOS LA VARIABLE MAESTRA
		modelaux = model;						// Pivote central guardado

		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CocheT.RenderModel();

		// Llantas del coche
		color = glm::vec3(0.5f, 0.5f, 0.5f); // Color gris para las llantas
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Trasera derecha 
		model = modelaux;
		model = glm::translate(model, glm::vec3(-4.3f, 0.4f, 3.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getRotacionLlantas()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaT.RenderModel();

		// Trasera izquierda 
		model = modelaux;
		model = glm::translate(model, glm::vec3(-4.3f, 0.4f, -3.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getRotacionLlantas()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaT.RenderModel();

		// Delantera derecha 
		model = modelaux;
		model = glm::translate(model, glm::vec3(6.0f, 0.4f, 3.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getRotacionLlantas()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaT.RenderModel();

		// Delantera izquierda 
		model = modelaux;
		model = glm::translate(model, glm::vec3(6.0f, 0.4f, -3.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getRotacionLlantas()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaT.RenderModel();

		// Cofre del coche 
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.2f, 2.6f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getAperturaCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CofreT.RenderModel();

		// HELICÓPTERO ------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, posHelicoptero); // Aplicamos la variable maestra aquí también
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		// DADO -------------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 5.0f, 6.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();

		// LÁMPARA HEXTECH --------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -7.0f));
		model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_M.RenderModel();


		/* // MARCADOR VISUAL DE LA LUZ BLANCA (Comentado por defecto)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.75f, -7.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();
		*/

		/*
		// ============================================================
		// PREVIO 07: LUCES PUNTUALES INTERCAMBIABLES + FOCOS VISIBLES
		// 1. Mandar luces direccionales y spot al shader (estas no se apagaron)
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		// 2. Lógica para intercambiar los arreglos de PointLights
		if (mainWindow.getCambioLuces() == false) {
			shaderList[0].SetPointLights(pointLights_Arr1, pointLightCount); // Estado 1
		}
		else {
			shaderList[0].SetPointLights(pointLights_Arr2, pointLightCount); // Estado 2
		}

		// 3. Dibujar los 4 "focos" flotantes para ver de dónde sale nuestras luces
		for (int i = 0; i < 4; i++) {
			model = glm::mat4(1.0);
			float posX = -15.0f + (i * 10.0f); // X saltará a: -15, -5, 5, 15
			model = glm::translate(model, glm::vec3(posX, 2.0f, 5.0f));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

			// Colores de los focos según el estado
			if (mainWindow.getCambioLuces() == false) {
				if (i == 0) color = glm::vec3(0.0f, 1.0f, 0.0f); // Verde
				if (i == 1) color = glm::vec3(0.0f, 0.0f, 1.0f); // Azul
				if (i == 2) color = glm::vec3(1.0f, 0.0f, 0.0f); // Roja
				if (i == 3) color = glm::vec3(1.0f, 1.0f, 1.0f); // Blanca
			}
			else {
				if (i == 0) color = glm::vec3(1.0f, 1.0f, 1.0f); // Blanca
				if (i == 1) color = glm::vec3(0.0f, 1.0f, 0.0f); // Verde
				if (i == 2) color = glm::vec3(0.0f, 0.0f, 1.0f); // Azul
				if (i == 3) color = glm::vec3(1.0f, 0.0f, 0.0f); // Roja
			}

			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[4]->RenderMesh(); // Dibuja el dado como foco
		}
		// ============================================================
		*/

		// AGAVE (BLENDING) -------------------------------------------------------------------
		// ELEMENTOS CON TRANSPARENCIA SE DIBUJAN AL ULTIMO
		// Renderizado al final para correcta transparencia
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		// ====================================================================================
		// Renderizado del Octaedro
		model = glm::mat4(1.0);
		// Lo trasladamos un poco a la derecha para que no choque con tus cubos
		model = glm::translate(model, glm::vec3(-10.0f, 5.0f, 6.0f));
		model = glm::rotate(model, mainWindow.getRotacionDado() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		OctaedroTexture.UseTexture();
		meshList[5]->RenderMesh();
		// ====================================================================================

		glDisable(GL_BLEND);

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}