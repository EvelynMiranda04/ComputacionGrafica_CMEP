/* ================================================================
* ===================== ANIMACIÓN POR KEYFRAMES ===================
================================================================ */

//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION
#include <fstream> // Librería para manejo de archivos
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
;
// ================= VARIABLES PARA ANIMACIÓN =================
// --- Variables Generales (Texturas y Rotaciones de la Escena) ---
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
// --- Animación: Coche ---
bool avanza;
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
// --- Animación: Nave (Aleteo de alas y hélices) ---
bool aleteaSube = true;
float rotAleteo = 0.0f;
float velAleteo = 0.5f;     // Velocidad del aleteo (grados por segundo)
float limiteAleteo = 30.0f;  // Límite máximo del ángulo de las alas
float rotHelices = 0.0f;
float velHelices = 10.0f;   // Velocidad de giro de las hélices

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
// ==================== MODELOS 'PROPIOS' / STEAMPUNK =====================
Model RelojSpT;
Model PenduloSpT;
Model EngranajeGr1T;
Model EngranajeGr2T;
Model EngranajeCh1T;
Model EngranajeCh2T;
Model NaveT;
Model Ala1T;
Model Ala2T;
Model Helice1T;
Model Helice2T;
// ========================================================================
Skybox skybox;
//materiales
Material Material_brillante;
Material Material_opaco;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

//INPUT DE KEYFRAMES
void inputKeyframes(bool* keys);
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



/* ==================================================================
================= SECCIÓN DE ANIMACIÓN POR KEYFRAMES ================
================================================================== */

// --- Animación: Keyframes (Nave/Avión) ---
float posXavion = 2.0,		posYavion = 2.0,		posZavion = 0;
float movAvion_x = 0.0f,	movAvion_y = 0.0f;
float giroAvion = 0;

float ciclo,				ciclo2,					contador = 0;
float guardoFrame = 0,		reinicioFrame = 0;
float reproduciranimacion=0,habilitaranimacion=0;
// Agreanos nuevos candados separados por eje:
float lockX_enable = 0, lockX_move = 0;
float lockY_enable = 0, lockY_move = 0;
float lockZ_enable = 0, lockZ_move = 0;


bool animacion = false;
bool play = false;
int playIndex = 0;
int FrameIndex = 0;			// Introducir datos iniciales
#define MAX_FRAMES 100
int i_max_steps = 90;		// Para ver cuantos cuadros intermedios calcula la interpolación
int i_curr_steps = 6;		// Numero de cuadros creados

typedef struct _frame
{
	// Variables para las rotaciones de los engranajes
	float rotG1, rotG2, rotCh1, rotP;
	// Incrementos para la interpolación
	float rotG1Inc, rotG2Inc, rotCh1Inc, rotPInc;
} FRAME;
FRAME KeyFrame[MAX_FRAMES];


// ==================== FUNCIONES DE KEYFRAMES =====================
void saveFrame(void) {
	// 1. Obtener valores actuales de la ventana
	float g1 = mainWindow.getRotEngranajeG1();
	float g2 = mainWindow.getRotEngranajeG2();
	float ch1 = mainWindow.getRotEngranajeC1();
	float p = mainWindow.getRotPendulo();

	// 2. Guardar en memoria
	KeyFrame[FrameIndex].rotG1 = g1;
	KeyFrame[FrameIndex].rotG2 = g2;
	KeyFrame[FrameIndex].rotCh1 = ch1;
	KeyFrame[FrameIndex].rotP = p;

	// 3. Escribir en el archivo TXT (Append)
	std::ofstream archivo;
	archivo.open("keyframes.txt", std::ios::app);
	if (archivo.is_open()) {
		archivo << g1 << " " << g2 << " " << ch1 << " " << p << "\n";
		archivo.close();
		printf("Frame %d guardado en archivo.\n", FrameIndex);
	}
	FrameIndex++;
}

void loadFrames(void) {
	std::ifstream archivo("keyframes.txt");
	if (!archivo.is_open()) {
		printf("No se encontro archivo. Iniciando desde cero.\n");
		return;
	}
	int i = 0;
	while (archivo >> KeyFrame[i].rotG1 >> KeyFrame[i].rotG2 >> KeyFrame[i].rotCh1 >> KeyFrame[i].rotP) {
		i++;
	}
	FrameIndex = i; // Actualizamos el contador real de frames leídos
	archivo.close();
	printf("Se cargaron %d frames desde el archivo.\n", FrameIndex);
}

void resetElements(void) {
	mainWindow.setRotEngranajeG1(KeyFrame[0].rotG1);
	mainWindow.setRotEngranajeG2(KeyFrame[0].rotG2);
	mainWindow.setRotEngranajeC1(KeyFrame[0].rotCh1);
	mainWindow.setRotPendulo(KeyFrame[0].rotP);
}

void interpolation(void) {
	KeyFrame[playIndex].rotG1Inc = (KeyFrame[playIndex + 1].rotG1 - KeyFrame[playIndex].rotG1) / i_max_steps;
	KeyFrame[playIndex].rotG2Inc = (KeyFrame[playIndex + 1].rotG2 - KeyFrame[playIndex].rotG2) / i_max_steps;
	KeyFrame[playIndex].rotCh1Inc = (KeyFrame[playIndex + 1].rotCh1 - KeyFrame[playIndex].rotCh1) / i_max_steps;
	KeyFrame[playIndex].rotPInc = (KeyFrame[playIndex + 1].rotP - KeyFrame[playIndex].rotP) / i_max_steps;
}

// Movimiento del objeto
void animate(void) {					// BARRA ESPACIADORA
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				interpolation();
			}
		}
		else
		{
			// Dentro del else, actualizar los Setters de la ventana:
			mainWindow.setRotEngranajeG1(mainWindow.getRotEngranajeG1() + KeyFrame[playIndex].rotG1Inc);
			mainWindow.setRotEngranajeG2(mainWindow.getRotEngranajeG2() + KeyFrame[playIndex].rotG2Inc);
			mainWindow.setRotEngranajeC1(mainWindow.getRotEngranajeC1() + KeyFrame[playIndex].rotCh1Inc);
			mainWindow.setRotPendulo(mainWindow.getRotPendulo() + KeyFrame[playIndex].rotPInc);
			i_curr_steps++;
		}
	}
}

/* ==================================================================
============= FIN DE SECCIÓN DE ANIMACIÓN POR KEYFRAMES =============
================================================================== */


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

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

	// ====================================================================================
	// 2. CARGA DE MODELOS
	// ====================================================================================
	Kitt_M = Model();				Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();				Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Pista_M = Model();				Pista_M.LoadModel("Models/pista.obj");
	Nave_M = Model();				Nave_M.LoadModel("Models/nave.obj");
	Ala_M = Model();				Ala_M.LoadModel("Models/ala.obj");

	// 'Propias' Steampunk -----------------------------------------------------------------
	Aeolipile_base_M = Model();		Aeolipile_base_M.LoadModel("Models/Aeolipile_base.obj");
	Aeolipile_M = Model();			Aeolipile_M.LoadModel("Models/Aeolipile.obj");
	RelojSpT = Model();				RelojSpT.LoadModel("Models/MaquinaSt_T.obj");
	PenduloSpT = Model();			PenduloSpT.LoadModel("Models/PenduloST_T.obj");
	EngranajeGr1T = Model();		EngranajeGr1T.LoadModel("Models/EngranajeGr01_T.obj");
	EngranajeGr2T = Model();		EngranajeGr2T.LoadModel("Models/EngranajeGr02_T.obj");
	EngranajeCh1T = Model();		EngranajeCh1T.LoadModel("Models/EngranajeCh01_T.obj");
	EngranajeCh2T = Model();		EngranajeCh2T.LoadModel("Models/EngranajeCh02_T.obj");
	NaveT = Model();				NaveT.LoadModel("Models/NaveT.obj");
	Ala1T = Model();				Ala1T.LoadModel("Models/Ala1T.obj");
	Ala2T = Model();				Ala2T.LoadModel("Models/Ala2T.obj");
	Helice1T = Model();				Helice1T.LoadModel("Models/Helice1T.obj");
	Helice2T = Model();				Helice2T.LoadModel("Models/Helice2T.obj");

	// ====================================================================================
	// 3. SKYBOX Y MATERIALES
	// ====================================================================================
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/31Atardecer_px.png"); // +X Derecha
	skyboxFaces.push_back("Textures/Skybox/31Atardecer_nx.png"); // -X Izquierda
	skyboxFaces.push_back("Textures/Skybox/31Atardecer_ny.png"); // -Y Abajo (PRIMERO)
	skyboxFaces.push_back("Textures/Skybox/31Atardecer_py.png"); // +Y Arriba (DESPUÉS)
	skyboxFaces.push_back("Textures/Skybox/31Atardecer_pz.png"); // +Z Atrás
	skyboxFaces.push_back("Textures/Skybox/31Atardecer_nz.png"); // -Z Frente
	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	// ====================================================================================
	// 4. CONFIGURACIÓN INICIAL DE LUCES
	// ====================================================================================
	
	// DIRECCIONAL - sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, -1.0f, 0.0f);
	
	// PUNTUALES
	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// SPOTS
	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	// ====================================================================================
	// VARIABLES DE SHADER Y MATRICES (Movidas fuera del ciclo)
	// ====================================================================================
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	GLfloat now = 0.0f;
	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 modelaux = glm::mat4(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	glm::vec3 lowerLight = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 matrizNaveVisual;
	glm::mat4 matrizReloj;

	
	movCoche = 0.0f;
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;

	//Keyframes
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);

	// Reemplazamos la declaración manual por
	loadFrames();

	printf("\nTECLAS PARA EL USO DE KEYFRAMES STEAMPUNK:\n");
	printf("ESPACIO:\tReproducir animacion.\n");
	printf("0:\t\tVolver a habilitar reproduccion de la animacion\n");
	printf("P:\t\tHabilitar guardar nuevo frame\n");
	printf("L:\t\tGuardar nuevo frame\n");
	printf("\nCONTROLES DE LA MAQUINA (En Tiempo Real):\n");
	printf("1 y 2:\t\tRotar Engranaje Grande 1\n");
	printf("3 y 4:\t\tRotar Engranaje Grande 2\n");
	printf("5 y 6:\t\tRotar Engranaje Chico 1\n");
	printf("7 y 8:\t\tRotar Pendulo\n");
	// ====================================================================================
	// CICLO DE RENDERIZADO PRINCIPAL (WHILE)
	// ====================================================================================
	while (!mainWindow.getShouldClose())
	{
		now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f*deltaTime;
		// ¿Cómo haces para que el coche no se salga del piso?
		movCoche -= movOffset * deltaTime;
		rotllanta += rotllantaOffset * deltaTime;

		// Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		// para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();
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
		//  LÓGICA DE ANIMACIÓN 
		// ====================================================================================
		movCoche -= movOffset * deltaTime;
		rotllanta += rotllantaOffset * deltaTime;

		// --- Lógica Ping-Pong del Aleteo ---
		if (aleteaSube) {
			rotAleteo += velAleteo * deltaTime;
			if (rotAleteo >= limiteAleteo) {
				rotAleteo = limiteAleteo;
				aleteaSube = false;
			}
		}
		else {
			rotAleteo -= velAleteo * deltaTime;
			if (rotAleteo <= -limiteAleteo) {
				rotAleteo = -limiteAleteo;
				aleteaSube = true;
			}
		}

		// --- Lógica de rotación continua (Hélices) ---
		rotHelices += velHelices * deltaTime;

		
		// ====================================================================================
		// FASE 1: CÁLCULO DE JERARQUÍAS DE LUCES
		// ====================================================================================
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		// ====================================================================================
		// FASE 2: ENVÍO DE LUCES AL SHADER
		// ====================================================================================
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		// Reiniciando variables de color y offset para modelos
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// ====================================================================================
		// FASE 3: RENDERIZADO DE MODELOS OPACOS
		// ====================================================================================

		// PISO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		// ====================================================================================
		// NAVE Y JERARQUÍA (KEYFRAMES + ANIMACIÓN PROCEDIMENTAL)
		// ====================================================================================

		/*
		// 1. Matriz Padre: Dictada por los Keyframes
		matrizNaveVisual = glm::mat4(1.0f);
		posblackhawk = glm::vec3(posXavion + movAvion_x, posYavion + movAvion_y + 10.0f, posZavion);
		matrizNaveVisual = glm::translate(matrizNaveVisual, posblackhawk);
		matrizNaveVisual = glm::rotate(matrizNaveVisual, giroAvion * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		// --- Lógica de orientación automática ---
		if (play && KeyFrame[playIndex].movAvion_xInc < 0.0f) {
			// Si va hacia -X (negativo), no rotamos en Y porque el modelo ya ve hacia allá
		}
		else {
			// Si va hacia +X o estamos configurando keyframes, rotamos 180° en Y
			matrizNaveVisual = glm::rotate(matrizNaveVisual, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizNaveVisual));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NaveT.RenderModel();

		
		// 2. HÉLICE 1 (Costado Derecho)
		model = matrizNaveVisual; // Hereda traslación y giro del keyframe
		model = glm::translate(model, glm::vec3(-2.3f, -2.1f, -2.55f));
		model = glm::rotate(model, rotHelices * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helice1T.RenderModel();

		// 3. HÉLICE 2 (Costado Izquierdo)
		model = matrizNaveVisual; // Hereda traslación y giro del keyframe
		model = glm::translate(model, glm::vec3(-2.3f, -2.1f, 2.55f));
		model = glm::rotate(model, rotHelices * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helice2T.RenderModel();

		// 4. ALA 1 (Costado Derecho)
		model = matrizNaveVisual; // Hereda traslación y giro del keyframe
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.5f));
		model = glm::rotate(model, rotAleteo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala1T.RenderModel();

		// 5. ALA 2 (Costado Izquierdo)
		model = matrizNaveVisual; // Hereda traslación y giro del keyframe
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.5f));
		model = glm::rotate(model, -rotAleteo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala2T.RenderModel();
		*/

		
		// MÁQUINA STEAMPUNK ------------------------------------------------------------------
		// BASE
		matrizReloj = glm::mat4(1.0f); // Iniciamos matriz identidad
		matrizReloj = glm::translate(matrizReloj, glm::vec3(0.0f, -2.0f, 0.0f)); // Posición: Origen
		matrizReloj = glm::scale(matrizReloj, glm::vec3(0.5f, 0.5f, 0.5f)); // Escala: 0.5
		matrizReloj = glm::rotate(matrizReloj, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación en Y
		model = matrizReloj;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RelojSpT.RenderModel();
		modelaux = matrizReloj;
		//  Péndulo
		model = modelaux;
		model = glm::translate(model, glm::vec3(6.0f, 34.0f, -0.9f));
		model = glm::rotate(model, mainWindow.getRotPendulo() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PenduloSpT.RenderModel();
		// Engranaje Grande 1 
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.8f, 8.2f, 0.0f));
		model = glm::rotate(model, mainWindow.getRotEngranajeG1() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeGr1T.RenderModel();
		// Engranaje Grande 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.2f, 4.0f, -3.0f));
		model = glm::rotate(model, mainWindow.getRotEngranajeG2() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeGr2T.RenderModel();
		// Engranaje Chico 1
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.4f, 9.7f, -3.7f));
		model = glm::rotate(model, mainWindow.getRotEngranajeC1() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeCh1T.RenderModel();


		


		// ====================================================================================
		// FASE 4: ELEMENTOS CON TRANSPARENCIA (BLENDING)
		// ====================================================================================
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// AGAVE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		glDisable(GL_BLEND);


		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}


void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n----> PRESIONA 0 PARA: \tHabilitar reproducir de nuevo la animación\n");
				habilitaranimacion = 0;
			}
			else
			{
				play = false;
			}
		}
	}

	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
			printf("----> Ya puedes reproducir de nuevo la animación con: \tBARRA ESPACIADORA\n");

			// MENÚ ACTUALIZADO PARA STEAMPUNK
			printf("\n--- CONTROLES DE GRABACION ---\n");
			printf("ESPACIO: Reproducir animacion\t 0: Reiniciar reproduccion\n");
			printf("P: Habilitar guardado de frame\t L: Guardar nuevo frame\n");
			printf("\n--- CONTROLES DE LA MAQUINA (Manten presionada la tecla) ---\n");
			printf("1 y 2: Engranaje Grande 1\n");
			printf("3 y 4: Engranaje Grande 2\n");
			printf("5 y 6: Engranaje Chico 1\n");
			printf("7 y 8: Pendulo\n");

			habilitaranimacion++;
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame(); // Aquí se llama a la función que guarda las rotaciones en el FrameIndex actual
			printf("\n\nNUEVO KEYFRAME GUARDADO PARA STEAMPUNK\n");

			// MENÚ ACTUALIZADO PARA STEAMPUNK
			printf("\n--- CONTROLES DE GRABACION ---\n");
			printf("ESPACIO: Reproducir animacion\t 0: Reiniciar reproduccion\n");
			printf("P: Habilitar guardado de frame\t L: Guardar nuevo frame\n");
			printf("\n--- CONTROLES DE LA MAQUINA (Manten presionada la tecla) ---\n");
			printf("1 y 2: Engranaje Grande 1\n");
			printf("3 y 4: Engranaje Grande 2\n");
			printf("5 y 6: Engranaje Chico 1\n");
			printf("7 y 8: Pendulo\n");

			guardoFrame++;
			reinicioFrame = 0;
		}
	}

	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
			printf("Ya puedes comenzar a configurar tu nuevo keyframe ajustando los engranajes.\n");
			printf("----> No olvides guardarlo con la tecla: \tL\n");
			reinicioFrame++;
		}
	}
}


/*void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n----> PRESIONA 0 PARA: \tHabilitar reproducir de nuevo la animación\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;

			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
			printf("----> Ya puedes reproducir de nuevo la animación con: \t\BARRA ESPACIADORA\n");
			printf("\nRECORDANDO LAS TECLAS PARA EL USO DE KEYFRAMES:\n");
			printf("01.- Presionar:  ESPACIO para \tReproducir animacion.\n02.- Presionar:  0 para \tVolver a habilitar reproduccion de la animacion\n");
			printf("03.- Presionar:  P para \tHabilitar guardar nuevo frame\n04.- Presionar:  L para \tGuardar nuevo frame\n");
			printf("05.- Presionar:  1 para \t\Habilitar mover en X\n06.- Presionar:  2 para \tMover en -X\n07.- Presionar:  3 para \tMover en +X\n");
			printf("08.- Presionar:  4 para \t\Habilitar mover en Y\n09.- Presionar:  5 para \tMover en -Y\n10.- Presionar:  6 para \tMover en +Y\n");
			printf("11.- Presionar:  7 para \t\Habilitar rotar en Z\n12.- Presionar:  8 para \tRotar en Z\n");
			habilitaranimacion++;
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("\n\nNUEVO KEYFRAME GUARDADO\n");
			printf("--- movAvion_x: %f\t", movAvion_x);
			printf("movAvion_y es: %f\n", movAvion_y);
			printf("\nRECORDANDO LAS TECLAS PARA EL USO DE KEYFRAMES:\n");
			printf("01.- Presionar:  ESPACIO para \tReproducir animacion.\n02.- Presionar:  0 para \tVolver a habilitar reproduccion de la animacion\n");
			printf("03.- Presionar:  P para \tHabilitar guardar nuevo frame\n04.- Presionar:  L para \tGuardar nuevo frame\n");
			printf("05.- Presionar:  1 para \t\Habilitar mover en X\n06.- Presionar:  2 para \tMover en -X\n07.- Presionar:  3 para \tMover en +X\n");
			printf("08.- Presionar:  4 para \t\Habilitar mover en Y\n09.- Presionar:  5 para \tMover en -Y\n10.- Presionar:  6 para \tMover en +Y\n");
			printf("11.- Presionar:  7 para \t\Habilitar rotar en Z\n12.- Presionar:  8 para \tRotar en Z\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
			printf("Ya puedes comenzar a configurar tu nuevo keyframe'\n");
			printf("----> No olvides guardarlo con la tecla: \tL\n");
			reinicioFrame++;
		}
	}


	// =====================================================================
	// CONTROLES DEL EJE X
	// =====================================================================
	if (keys[GLFW_KEY_1])
	{
		if (lockX_enable < 1)
		{
			lockX_move = 0; // Libera las teclas 2 y 3
			lockX_enable++; // Bloquea la tecla 1
			printf("\nYa puedes comenzar a mover en el eje:\t X\n");
			printf("----> Presiona 2:\tMover en -X\tPresiona 3:\tMover en +X\n");
		}
	}
	if (keys[GLFW_KEY_2])
	{
		if (lockX_move < 1)
		{
			movAvion_x -= 2.0f;
			printf("---------> movAvion_x actual: %f\n", movAvion_x);
			lockX_move++;
			lockX_enable = 0; // Vuelve a liberar la tecla 1
			printf("----> Habilita el movimiento en X con la tecla: 1\n");
		}
	}
	if (keys[GLFW_KEY_3])
	{
		if (lockX_move < 1)
		{
			movAvion_x += 2.0f;
			printf("---------> movAvion_x actual: %f\n", movAvion_x);
			lockX_move++;
			lockX_enable = 0; // Vuelve a liberar la tecla 1
			printf("----> Habilita el movimiento en X con la tecla: 1\n");
		}
	}

	// =====================================================================
	// CONTROLES DEL EJE Y
	// =====================================================================
	if (keys[GLFW_KEY_4])
	{
		if (lockY_enable < 1)
		{
			lockY_move = 0; // Libera las teclas 5 y 6
			lockY_enable++; // Bloquea la tecla 4
			printf("\nYa puedes comenzar a mover en el eje:\t Y\n");
			printf("----> Presiona 5:\tMover en -Y\tPresiona 6:\tMover en +Y\n");
		}
	}
	if (keys[GLFW_KEY_5])
	{
		if (lockY_move < 1)
		{
			movAvion_y -= 3.0f;
			printf("---------> movAvion_y actual: %f\n", movAvion_y);
			lockY_move++;
			lockY_enable = 0; // Vuelve a liberar la tecla 4
			printf("----> Habilita el movimiento en Y con la tecla: 4\n");
		}
	}
	if (keys[GLFW_KEY_6])
	{
		if (lockY_move < 1)
		{
			movAvion_y += 3.0f;
			printf("---------> movAvion_y actual: %f\n", movAvion_y);
			lockY_move++;
			lockY_enable = 0; // Vuelve a liberar la tecla 4
			printf("----> Habilita el movimiento en Y con la tecla: 4\n");
		}
	}

	// =====================================================================
	// CONTROLES DEL EJE Z (ROTACIÓN)
	// =====================================================================
	if (keys[GLFW_KEY_7])
	{
		if (lockZ_enable < 1)
		{
			lockZ_move = 0; // Libera la tecla 8
			lockZ_enable++; // Bloquea la tecla 7
			printf("\nYa puedes comenzar a rotar en el eje:\t Z\n");
			printf("----> Presiona 8:\tRotar en Z (+180 grados)\n");
		}
	}
	if (keys[GLFW_KEY_8])
	{
		if (lockZ_move < 1)
		{
			giroAvion += 180.0f; // Ajusta este valor a los grados que prefieras por "click"

			// Opcional: Mantener los grados dentro del rango 0-360
			if (giroAvion >= 360.0f) giroAvion -= 360.0f;

			printf("---------> giroAvion actual: %f grados\n", giroAvion);
			lockZ_move++;
			lockZ_enable = 0; // Vuelve a liberar la tecla 7
			printf("----> Habilita la rotacion en Z con la tecla: 7\n");
		}
	}

}

*/