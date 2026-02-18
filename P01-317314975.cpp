// LIBRERIAS
#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>


// CAMBIO 1: --------------------------------------------------
#include <ctime>   // Para time()
#include <cstdlib> // Para rand() y srand()
// Variables para el control del tiempo y color
float tiempoAnterior = 0.0f;
float r = 1.0f, g = 1.0f, b = 1.0f; // Color inicial (Blanco)
// ------------------------------------------------------------


//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;

// LENGUAJE DE SHADER
static const char* vShader = "                              \n\
#version 330                                                \n\
layout (location = 0) in vec3 pos;                          \n\
void main()                                                 \n\
{gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);}";

// Fragment Shader con color blanco para las figuras
//recibir Vcolor y dar de salida color
static const char* fShader = "                              \n\
#version 330                                                \n\
out vec4 color;                                             \n\
void main()                                                 \n\
{color = vec4(1.0f, 1.0f, 1.0f, 1.0f);}";

// CAMBIO 4: --------------------------------------------------
void CrearTriangulo()
{
    // Como estamos en 2D el ultimo valor (Z) 
    // lo dejamos siempre en 0 ----     X,Y,Z
    GLfloat vertices[] = {

        // LETRA E (Conformada por 8 triángulos):
        // Rectangulo vertical
        -0.6f,  0.3f, 0.0f,     -0.6f, -0.2f, 0.0f,     -0.5f, -0.2f, 0.0f, //Triangulo 1
        -0.6f,  0.3f, 0.0f,     -0.5f, -0.2f, 0.0f,     -0.5f, 0.3f, 0.0f,  //Triangulo 2
        // Rectangulo superior
        -0.5f, 0.3f, 0.0f,      -0.5f, 0.2f, 0.0f,      -0.3f, 0.2f, 0.0f,  //Triangulo 3
        -0.5f, 0.3f, 0.0f,      -0.3f, 0.2f, 0.0f,      -0.3f, 0.3f, 0.0f,  //Triangulo 4
        // Rectangulo medio
        -0.5f, 0.1f, 0.0f,      -0.5f, 0.0f, 0.0f,      -0.4f, 0.0f, 0.0f,  //Triangulo 5
        -0.5f, 0.1f, 0.0f,      -0.4f, 0.0f, 0.0f,      -0.4f, 0.1f, 0.0f,  //Triangulo 6
		// Rectangulo inferior
        -0.5f, -0.1f, 0.0f,     -0.5f, -0.2f, 0.0f,     -0.3f, -0.2f, 0.0f, //Triangulo 7
        -0.5f, -0.1f, 0.0f,     -0.3f, -0.2f, 0.0f,     -0.3f, -0.1f, 0.0f, //Triangulo 8

        // LETRA C (Conformada por 6 triángulos):
        // Rectangulo vertical
        -0.2f,  0.3f, 0.0f,     -0.2f, -0.2f, 0.0f,     -0.1f, -0.2f, 0.0f, //Triangulo 1
        -0.2f,  0.3f, 0.0f,     -0.1f, -0.2f, 0.0f,     -0.1f, 0.3f, 0.0f,  //Triangulo 2
        // Rectangulo superior
        -0.1f, 0.3f, 0.0f,      -0.1f, 0.2f, 0.0f,      0.1f, 0.2f, 0.0f,   //Triangulo 3
        -0.1f, 0.3f, 0.0f,      0.1f, 0.2f, 0.0f,       0.1f, 0.3f, 0.0f,   //Triangulo 4
        // Rectangulo inferior
        -0.1f, -0.1f, 0.0f,     -0.1f, -0.2f, 0.0f,     0.1f, -0.2f, 0.0f,  //Triangulo 5
        -0.1f, -0.1f, 0.0f,     0.1f, -0.2f, 0.0f,      0.1f, -0.1f, 0.0f,  //Triangulo 6

        // LETRA M (Conformada por 8 triángulos):
        //Rectangulo vertical izquierdo
        0.2f,  0.3f, 0.0f,      0.2f, -0.2f, 0.0f,      0.3f, -0.2f, 0.0f,  //Triangulo 1
        0.2f,  0.3f, 0.0f,      0.3f, -0.2f, 0.0f,      0.3f, 0.3f, 0.0f,   //Triangulo 2
        //Rectangulo vertical derecho
        0.5f,  0.3f, 0.0f,      0.5f, -0.2f, 0.0f,      0.6f, -0.2f, 0.0f,  //Triangulo 3
        0.5f,  0.3f, 0.0f,      0.6f, -0.2f, 0.0f,      0.6f, 0.3f, 0.0f,   //Triangulo 4
        // Diagonales
        0.3f, 0.3f, 0.0f,       0.4f, 0.2f, 0.0f,       0.3f, 0.1f, 0.0f,   //Triangulo 5
        0.3f, 0.1f, 0.0f,       0.4f, 0.2f, 0.0f,       0.4f, 0.0f, 0.0f,   //Triangulo 6
        0.5f, 0.3f, 0.0f,       0.4f, 0.2f, 0.0f,       0.5f, 0.1f, 0.0f,   //Triangulo 7
        0.5f, 0.1f, 0.0f,       0.4f, 0.2f, 0.0f,       0.4f, 0.0f, 0.0f   //Triangulo 8
        // ----------------------------------------------------
    };
        

    glGenVertexArrays(1, &VAO);     //generar 1 VAO
    glBindVertexArray(VAO);         //asignar VAO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //pasarle los datos al VBO asignando tamano, los datos y en este caso es est tico pues no se modificar n los valores
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    //Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
    glEnableVertexAttribArray(0);
    //agregar valores a v rtices y luego declarar un nuevo vertexAttribPointer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) 
//Función para agregar los shaders a la tarjeta gráfica 
// the Program recibe los datos de theShader
{
    GLuint theShader = glCreateShader(shaderType);  //theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
    const GLchar* theCode[1];
    theCode[0] = shaderCode;                        //shaderCode es el texto que se le pasa a theCode
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);             //longitud del texto
    glShaderSource(theShader, 1, theCode, codeLength);//Se le asigna al shader el c digo
    glCompileShader(theShader);                     //Se comila el shader
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    //verificaciones y prevenci n de errores
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
        return;
    }
    glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el c digo a la tarjeta gr fica
}


void CompileShaders() {
    shader = glCreateProgram(); //se crea un programa
    if (!shader)
    {
        printf("Error creando el shader");
        return;
    }
    AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
    //Para terminar de linkear el programa y ver que no tengamos errores
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glLinkProgram(shader);//se linkean los shaders a la tarjeta gr fica
    //verificaciones y prevenci n de errores
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("EL error al linkear es: %s \n", eLog);
        return;
    }
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("EL error al validar es: %s \n", eLog);
        return;
    }
}

int main()
{
    // CAMBIO 2: ----------------------------------------------
    // Inicializar semilla aleatoria
    srand(static_cast<unsigned int>(time(0)));
    //---------------------------------------------------------

    //Inicialización de GLFW
    if (!glfwInit()) return 1;
    //****  LAS SIGUIENTES 4 L NEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSIÓN DE OPENGL QUE TIENE ****/
    //Asignando variables de GLFW y propiedades de ventana
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //para solo usar el core profile de OpenGL y no tener retrocompatibilidad
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //CREAR VENTANA
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "CMEP- PRIMERA PRACTICA: Actividad 1 y 2", NULL, NULL);
    if (!mainWindow)
    {
        printf("Fallo en crearse la ventana con GLFW");
        glfwTerminate();
        return 1;
    }

    //Obtener tamaño de Buffer
    int BufferWidth, BufferHeight;
    glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);
    //asignar el contexto
    glfwMakeContextCurrent(mainWindow);
    //permitir nuevas extensiones
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("Fallo la inicialización de GLEW");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Asignar valores de la ventana y coordenadas
    //Asignar Viewport
    glViewport(0, 0, BufferWidth, BufferHeight);
    //Llamada a las funciones creadas antes del main
    CrearTriangulo();
    CompileShaders();

 

    //Loop mientras no se cierra la ventana
    while (!glfwWindowShouldClose(mainWindow))
    {
        //Recibir eventos del usuario
        glfwPollEvents();
        // Cambio 3: ------------------------------------------
        // Control del tiempo para el cambio de color (cada 2.0 segundos)
        float tiempoActual = (float)glfwGetTime();
        if (tiempoActual - tiempoAnterior >= 2.0f)
        {
            // Generar numeros para el RGB de forma aleatoria
            r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            tiempoAnterior = tiempoActual;
        }
        // Limpiar la ventana con el nuevo color aleatorio
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // ----------------------------------------------------

        glUseProgram(shader);
        glBindVertexArray(VAO);

        // Cambio 5: ------------------------------------------
        // DIBUJAR: Ahora son 66 vértices en total (8+8+6 = 22  -> 22*3 = 66)
        glDrawArrays(GL_TRIANGLES, 0, 66);
        // ----------------------------------------------------
        glBindVertexArray(0);
        glUseProgram(0);
        glfwSwapBuffers(mainWindow);
        //NO ESCRIBIR NINGUNA LINEA DESPUES DE glfwSwapBuffers(mainWindow); 
    }
    return 0;
}