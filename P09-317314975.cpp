/*
Animación:
Sesión 1: Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Sesión 2: Compleja: Por medio de funciones y algoritmos. Textura Animada
*/

//para cargar imagen

#define STB_IMAGE_IMPLEMENTATION

#include <stdlib.h> // Para usar rand()
#include <time.h>   // Semilla aleatoria

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
#include "RecorridoP.h"

RecorridoP animadorRuta; // Instanciamos la clase globalmente

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;


// ================= VARIABLES PARA ANIMACIÓN =================
// (Declaración global, sin inicialización)

// --- Variables Generales (Texturas y Rotaciones de la Escena) ---
float toffsetflechau;
float toffsetflechav;
float toffsetnumerou;
float toffsetnumerov;
float toffsetnumerocambiau;
float angulovaria;	

// --- Animación: Coche ---
/*bool  avanza;					// Estado de dirección (true = avanza, false = retrocede)
float movCoche;					// Posición actual del coche en su eje de traslación
float movOffset;				// Velocidad de avance/retroceso del coche
*/float rotllanta;				// Ángulo actual de las llantas
float rotllantaOffset;			// Velocidad a la que giran las llantas

// --- Animación: Nave (Avance en X) ---
/*bool  avanzaNave;				// Estado de dirección de la nave
float movNave;					// Posición actual de avance de la nave
float movOffsetNave;			// Velocidad a la que avanza la nave

// --- Animación: Nave (Zigzag en Y) ---
bool  subeNave;					// Estado vertical (true = sube, false = baja)
float alturaNave;				// Posición actual flotante en Y
float velocidadAlturaNave;		// Qué tan rápido sube o baja la nave
*/
// --- Animación: Nave (Aleteo de alas en Z) ---
bool  aleteaSube;				// Estado del aleteo (true = rotando hacia arriba)
float rotAleteo;				// Ángulo actual en el que se encuentra el ala
float velAleteo;				// Qué tan rápido rotan las alas (grados por segundo)
float limiteAleteo;				// Límite de rotación máxima (amplitud del aleteo)

// --- Animación: Nave (Hélices en Z) ---
float rotHelices;				// Ángulo actual de las hélices
float velHelices;				// Velocidad de giro

bool flagReinicio;


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
Model Blackhawk_M;
Model Pista_M;
Model Nave_M;
Model Ala_M;

// ==================== MODELOS 'PROPIOS' =====================
Texture OctaedroTexture;
Texture dadoTexture;
Model LlantaT;
Model CofreT;
Model CocheT;
Model Lampara_M;
Model Pecera;
Model PezAbisal;
Model NaveT;
Model Ala1T;
Model Ala2T;
Model Helice1T;
Model Helice2T;
Model D8T;
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

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

/*
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


// ==================== OCTAEDRO 'PROPIO' =====================
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

*/


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	//CrearDado();
	CreateShaders();
	//CrearOctaedro();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	// ====================================================================================
	// 1. CARGA DE TEXTURAS
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
	dadoTexture = Texture("Textures/dado_animales2.tga");	dadoTexture.LoadTextureA();
	OctaedroTexture = Texture("Textures/OCTAEDRO.tga");		OctaedroTexture.LoadTextureA();



	// ====================================================================================
	// 2. CARGA DE MODELOS
	// ====================================================================================
	Kitt_M = Model();				Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();				Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();			Blackhawk_M.LoadModel("Models/uh60.obj");
	Pista_M = Model();				Pista_M.LoadModel("Models/pista.obj");
	Nave_M = Model();				Nave_M.LoadModel("Models/nave.obj");
	Ala_M = Model();				Ala_M.LoadModel("Models/ala.obj");
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
	Helice1T = Model();				Helice1T.LoadModel("Models/Helice1T.obj");
	Helice2T = Model();				Helice2T.LoadModel("Models/Helice2T.obj");
	D8T = Model();					D8T.LoadModel("Models/octaedrod8.obj");

	// ====================================================================================
	// 3. SKYBOX Y MATERIALES
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
		0.1f, 0.1f,						// Bajamos intensidad
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
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	// ====================================================================================
	// DECLARACIONES MOVIDAS FUERA DEL CICLO
	// ====================================================================================

	// --- 1. TIEMPO Y VELOCIDADES BASE ---
	GLfloat now = 0.0f;				
	float velTramoX = 0.15f;			// VELOCIDAD de la máquina de estados en el eje X
	float velTramoY = 0.15f;			// VELOCIDAD proporcional en Y (Uso interno de etapa)
	float velTramoZ = 0.15f;			// VELOCIDAD proporcional en Z (Uso interno de etapa)

	// --- 2. ANIMACIONES GLOBALES Y TEXTURAS ---
	toffsetflechau = 0.0f;
	toffsetflechav = 0.0f;
	toffsetnumerou = 0.0f;
	toffsetnumerov = 0.0f;
	toffsetnumerocambiau = 0.0f;
	angulovaria = 0.0f;
	rotllanta = 0.0f;				// Ángulo actual de las llantas
	rotAleteo = 0.0f;				// Ángulo actual de las alas de la nave
	aleteaSube = true;				// Bandera de dirección del aleteo (arriba/abajo)
	limiteAleteo = 30.0f;			// Ángulo máximo de apertura de las alas
	rotHelices = 0.0f;				// Ángulo acumulado de las hélices
	rotllantaOffset = 2.0f;			// VELOCIDAD DE ROTACIÓN DE LAS LLANTAS
	velAleteo = 0.5f;				// VELOCIDAD DE ALETEO 
	velHelices = 10.0f;				// VELOCIDAD DE ROTACIÓN DE LAS HELICES
	flagReinicio = false;			// Bandera para reiniciar animaciones al volver a la etapa 1

	// --- 3. VARIABLES BASE DE SHADER Y RENDERIZADO ---
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 modelaux = glm::mat4(1.0f);	
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	glm::vec3 lowerLight = glm::vec3(0.0f, 0.0f, 0.0f);
	unsigned int lucesPuntualesActivas = 0;

	// --- 4. JERARQUÍA Y LUCES: COCHE ---
	glm::mat4 modelCompensado = glm::mat4(1.0f);		// Matriz para corregir la rotación en reversa
	glm::mat4 matrizCocheLuz = glm::mat4(1.0f);			// Matriz base anclada al coche para el faro
	glm::vec4 posicionLocalFaroCoche = glm::vec4(8.8f, 2.1f, 0.0f, 1.0f); // Posición del faro respecto al chasis
	glm::vec4 posFaroFrente = glm::vec4(0.0f);			// Posición temporal del faro en el frame
	glm::vec3 dirFaroFrente = glm::vec3(0.0f);			// Vector de dirección de la luz temporal
	glm::vec4 posicionGlobalFaroCoche = glm::vec4(0.0f);// Coordenada real del faro en el mundo 3D
	glm::vec3 dirFaroGlobal = glm::vec3(0.0f);			// Hacia dónde apunta la luz en el mundo 3D

	// --- 5. JERARQUÍA Y LUCES: NAVE ---
	glm::mat4 matrizBaseNave = glm::mat4(1.0f);			// Matriz pura sacada de la máquina de estados
	glm::mat4 matrizNaveVisual = glm::mat4(1.0f);		// Matriz parcheada visualmente (compensación Blender)
	glm::mat4 matrizNaveLuz = glm::mat4(1.0f);			// Matriz de anclaje para la luz de la nave
	glm::vec4 posLocalFaroNave = glm::vec4(-1.0f, -0.5f, 0.0f, 1.0f); // Dónde va el foco en la nave
	glm::vec4 posFaroNaveFrente = glm::vec4(0.0f);		// Cálculo temporal de posición
	glm::vec3 dirFaroNaveFrente = glm::vec3(0.0f);		// Cálculo temporal de dirección
	glm::vec4 posGlobalFaroNave = glm::vec4(0.0f);		// Posición real del faro de la nave en el mundo
	glm::vec3 dirLocalFaroNave = glm::vec3(0.0f);		// Hacia dónde apunta internamente
	glm::vec3 dirGlobalFaroNave = glm::vec3(0.0f);		// Hacia dónde dispara la luz en el mundo

	// --- 6. JERARQUÍA Y LUCES: PEZ ABISAL ---
	float posX_Pez = 0.0f;								// Posición en X controlada por teclado
	float posY_Pez = 0.0f;								// Posición en Y controlada por teclado
	float posZ_Pez = 0.0f;								// Posición en Z controlada por teclado
	glm::mat4 matrizPezLuz = glm::mat4(1.0f);			// Matriz base del pez
	glm::mat4 matrizDirSpot = glm::mat4(1.0f);			// Matriz extra para rotar solo el faro independientemente
	glm::vec4 posLocalLuzPez = glm::vec4(0.0f, 2.65f, 0.65f, 1.0f); // Dónde está la antena del pez
	glm::vec4 posGlobalLuzPez = glm::vec4(0.0f);		// Coordenada real de la luz de la antena
	glm::vec3 dirBaseSpot = glm::vec3(0.0f);			// Dirección inicial sin rotar
	glm::vec3 dirFinalSpot = glm::vec3(0.0f);			// Dirección calculada de la luz

	// --- 7. JERARQUÍA Y LUCES: HELICÓPTERO ---
	glm::vec3 posHelicoptero = glm::vec3(0.0f);			// Vector base de posición
	glm::mat4 matrizHeliLuz = glm::mat4(1.0f);			// Matriz de anclaje para luces del helicóptero
	glm::vec4 posLocalHeli = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // Posición interna de la luz
	glm::vec4 posGlobalHeli = glm::vec4(0.0f);			// Posición de la luz en el mundo


	// --- 8. VARIABLES DE ANIMACIÓN: DADO D8 ---
	bool dadoRodando = false;				// ¿Dado en el aire (true) o en el piso (false)?
	float tiempoDado = 0.0f;				// Cronómetro interno que lleva la cuenta del tiro actual
	float duracionTiro = 150.0f;			// Tiempo total del vuelo
	int resultadoDado = 0;					// Resultado
	// Para el tiro-vuelo del d8
	float t = 0.0f;							// Tiempo de rot
	float tSuave = 0.0f;
	float alturaDestino;
	float baseYActual;

	glm::vec3 posDadoBase = glm::vec3(0.0f, -2.0f, 10.0f);	// Coordenada donde descansa el dado
	glm::vec3 posDadoActual = posDadoBase;					// Posición dinámica (Sube y baja durante la parábola)
	glm::vec3 rotDadoActual = glm::vec3(0.0f);				// Rotación dinámica actual en pantalla (X, Y, Z)
	glm::vec3 rotDadoInicio = glm::vec3(0.0f);				// Ángulo exacto desde el que arranca el tiro
	glm::vec3 rotDadoDestino = glm::vec3(0.0f);				// Ángulo meta al que debe llegar (Cara ganadora + Vueltas extra)

	// Inicializamos la semilla aleatoria tomando el reloj de la computadora
	srand(time(NULL));

	// METAS DEL DADO D8 (Ángulos Euler en Grados)
	glm::vec3 metasD8[8] = {
		glm::vec3(55.0f,    0.0f,    0.0f),		// Cara 1
		glm::vec3(0.0f,  -90.0f,  -55.0f),		// Cara 2
		glm::vec3(-55.0f, -180.0f,    0.0f),	// Cara 3
		glm::vec3(0.0f, -270.0f,   55.0f),		// Cara 4
		glm::vec3(180.0f,   90.0f,  -55.0f),	// Cara 5 (+1.2 en Y)
		glm::vec3(-235.0f,    0.0f,    0.0f),	// Cara 6 (+1.2 en Y)
		glm::vec3(180.0f,   90.0f,   55.0f),	// Cara 7 (+1.2 en Y)
		glm::vec3(235.0f,    0.0f,    0.0f)		// Cara 8 (+1.2 en Y)
	};


	// ====================================================================================
	// CICLO DE RENDERIZADO PRINCIPAL (WHILE)
	// ====================================================================================
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
		
		/*
		// 1. Lógica Ping-Pong del Coche (Movimiento en X) ------------------------------------
		rotllanta += rotllantaOffset * deltaTime; // Rotación continua de las llantas
		if (avanza) {
			// Si avanza, RESTAMOS para que viaje hacia la dirección correcta en X
			movCoche -= movOffset * deltaTime;
			if (movCoche <= -50.0f) {     // Límite de 50 unidades (hacia los negativos)
				movCoche = -50.0f;        // Tope exacto
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
			if (movNave <= -20.0f) {      // Llegó al límite negativo
				movNave = -20.0f;
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
		*/


		// 4. Lógica Ping-Pong del Aleteo (Rotación en Z) -------------------------------------
		if (aleteaSube) {
			// Fase de subida: Sumamos velocidad hasta tocar el "techo"
			rotAleteo += velAleteo * deltaTime;
			if (rotAleteo >= limiteAleteo) {  
				rotAleteo = limiteAleteo; // Anclamos al tope exacto
				aleteaSube = false;       // Invertimos la bandera para que baje
			}
		}
		else {
			// Fase de bajada: Restamos velocidad hasta tocar el "piso"
			rotAleteo -= velAleteo * deltaTime;
			if (rotAleteo <= -limiteAleteo) { 
				rotAleteo = -limiteAleteo; // Anclamos al piso exacto
				aleteaSube = true;         // Invertimos la bandera para que suba
			}
		}

		// 5.Lógica Ping-Pong de rotaión continua (Hélices en Z) ------------------------------
		// Al no tener límites, el ángulo crecerá por siempre, generando un giro constante.
		rotHelices += velHelices * deltaTime;

		// 6. MÁQUINA DE ESTADOS UNIFICADA (Coche y Nave) -------------------------------------
		// 6.1 Extraemos el valor del teclado desde la ventana
		flagReinicio = mainWindow.getReiniciaCoche();
		// 6.2 Pasamos el valor a la clase (por referencia) y ejecutamos las 17 etapas.
		animadorRuta.actualizarRuta(deltaTime, velTramoX, flagReinicio);
		// 6.3 Sincronización Inversa: 
		// Si la clase reseteó el coche, 'flagReinicio' habrá cambiado a false internamente.
		// Si es así, actualizamos la ventana para apagar la señal del teclado.
		if (!flagReinicio && mainWindow.getReiniciaCoche()) {
			mainWindow.setReiniciaCoche(false);
		}

		// 7. Rotación de llantas ------------------------------------------------------------
		// Consultamos la variable pública 'termino' de nuestra clase.
		if (!animadorRuta.coche.termino) {
			rotllanta += rotllantaOffset * deltaTime;
		}

		// 8. Tiro D8 -------------------------------------------------------------------------
		// 8.1 Verificamos tecla (N)
		if (mainWindow.getTiroD8() && !dadoRodando) {
			dadoRodando = true;				// Cambiamos el estado a "En el aire"
			tiempoDado = 0.0f;				// Reiniciamos el cronómetro de la animación
			mainWindow.setTiroD8(false);	// Apagamos la señal del teclado
			// 1. Aleatoriamente decidimos resultado con un residuo
			resultadoDado = rand() % 8;
			// 2. Guardamos la posición 
			rotDadoInicio = rotDadoActual;
			// 3. Para el destino: Tomamos la meta y le sumamos vueltas completas (360°).
			rotDadoDestino = metasD8[resultadoDado] + glm::vec3(360.0f * 4, 360.0f * 6, 360.0f * 3);
			// Impresión para verificar resultado esperado
			printf("El dado caera en la cara: %d\n", resultadoDado + 1);
		}
		// Ignoramos la tecla si el dado ya está a medio vuelo para evitar que se reinicie de golpe
		else if (mainWindow.getTiroD8() && dadoRodando) {
			mainWindow.setTiroD8(false);
		}

		// 8.2 Animación de tiro
		if (dadoRodando) {
			tiempoDado += deltaTime; // Avanzamos el reloj interno

			// Convertimos el tiempo a un porcentaje que va de 0.0 (inicio) a 1.0 (final).
			t = tiempoDado / duracionTiro;
			if (t > 1.0f) t = 1.0f; // Para que no pase del 100%

			// CURVA DE DESACELERACIÓN (Fórmula: Ease-Out Cubic)
			// Altera la variable 't' para que arranque a máxima velocidad y frene suavemente al final.
			tSuave = 1.0f - pow(1.0f - t, 3.0f);
			// ROTACIÓN: INSERTANDO DE MANERA LINEAL (Lerp)
			// Mezclamos el ángulo inicial con el destino calculados en el Paso A, usando el tiempo suavizado.
			rotDadoActual = glm::mix(rotDadoInicio, rotDadoDestino, tSuave);

			// Revisamos siel resultado requiere +1.2 Y
			alturaDestino = posDadoBase.y;
			if (resultadoDado >= 4) {
				alturaDestino += 1.2f;
			}
			// Insertamos a nivel de piso 
			baseYActual = glm::mix(posDadoBase.y, alturaDestino, tSuave);

			// Calculamos el salto aplicando la función Seno y se lo sumamos al piso móvil.
			//    (El seno va de 0 a 1 y regresa a 0, creando una parábola perfecta de altura 3.0).
			posDadoActual.y = baseYActual + sin(t * 3.14159f) * 3.0f;


			// ATERRIZAJE 
			if (tiempoDado >= duracionTiro) {
				dadoRodando = false; // 'Apagamos' animación
				// Forzamos la altura final
				if (resultadoDado >= 4) posDadoActual.y = posDadoBase.y + 1.2f;
				else posDadoActual.y = posDadoBase.y;
				// Pasamos de rotación actual por la meta
				rotDadoActual = metasD8[resultadoDado];
			}
		}
		

		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

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
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);



		// ====================================================================================
		// FASE 1: CÁLCULO DE MATRICES Y SUS JERARQUÍAS DE LUCES
		// ====================================================================================

		// ====================================================================================
		// MATRIZ Y LUZ: COCHE (Espacio Local vs Espacio Global)
		// ====================================================================================
		modelaux = glm::mat4(1.0f); // Iniciamos con una matriz "limpia" (Identidad) en el centro del mundo.
		// Movemos el origen de la matriz a la posición actual del coche.
		modelaux = glm::translate(modelaux, glm::vec3(animadorRuta.coche.posX, animadorRuta.coche.posY, animadorRuta.coche.posZ));
		// Aplicamos las rotaciones de la pista (Em orden Y -> X -> Z)
		modelaux = glm::rotate(modelaux, animadorRuta.coche.anguloY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = glm::rotate(modelaux, (90.0f - animadorRuta.coche.anguloX) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = glm::rotate(modelaux, -animadorRuta.coche.anguloZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		// Giramos el modelo -90° en Y para corregir cómo trabajamos desdeBlender
		modelCompensado = glm::rotate(modelaux, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		// Si el coche va de reversa, giramos todo el modelo 180° visualmente para que la cajuela apunte hacia adelante.
		if (animadorRuta.coche.deRegreso) {
			modelCompensado = glm::rotate(modelCompensado, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		// LUZ DEL FARO DEL COCHE (Spot Light) 
		spotLights[0] = farosAuxiliares[mainWindow.getColorFaroCoche()]; // Cargamos el color del faro
		matrizCocheLuz = modelCompensado; // La luz hereda TODAS las transformaciones del coche
		// Posición y Dirección "Locales"
		posFaroFrente = glm::vec4(-7.5f, 2.1f, 0.0f, 1.0f); // El faro está físicamente adelante (-X) y un poco arriba (+Y).
		dirFaroFrente = glm::vec3(-1.0f, -0.2f, 0.0f);      // Apunta derecho hacia adelante (-X) y ligeramente hacia abajo (-Y).
		// Multiplicamos Local * Matriz para obtener las coordenadas reales en el mundo 3D.
		posicionGlobalFaroCoche = matrizCocheLuz * posFaroFrente;
		// Para la dirección, usamos mat3 para ignorar la Traslación (no se puede "mover" una dirección, solo rotarla).
		dirFaroGlobal = glm::mat3(matrizCocheLuz) * dirFaroFrente;
		// Le enviamos los datos finales calculados al Shader.
		spotLights[0].SetFlash(glm::vec3(posicionGlobalFaroCoche), dirFaroGlobal);


		// ====================================================================================
		// MATRIZ Y LUZ: PEZ ABISAL 
		// ====================================================================================
		posX_Pez = 0.0f;
		posY_Pez = -1.0f + mainWindow.getMovPez();
		posZ_Pez = -15.55f - (3.5f * mainWindow.getMovPez());
		matrizPezLuz = glm::mat4(1.0f);
		matrizPezLuz = glm::translate(matrizPezLuz, glm::vec3(posX_Pez, posY_Pez, posZ_Pez));
		matrizPezLuz = glm::rotate(matrizPezLuz, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		matrizPezLuz = glm::rotate(matrizPezLuz, mainWindow.getRotPez() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		// Posición global de la luz del Pez
		posGlobalLuzPez = matrizPezLuz * posLocalLuzPez;
		// Dirección de la Luz Spot del Pez
		matrizDirSpot = glm::mat4(1.0f);
		matrizDirSpot = glm::rotate(matrizDirSpot, mainWindow.getRotSpotX() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		matrizDirSpot = glm::rotate(matrizDirSpot, mainWindow.getRotSpotY() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		matrizDirSpot = glm::rotate(matrizDirSpot, mainWindow.getRotSpotZ() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		dirFinalSpot = glm::mat3(matrizPezLuz) * glm::mat3(matrizDirSpot) * glm::vec3(0.0f, -1.0f, 0.0f);
		spotLights[2].SetFlash(glm::vec3(posGlobalLuzPez), dirFinalSpot);




		// ====================================================================================
		// MATRIZ Y LUZ: NAVE (Espacio Local vs Espacio Global)
		// ====================================================================================
		matrizBaseNave = glm::mat4(1.0f); // Iniciamos con una matriz "limpia" (Identidad) en el centro del mundo.
		// Movemos el origen de la matriz a la posición actual de la nave, sumando 10 unidades en Y para que vuele.
		matrizBaseNave = glm::translate(matrizBaseNave, glm::vec3(animadorRuta.nave.posX, animadorRuta.nave.posY + 10.0f, animadorRuta.nave.posZ));
		// Aplicamos las rotaciones de la pista (En orden Y -> X -> Z)
		matrizBaseNave = glm::rotate(matrizBaseNave, animadorRuta.nave.anguloY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		matrizBaseNave = glm::rotate(matrizBaseNave, (90.0f - animadorRuta.nave.anguloX) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		matrizBaseNave = glm::rotate(matrizBaseNave, -animadorRuta.nave.anguloZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		// Giramos el modelo 90° en Y para corregir cómo trabajamos desde Blender
		matrizNaveVisual = glm::rotate(matrizBaseNave, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		// LUZ DEL FARO DE LA NAVE (Spot Light)
		spotLights[1] = farosAuxiliares[6]; // Cargamos el faro auxiliar amarillo hacia adelante
		matrizNaveLuz = matrizNaveVisual; // La luz hereda TODAS las transformaciones de la nave
		// Posición y Dirección "Locales"
		posFaroNaveFrente = glm::vec4(-4.0f, -0.5f, 0.0f, 1.0f); // El faro está físicamente en la nariz (-X) y un poco abajo (-Y).
		dirFaroNaveFrente = glm::vec3(-1.0f, -1.0f, 0.0f);       // Apunta en diagonal hacia adelante (-X) y hacia el suelo (-Y).
		// Multiplicamos Local * Matriz para obtener las coordenadas reales en el mundo 3D.
		posGlobalFaroNave = matrizNaveLuz * posFaroNaveFrente;
		// Para la dirección, usamos mat3 para ignorar la Traslación (no se puede "mover" una dirección, solo rotarla).
		dirGlobalFaroNave = glm::mat3(matrizNaveLuz) * dirFaroNaveFrente;
		// Le enviamos los datos finales calculados al Shader.
		spotLights[1].SetFlash(glm::vec3(posGlobalFaroNave), dirGlobalFaroNave);





		// ====================================================================================
		// FASE 2: ENVÍO DE LUCES AL SHADER
		// ====================================================================================
		shaderList[0].SetDirectionalLight(&mainLight);

		// Reiniciamos el contador de luces puntuales en CADA FRAME
		lucesPuntualesActivas = 0;

		// Lámpara Hextech (Se enciende y apaga)
		if (!mainWindow.getApagarLampara()) {
			// La metemos en la posición actual del contador (sería la 0)
			pointLights[lucesPuntualesActivas] = PointLight(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 5.75f, -7.0f, 1.0f, 0.09f, 0.032f);
			lucesPuntualesActivas++;
		}
		// Foco Azul del Pez (Se enciende y apaga)
		if (!mainWindow.getApagarLuzPez()) {
			// La metemos en la posición actual del contador 
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
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// PISTA ------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.1f, -1.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pista_M.RenderModel();


		// COCHE -----------------------------------------------------------------------------
		model = modelCompensado;
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // Recupera su rotación visual interna
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CocheT.RenderModel();
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Llanta trasera derecha
		model = glm::translate(modelCompensado, glm::vec3(4.3f, 0.4f, 3.1f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaT.RenderModel();
		// Llanta trasera izquierda
		model = glm::translate(modelCompensado, glm::vec3(4.3f, 0.4f, -3.1f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaT.RenderModel();
		// Llanta delantera derecha
		model = glm::translate(modelCompensado, glm::vec3(-6.0f, 0.4f, 3.1f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaT.RenderModel();
		// Llanta delantera izquierda
		model = glm::translate(modelCompensado, glm::vec3(-6.0f, 0.4f, -3.1f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaT.RenderModel();
		// COFRE
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(modelCompensado, glm::vec3(-4.2f, 2.6f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getAperturaCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CofreT.RenderModel();

		// NAVE -------------------------------------------------------------------------------
		model = matrizNaveVisual; // ¡Usamos la matriz mágica calculada en la Fase 1!
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		NaveT.RenderModel();
		// HÉLICE 1 (Costado Derecho)
		model = matrizNaveVisual;
		model = glm::translate(model, glm::vec3(-2.3f, -2.1f, -2.55f));
		model = glm::rotate(model, rotHelices * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helice1T.RenderModel();
		// HÉLICE 2 (Costado Izquierdo)
		model = matrizNaveVisual;
		model = glm::translate(model, glm::vec3(-2.3f, -2.1f, 2.55f));
		model = glm::rotate(model, rotHelices * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helice2T.RenderModel();
		// ALA 1 (Costado Derecho)
		model = matrizNaveVisual;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.5f));
		model = glm::rotate(model, rotAleteo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala1T.RenderModel();
		// ALA 2 (Costado Izquierdo)
		model = matrizNaveVisual;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.5f));
		model = glm::rotate(model, -rotAleteo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala2T.RenderModel();


		// LÁMPARA HEXTECH ---------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, -7.0f));
		model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_M.RenderModel();

		// PEZ ABISAL ---------------------------------------------------------------------------
		model = matrizPezLuz;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PezAbisal.RenderModel();





		// ====================================================================================
		// FASE 4: ELEMENTOS CON TRANSPARENCIA (BLENDING)
		// ====================================================================================
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// PECERA -----------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.95f, -20.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pecera.RenderModel();

		// AGAVE ------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();



		// DADO D8 COMPENSADO -----------------------------------------------------------------
		model = glm::mat4(1.0f);
		// Lo movemos a su posición física + la compensación (si la hay)
		model = glm::translate(model, glm::vec3(posDadoActual.x, posDadoActual.y, posDadoActual.z));
		// Aplicamos la rotación de la meta (o el caos si está rodando)
		model = glm::rotate(model, rotDadoActual.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotDadoActual.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotDadoActual.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		D8T.RenderModel();


		glDisable(GL_BLEND);


		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}