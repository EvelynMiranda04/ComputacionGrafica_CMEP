/*
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/


// ====================================================================================
// CONFIGURACIÓN GLOBAL: LIBRERÍAS, VARIABLES, TEXTURAS Y MODELOS
// ====================================================================================

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

// ================= VARIABLES PARA ANIMACIÓN =================
// --- Variables Generales (Texturas y Rotaciones de la Escena) ---
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0f;
float angulovaria = 0.0f;
// --- Animación: Coche ---
bool  avanza;                       // Estado de dirección (true = avanza, false = retrocede)
float movCoche;                     // Posición actual del coche en su eje de traslación
float movOffset;                    // Velocidad de avance/retroceso del coche
float rotllanta;                    // Ángulo actual de las llantas
float rotllantaOffset;              // Velocidad a la que giran las llantas
// --- Animación: Nave (Avance en X) ---
bool  avanzaNave;                   // Estado de dirección de la nave
float movNave;                      // Posición actual de avance de la nave
float movOffsetNave;                // Velocidad a la que avanza la nave
// --- Animación: Nave (Zigzag en Y) ---
bool  subeNave;                     // Estado vertical (true = sube, false = baja)
float alturaNave;                   // Posición actual flotante en Y
float velocidadAlturaNave;          // Qué tan rápido sube o baja la nave
// --- Animación: Nave (Aleteo de alas en Z) ---
bool  aleteaSube;                   // Estado del aleteo (true = rotando hacia arriba)
float rotAleteo;                    // Ángulo actual en el que se encuentra el ala
float velAleteo;                    // Qué tan rápido rotan las alas (grados por segundo)
float limiteAleteo;                 // Límite de rotación máxima (amplitud del aleteo)


// --- Animación: Control de Texturas (Offsets y Frames) ---
float AcumuladorTN;				// Cambio de frame en la textura de números
float tiempoDeEsperaN;			// Límite de tiempo para el cambio perceptible
int numero1 = 0;				// Contador de ciclos/frames para alternar entre texturas (Frame-by-frame)

// --- Animación: Giro del Aeolipile ---
float tiempoFuego;				// Controlador de etapas para el fuego
float velAeolipile;				// Velocidad de giro (Incrementa y baja)
float anguloAeolipile;			// Rotación acumulada en grados (Para aplicar en Z)
float tiempoGirando;			// Tiempo que lleva rotando (Al calentarse)
float tiempoApagando;			// Tiempo que lleva desde que el fuego comienza a apagarse
// --- Animación: Efectos Visuales del Fuego ---
float EscalamientoY;			// Factor de escala en Y
float EscalamientoXZ;			// Factor de escala en XZ
float escalaExtincion;			// Multiplicador de tamaño (1.0 a 0.0) para encoger el fuego al apagarse

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;

Model Kitt_M;
Model Llanta_M;
Model Pista_M;
Model Nave_M;
Model Ala_M;
Model Aeolipile_base_M;
Model Aeolipile_M;


// ==================== MODELOS 'PROPIOS' =====================
Texture OctaedroTexture;
Texture dadoTexture;
Texture Fuego;
Model LlantaT;
Model CofreT;
Model CocheT;
Model Lampara_M;
Model Pecera;
Model PezAbisal;
Model NaveT;
Model Ala1T;
Model Ala2T;
// ============================================================

Skybox skybox;

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



//cálculo del promedio de las normales para sombreado de Phong
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
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

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


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	// ====================================================================================
	// PREPARACIÓN DE LA ESCENA: VENTANA, SHADERS Y CÁMARA
	// ====================================================================================
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CreateShaders();
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f,0.5f, 0.5f);

	// ====================================================================================
	// CARGA DE TEXTURAS Y MODELOS 3D
	// ====================================================================================
	brickTexture = Texture("Textures/brick.png");			brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");				dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");			plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");				pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");			AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");		FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");		Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");		Numero2Texture.LoadTextureA();
	// 'Propias' --------------------------------------------------------------------------
	Fuego = Texture("Textures/fuegito1.tga");				Fuego.LoadTextureA();


	Kitt_M = Model();				Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();				Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Pista_M = Model();				Pista_M.LoadModel("Models/pista.obj");
	Nave_M = Model();				Nave_M.LoadModel("Models/nave.obj");
	Ala_M = Model();				Ala_M.LoadModel("Models/ala.obj");
	Aeolipile_base_M = Model();		Aeolipile_base_M.LoadModel("Models/Aeolipile_base.obj");
	Aeolipile_M = Model();			Aeolipile_M.LoadModel("Models/Aeolipile.obj");
	// 'Propias' --------------------------------------------------------------------------
	LlantaT = Model();				LlantaT.LoadModel("Models/llantaT1.obj");
	CocheT = Model();				CocheT.LoadModel("Models/CocheT.obj");
	CofreT = Model();				CofreT.LoadModel("Models/CofreT.obj");
	Lampara_M = Model();			Lampara_M.LoadModel("Models/LamparaOBJ.obj");
	Pecera = Model();				Pecera.LoadModel("Models/Pecera.obj");
	PezAbisal = Model();			PezAbisal.LoadModel("Models/PezAbisal.obj");
	NaveT = Model();				NaveT.LoadModel("Models/NaveT.obj");
	Ala1T = Model();				Ala1T.LoadModel("Models/Ala1T.obj");
	Ala2T = Model();				Ala2T.LoadModel("Models/Ala2T.obj");


	// ====================================================================================
	// SKYBOX Y MATERIALES
	// ====================================================================================
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
	// 4. CONFIGURACIÓN INICIAL DE LUCES (Direccional, Puntuales y Spot)
	// ====================================================================================
	// 1. LUZ DE "SOL" (Direccional - sólo 1 y siempre debe de existir) -------------------
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.4f, 0.4f,						// Bajamos intensidad
		0.0f, -1.0f, 0.0f);
	// 2. LUCES PUNTUALES (Point Lights) --------------------------------------------------
	unsigned int pointLightCount = 0;
	// Luz de la Lámpara Hextech (Luz puntual BLANCA)
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,	// Color Blanco (RGB)
		1.0f, 1.0f,						// Intensidad (alta para que ilumine bien)
		0.0f, 5.75f, -7.0f,				// Posición: Misma X, Y, Z
		1.0f, 0.09f, 0.032f);			// Atenuación para difuminado realista
	pointLightCount++;
	// LUZ DEL PEZ ABISAL (Índice 1)
	pointLights[1] = PointLight(0.0f, 0.0f, 1.0f,	// Color Azul Puro (RGB)
		1.0f, 1.0f,						// Intensidad
		0.0f, 0.0f, 0.0f,				// Posición (Se sobreescribe en el while)
		1.0f, 0.7f, 1.8f);				// Atenuación	
	pointLightCount++;
	// 3. LUCES SPOT (Linternas y Faros) --------------------------------------------------
	unsigned int spotLightCount = 0;
	// Índice 0: Faro AZUL (Coche) - Este será sobreescrito en el while
	spotLights[0] = SpotLight(0.035f, 0.318f, 0.600f,	// Color Azul (RGB)
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, -0.2f, 0.0f,
		1.0f, 0.045f, 0.0075f,
		90.0f);
	spotLightCount++;
	// Índice 1: Faro AMARILLO 1 (Nave) - Este será sobreescrito en el while 
	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,		// Color Amarillo (RGB)
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, 0.045f, 0.0075f,
		30.0f);
	spotLightCount++;
	// Índice 2: Spotlight del Pez Abisal (Amarillo)
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f,	// Color Amarillo (RGB)
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.045f, 0.0075f,
		40.0f);
	spotLightCount++;


	// ====================================================================================
	// ARREGLO AUXILIAR DE FAROS (Aqui ponemos distintas luces auxiliares para faros)
	// ====================================================================================
	SpotLight farosAuxiliares[8];
	// 0: Faro ROJO (Cono cerrado, llega lejos)
	farosAuxiliares[0] = SpotLight(1.0f, 0.0f, 0.0f,	// Color Rojo (RGB)
		1.0f, 1.0f,										// Intensidades
		0.0f, 0.0f, 0.0f, 1.0f, -0.2f, 0.0f,			// Posición y Dirección 
		1.0f, 0.045f, 0.0075f,							// Atenuación 
		30.0f);											// Ángulo de apertura 
	// 1: Faro AMARILLO (Cono medio)
	farosAuxiliares[1] = SpotLight(1.0f, 1.0f, 0.0f,	// Color Amarillo (RGB)
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, -0.2f, 0.0f,
		1.0f, 0.045f, 0.0075f,
		45.0f);
	// 2: Faro VERDE (Cono amplio, se atenúa más rápido)
	farosAuxiliares[2] = SpotLight(0.0f, 1.0f, 0.0f,	// Color Verde (RGB)
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, -0.2f, 0.0f,
		1.0f, 0.09f, 0.032f,
		60.0f);
	// 3: Faro CIAN 
	farosAuxiliares[3] = SpotLight(0.0f, 1.0f, 1.0f,	// Color Cian (RGB)
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, -0.2f, 0.0f,
		1.0f, 0.045f, 0.0075f,
		75.0f);
	// 4: Faro AZUL (Original)
	farosAuxiliares[4] = SpotLight(0.0f, 0.0f, 1.0f,	// Color Azul (RGB)
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, -0.2f, 0.0f,
		1.0f, 0.045f, 0.0075f,
		90.0f);
	// 5: Faro MAGENTA (Cono gigante)
	farosAuxiliares[5] = SpotLight(1.0f, 0.0f, 1.0f,	// Color Magenta (RGB)
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, -0.2f, 0.0f,
		1.0f, 0.022f, 0.0019f,
		120.0f);
	// 6: HELICOPTERO FARO AMARILLO PERO HACIA ADELANTE
	farosAuxiliares[6] = SpotLight(1.0f, 1.0f, 0.0f,	// Color Amarillo (RGB)
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,
		1.0f, 0.09f, 0.032f,
		30.0f);
	// 7: HELICOPTERO FARO AMARILLO PERO HACIA ATRAS
	farosAuxiliares[7] = SpotLight(1.0f, 1.0f, 0.0f,	// Color Amarillo (RGB)
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		1.0f, 0.045f, 0.0075f,
		30.0f);

	// ====================================================================================
	// VARIABLES DE SHADER Y MATRICES
	// ====================================================================================
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	


	// ====================================================================================
	// DECLARACIONES MOVIDAS FUERA DEL CICLO 
	// ====================================================================================

	// Inicialización de matrices y vectores de apoyo -------------------------------------
	GLfloat   now = 0.0f;
	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 modelaux = glm::mat4(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	glm::vec3 lowerLight = glm::vec3(0.0f, 0.0f, 0.0f);

	// Estados iniciales para: Coche y faro -----------------------------------------------
	glm::vec3 posCoche;
	glm::mat4 matrizCocheLuz;
	glm::vec4 posicionLocalFaroCoche = glm::vec4(8.8f, 2.1f, 0.0f, 1.0f);
	glm::vec4 posicionGlobalFaroCoche;
	glm::vec3 dirFaroGlobal;
	movCoche = 0.0f;
	movOffset = 0.1f;						// Ajusta la velocidad de avance
	rotllanta = 0.0f;
	rotllantaOffset = 2.0f;					// Ajusta la velocidad de giro de llantas

	// Estados iniciales para: Nave y faro ------------------------------------------------
	// --- Jerarquía de la Luz Spot ---
	glm::mat4 matrizNaveLuz;
	glm::vec4 posLocalFaroNave = glm::vec4(-1.0f, -0.5f, 0.0f, 1.0f); // Hacia "adelante" y "abajo"
	glm::vec4 posGlobalFaroNave;
	glm::vec3 dirLocalFaroNave;
	glm::vec3 dirGlobalFaroNave;
	// --- Avance (Ping-Pong en X) ---
	avanzaNave = true;
	movNave = 0.0f;
	movOffsetNave = 0.1f;					// Velocidad a la que avanza la nave
	// --- Zigzag (Sube y baja en Y) ---
	subeNave = true;						// Booleano para el ping-pong vertical
	alturaNave = 0.0f;
	velocidadAlturaNave = 0.05f;			// Qué tan rápido sube/baja
	// --- Aleteo (Rotación en Z) ---
	aleteaSube = true;						// Booleano para el ping-pong de las alas
	rotAleteo = 0.0f;						// Ángulo actual del ala
	velAleteo = 0.5f;						// Qué tan rápido rotan las alas
	limiteAleteo = 30.0f;					// Amplitud máxima del aleteo


	// Estados iniciales para : Pez abisal y luces ----------------------------------------
	glm::mat4 matrizPezLuz;
	glm::mat4 matrizDirSpot;
	glm::vec3 dirBaseSpot;
	glm::vec3 dirFinalSpot;
	glm::vec4 posLocalLuzPez = glm::vec4(0.0f, 2.65f, 0.65f, 1.0f);
	glm::vec4 posGlobalLuzPez;
	float posX_Pez = 0.0f;
	float posY_Pez = 0.0f;
	float posZ_Pez = 0.0f;

	// Estados iniciales para: Helicoptero y faro -----------------------------------------
	glm::vec3 posHelicoptero;
	glm::mat4 matrizHeliLuz;
	glm::vec4 posLocalHeli = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 posGlobalHeli;


	// EJERCICIO 9.2: Animaciones con Texturas y Transformaciones
	// --- Animación: Control de Texturas (Offsets y Frames) ---
	AcumuladorTN = 0.0f;
	tiempoDeEsperaN = 25.0f;	// VELOCIDAD perceptible para el cambio
	// --- Animación: Giro del Aeolipile ---
	tiempoFuego = 0.0f;			// Cronómetro para los 2.5 segundos aprox
	velAeolipile = 0.0f;		// Velocidad de giro (Incrementa y baja)
	anguloAeolipile = 0.0f;		// Rotación acumulada en grados (Para aplicar en Z)
	tiempoGirando = 0.0f;		// Tiempo que lleva rotando (Al calentarse)
	tiempoApagando = 0.0f;		// Tiempo que lleva desde que el fuego comienza a apagarse
	// --- Animación: Efectos Visuales del Fuego ---
	EscalamientoY =0.0f;		// Factor de escala en Y
	EscalamientoXZ=0.0f;		// Factor de escala en XZ
	escalaExtincion = 0.0f;		// Multiplicador de tamaño (1.0 a 0.0) para encoger



	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{

		now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		angulovaria += 0.5f * deltaTime;

		// ====================================================================================
		//  LÓGICA DE ANIMACIÓN PING-PONG
		// ====================================================================================

		// 1. Lógica Ping-Pong del Coche (Movimiento en X) ------------------------------------
		rotllanta += rotllantaOffset * deltaTime; // Rotación continua de las llantas
		if (avanza) {
			// Si avanza, RESTAMOS para que viaje hacia la dirección correcta en X
			movCoche -= movOffset * deltaTime;
			if (movCoche <= -90.0f) {     // Límite de 90 unidades (hacia los negativos)
				movCoche = -90.0f;        // Tope exacto
				avanza = false;           // Da la vuelta
			}
		}
		else {
			// Si retrocede, SUMAMOS para que regrese a 0
			movCoche += movOffset * deltaTime;
			if (movCoche >= 0.0f) {       // Rregresamos al punto de partida
				movCoche = 0.0f;          // Tope exacto
				avanza = true;            // Vuelve a avanzar dando la vuelta
			}
		}

		// 2. Lógica Ping-Pong de la Nave (Avanza hacia -20 en X) -----------------------------
		if (avanzaNave) {
			// Si avanza, RESTAMOS para que viaje hacia el -20
			movNave -= movOffsetNave * deltaTime;
			if (movNave <= -90.0f) {      // Llegó al límite negativo
				movNave = -90.0f;
				avanzaNave = false;       // Da la vuelta para regresar
			}
		}
		else {
			// Si regresa, SUMAMOS para que vuelva al origen (0)
			movNave += movOffsetNave * deltaTime;
			if (movNave >= 0.0f) {        // Llegó al inicio
				movNave = 0.0f;
				avanzaNave = true;        // Vuelve a avanzar hacia el -20
			}
		}

		// 3. Lógica ZIGZAG de la Nave (Sube y Baja en Y) ------------------------------------
		if (subeNave) {
			alturaNave += velocidadAlturaNave * deltaTime;
			if (alturaNave >= 2.0f) {     // Llegó al tope superior
				alturaNave = 2.0f;
				subeNave = false;         // Ahora baja
			}
		}
		else {
			alturaNave -= velocidadAlturaNave * deltaTime;
			if (alturaNave <= -2.0f) {    // Llegó al tope inferior
				alturaNave = -2.0f;
				subeNave = true;          // Ahora sube
			}
		}

		// 4. Lógica Ping-Pong del Aleteo (Rotación en Z) -------------------------------------
		if (aleteaSube) {
			rotAleteo += velAleteo * deltaTime;
			if (rotAleteo >= limiteAleteo) {  // Llegó al límite superior de rotación
				rotAleteo = limiteAleteo;
				aleteaSube = false;           // Ahora rota hacia abajo
			}
		}
		else {
			rotAleteo -= velAleteo * deltaTime;
			if (rotAleteo <= -limiteAleteo) { // Llegó al límite inferior de rotación
				rotAleteo = -limiteAleteo;
				aleteaSube = true;            // Ahora rota hacia arriba
			}
		}

 
		// ====================================================================================
		// 6. Lógica de Rotación del Aeolipile -----------------------------------------------
		if (mainWindow.getAnimacionFuego()) {
			tiempoFuego += deltaTime; // Inicia el cronómetro del fuego
			// Inicia después de 125.0f (2.5 seg?)
			//	-> Acelera hasta llegar al tope en 625.f (+10 seg?)
			//	  -> Se queda en el tope 875.0f (+5 seg?)
			if (tiempoFuego >= 125.0f && tiempoFuego < 875.0f) {
				 tiempoGirando = tiempoFuego - 125.0f;
				// ARRANQUE SUAVE: La velocidad sube
				velAeolipile = tiempoGirando * 0.03f;
				// TOPE DE SEGURIDAD
				if (velAeolipile > 15.0f) {
					velAeolipile = 15.0f;
				}
			}
			// ¿15 seg? después de girar, comienza a relentizarse
			else if (tiempoFuego >= 875.0f) {
				 tiempoApagando = tiempoFuego - 875.0f;
				// FRENADO: -0.06f (se detiene en 5 seg?)
				velAeolipile = 15.0f - (tiempoApagando * 0.06f);
				// Tope para evitar que gire en reversa al acabarse el fuego
				if (velAeolipile < 0.0f) {
					velAeolipile = 0.0f;
				}
			}
		}
		else {
			// Si apagamos el fuego, todo regresa a cero
			tiempoFuego = 0.0f;
			velAeolipile = 0.0f;
		}
		// Sumamos la velocidad actual
		anguloAeolipile += velAeolipile * deltaTime;
		// Reset para no llegar a numeros gigantes
		if (anguloAeolipile > 360.0f) anguloAeolipile -= 360.0f;
		// ====================================================================================

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
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);


		// ====================================================================================
		// FASE 1: CÁLCULO DE JERARQUÍAS DE LUCES
		// ====================================================================================

		//  1. JERARQUÍA DEL COCHE (Faro Frontal Dinámico) ------------------------------------
		spotLights[0] = farosAuxiliares[mainWindow.getColorFaroCoche()];
		// Posición base del coche (X animada, Y ajustada a -1.5, Z estática)
		posCoche = glm::vec3(movCoche, -1.5f, 0.0f);
		matrizCocheLuz = glm::mat4(1.0);
		matrizCocheLuz = glm::translate(matrizCocheLuz, posCoche);
		// Giro de 180 grados si el coche va de regreso
		if (!avanza) {
			matrizCocheLuz = glm::rotate(matrizCocheLuz, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		// Giro constante del coche
		matrizCocheLuz = glm::rotate(matrizCocheLuz, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		// Actualizamos posición y dirección global para que alumbre siempre al frente del coche
		posicionGlobalFaroCoche = matrizCocheLuz * posicionLocalFaroCoche;
		dirFaroGlobal = glm::mat3(matrizCocheLuz) * glm::vec3(1.0f, -0.2f, 0.0f);
		spotLights[0].SetFlash(glm::vec3(posicionGlobalFaroCoche), dirFaroGlobal);

		// 2. JERARQUÍA DEL PEZ ABISAL --------------------------------------------------------
		posX_Pez = 0.0f;
		posY_Pez = -1.0f + mainWindow.getMovPez();
		posZ_Pez = -15.55f - (3.5f * mainWindow.getMovPez());
		matrizPezLuz = glm::mat4(1.0);
		matrizPezLuz = glm::translate(matrizPezLuz, glm::vec3(posX_Pez, posY_Pez, posZ_Pez));
		matrizPezLuz = glm::rotate(matrizPezLuz, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		matrizPezLuz = glm::rotate(matrizPezLuz, mainWindow.getRotPez() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		// Posición global de la luz del Pez
		posGlobalLuzPez = matrizPezLuz * posLocalLuzPez;
		// Dirección de la Luz Spot del Pez
		matrizDirSpot = glm::mat4(1.0);
		matrizDirSpot = glm::rotate(matrizDirSpot, mainWindow.getRotSpotX() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		matrizDirSpot = glm::rotate(matrizDirSpot, mainWindow.getRotSpotY() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		matrizDirSpot = glm::rotate(matrizDirSpot, mainWindow.getRotSpotZ() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		dirFinalSpot = glm::mat3(matrizPezLuz) * glm::mat3(matrizDirSpot) * glm::vec3(0.0f, -1.0f, 0.0f);
		spotLights[2].SetFlash(glm::vec3(posGlobalLuzPez), dirFinalSpot);

		// 3. JERARQUÍA DE LA LUZ DE LA NAVE (Índice 1 en spotLights) -------------------------
		// Le asignamos un faro auxiliar (Ej. el 1 que es Amarillo)
		spotLights[1] = farosAuxiliares[6];
		matrizNaveLuz = glm::mat4(1.0);
		// 1. Misma traslación exacta que la Nave (Avance en X + Zigzag en Y)
		matrizNaveLuz = glm::translate(matrizNaveLuz, glm::vec3(movNave, 13.0f + alturaNave, 1.5f));
		// 2. Mismo giro de 180 grados si la nave va de regreso
		if (!avanzaNave) {
			matrizNaveLuz = glm::rotate(matrizNaveLuz, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		// 3. Calculamos la Posición Global multiplicando por el punto local
		posGlobalFaroNave = matrizNaveLuz * posLocalFaroNave;
		// 4. Calculamos la Dirección (Apunta hacia adelante [-X] y hacia abajo [-Y])
		dirLocalFaroNave = glm::vec3(-1.0f, -1.0f, 0.0f);
		dirGlobalFaroNave = glm::mat3(matrizNaveLuz) * dirLocalFaroNave;
		// 5. Enviamos la información final al foco 1
		spotLights[1].SetFlash(glm::vec3(posGlobalFaroNave), dirGlobalFaroNave);



		// ====================================================================================
		// FASE 2: ENVÍO DE LUCES AL SHADER
		// ====================================================================================
		shaderList[0].SetDirectionalLight(&mainLight);
		// Reiniciamos el contador de luces puntuales en CADA FRAME
		unsigned int lucesPuntualesActivas = 0;
		// Lámpara Hextech (Se enciende y apaga)
		if (!mainWindow.getApagarLampara()) {
			// La metemos en la posición actual del contador (sería la 0)
			pointLights[lucesPuntualesActivas] = PointLight(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 5.75f, -7.0f, 1.0f, 0.09f, 0.032f);
			lucesPuntualesActivas++;
		}
		// Foco Azul del Pez (Se enciende y apaga)
		if (!mainWindow.getApagarLuzPez()) {
			// La metemos en la posición actual del contador (sería la 0 si la lámpara se apagó, o la 1 si la lámpara está prendida)
			pointLights[lucesPuntualesActivas] = PointLight(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, posGlobalLuzPez.x, posGlobalLuzPez.y, posGlobalLuzPez.z, 1.0f, 0.09f, 0.032f);
			lucesPuntualesActivas++;
		}
		// Le mandamos el arreglo y nuestro contador exacto de luces activas
		shaderList[0].SetPointLights(pointLights, lucesPuntualesActivas);
		// Las luces spot se quedan igual
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		// Reinicializando variables para modelos opacos
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		// ====================================================================================
		// FASE 3: RENDERIZADO DE MODELOS OPACOS
		// ====================================================================================

		// PISO -------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// PISTA ------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, -2.1f, -1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Pista_M.RenderModel();

		// COCHE Y CARROCERÍA -----------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche, -1.0f, 0.0f));
		if (!avanza) model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CocheT.RenderModel();

		// Llantas del coche ------------------------------------------------------------------
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Trasera derecha
		model = glm::translate(modelaux, glm::vec3(4.3f, 0.4f, 3.1f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaT.RenderModel();
		// Trasera izquierda
		model = glm::translate(modelaux, glm::vec3(4.3f, 0.4f, -3.1f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaT.RenderModel();
		// Delantera derecha
		model = glm::translate(modelaux, glm::vec3(-6.0f, 0.4f, 3.1f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaT.RenderModel();
		// Delantera izquierda
		model = glm::translate(modelaux, glm::vec3(-6.0f, 0.4f, -3.1f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaT.RenderModel();

		// Cofre ------------------------------------------------------------------------------
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(modelaux, glm::vec3(-4.2f, 2.6f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getAperturaCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CofreT.RenderModel();

		// NAVE Y SUS ALAS (Jerarquía Zigzag + Aleteo) ----------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movNave, 13.0f + alturaNave, 1.5f));
		if (!avanzaNave) {
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		NaveT.RenderModel();
		// ALA 1 (Costado Derecho)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.5f));
		model = glm::rotate(model, rotAleteo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala1T.RenderModel();
		// ALA 2 (Costado Izquierdo) 
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.5f));
		model = glm::rotate(model, -rotAleteo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala2T.RenderModel();

		// LÁMPARA HEXTECH
		// ====================================================================================
		model = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, -2.0f, -7.0f));
		model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_M.RenderModel();

		// PEZ ABISAL
		// ====================================================================================
		model = matrizPezLuz; // Recicla la matriz que calculaste arriba para la luz
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PezAbisal.RenderModel();

		
		// ====================================================================================
		// AEOLIPILE --------------------------------------------------------------------------
		// BASE
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Aeolipile_base_M.RenderModel();
		// CIRCULO
		model = glm::translate(model, glm::vec3(0.0f, 4.8f, 0.0f));
		// --- APLICAMOS LA ROTACIÓN ANIMADA ---
		model = glm::rotate(model, anguloAeolipile * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Aeolipile_M.RenderModel();
		// ====================================================================================
		

		
		// ====================================================================================
		// FASE 4: ELEMENTOS CON TRANSPARENCIA (BLENDING) Y TEXTURAS ANIMADAS
		// ====================================================================================
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// 1. PECERA (Transparencia Estática) -------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.95f, -20.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pecera.RenderModel();

		// 2. AGAVE (Transparencia Estática) --------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();


		// 3. FLECHA ANIMADA (Animación Continua de Coordenadas UV) ---------------------------
		// Se suma un valor pequeño continuo a 'toffsetflechau' para que la textura "corra" sobre el plano
		toffsetflechau += 0.001;
		toffsetflechav = 0.00;
		if (toffsetflechau > 1.0) toffsetflechau = 0.0; // Reinicio para evitar desbordamiento
		toffset = glm::vec2(toffsetflechau, toffsetflechav); // Empaquetamos en un vec2

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset)); // Mandamos el offset al Shader
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();


		// 4. PLANO BASE DE NÚMEROS (Referencia Estática) -------------------------------------
		// Renderiza la textura completa sin offsets (Offset = 0.0)
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();


		// 5. NÚMERO 1 ESTÁTICO (Extracción de una sección de textura) ------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();


		// 6. NÚMEROS DEL 2 AL 4 (Ciclo for con Offsets en el eje U) --------------------------
		for (int i = 1; i < 4; i++) {
			toffsetnumerou += 0.25; // Salto en el eje horizontal (U) del mapa de textura
			toffsetnumerov = 0.0;   // Se mantiene en la primera fila
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);

			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f)); // Posición dinámica en X
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();
		}

		// 7. NÚMEROS DEL 5 AL 8 (Ciclo for con Offsets en el eje U y V) ----------------------
		for (int j = 1; j < 5; j++) {
			toffsetnumerou += 0.25; // Salto horizontal
			toffsetnumerov = -0.33; // Salto vertical hacia la fila de abajo en la textura
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);

			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();
		}


		// 8. NÚMERO ANIMADO POR TIEMPO DELTATIME (Spritesheet Lógico) ------------------------
		AcumuladorTN += deltaTime; // Sumamos el tiempo real transcurrido

		// Si alcanzamos las 25.0f unidades (tiempoDeEsperaN), saltamos al siguiente número
		if (AcumuladorTN >= tiempoDeEsperaN) {
			toffsetnumerocambiau += 0.25f; // Cambiamos al siguiente número (offset de 0.25)
			if (toffsetnumerocambiau >= 1.0f) {
				toffsetnumerocambiau = 0.0f; // Reiniciamos si llegamos al final
			}
			AcumuladorTN = 0.0f; // Reseteamos el reloj
		}

		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();


		// 9. ANIMACIÓN POR INTERCAMBIO DE TEXTURAS (Por Frames) ------------------------------
		// En lugar de mover coordenadas UV, cargamos dos imágenes separadas y alternamos entre ellas
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Alternancia basada en un contador de iteraciones del 'while' (Aproximación por FPS)
		if (numero1 < 59) {
			Numero1Texture.UseTexture(); // Muestra textura A los primeros 60 frames
			numero1++;
		}
		if (numero1 >= 59) {
			Numero2Texture.UseTexture(); // Muestra textura B los siguientes 60 frames
			numero1++;
		}
		if (numero1 > 120) {
			numero1 = 0; // Reinicia el ciclo
		}

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh(); // Se renderiza sobre el mismo mesh


		// ====================================================================================
		// 10. FUEGO ANIMADO -------------------------------------------------------------
		if (mainWindow.getAnimacionFuego()) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -0.7f, 20.0f));

			// --- LÓGICA DE EXTINCIÓN ---
			escalaExtincion = 1.0f; // 1.0 significa 100% de tamaño normal
			// Si ya se alcanzo el giro max y los ¿5 seg? extra (625 + 250 = 875.0f)
			if (tiempoFuego > 875.0f) {
				// Tiempo para apagar (empieza en 0)
				 tiempoApagando = tiempoFuego - 875.0f;
				// En 5 segundos (250.0f) debe llegar a 0.0
				escalaExtincion = 1.0f - (tiempoApagando / 250.0f);
				// Tope final: Cuando se encoge por completo, lo apagamos del sistema
				if (escalaExtincion <= 0.0f) {
					escalaExtincion = 0.0f;
					mainWindow.setAnimacionFuego(false); // False para usar 'L' nuevamente
				}
			}

			// --- ANIMACIÓN DE ESCALADO ---
			EscalamientoY = 1.0f + (sin(now * 15.0f) * 0.15f);  // Estira hacia arriba/abajo
			EscalamientoXZ = 1.0f + (cos(now * 12.0f) * 0.05f); // Engorda un poquito a los lados
			// Aplicamos la escala dinámica Y LA MULTIPLICAMOS POR LA EXTINCIÓN
			model = glm::scale(model, glm::vec3(EscalamientoXZ * escalaExtincion, EscalamientoY * escalaExtincion, EscalamientoXZ * escalaExtincion));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

			// Limpiamos los offsets
			toffset = glm::vec2(0.0f, 0.0f);
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			Fuego.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[3]->RenderMesh(); //Mesh 3 (Planos cruzados en X como el agave)
		}
		// ====================================================================================

		glDisable(GL_BLEND); // Apagamos el blending al final de la fase


		/*
		PARA EL REPORTE
		Apretamos tecla
		Sale fuego (dinámico en escalado)
		Empieza a girar el airplaie de poco en poco (dejar ¿3? seg antes de que inicie)
		Emepieza a salir humo al girar
		El humo comienza a crecer
		Esto activa el brazo que tirará una esfera
		La esfera es lanzada mediante tiro parabolico
		La bola cae y rebota
		Llega hasta una textura de hueco y cae por esta
		La esfera desaparece
		Reaparece en el brazo
		(EL brazo antes regresa a su posición inicial?)

		 


		PARA EL PREVIO
		Tomar en cuenta los tamaños de los engranes y darle movimiento al elemento que los engranes estan moviendo 
		También agregar una tecla adicional para cada uno que de momento no tendra utilidad
		Osea, necesitamos una tecla que incremente el angulo?

		*/

		/*Añadir para ejercicio
		1.- Agregar que el número cambiante sea a una velocidad visible.
		Acomodar jerarquía del Aelopile
		2.- Giro del Aelopile
		3.-Fuego
			- Editar Textura para el fuego
			- Blending para el fuego
			- Animación del fuego

		Añadir para el reporte
		1.- Humo
			- Editar Textura para el humo
			- Blending para el humo
			- Animación del humo: no sale y sale
		2.- Brazo/palanca que lance la esfera metálica
			-El humo inicia el movimiento del brazo palanca
			- Animación del brazo
		3.- Esfera metálica
			- Movimiento de la esfera metálica por medio de física básica: tiro parabólico y rebote
		*/

		



		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
