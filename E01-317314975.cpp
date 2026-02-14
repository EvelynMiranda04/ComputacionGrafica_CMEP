#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>

//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;

// LENGUAJE DE SHADER
static const char* vShader = "                              \n\
#version 330                                                \n\
layout (location = 0) in vec3 pos;                          \n\
void main()                                                 \n\
{                                                           \n\
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);          \n\
}";

// Fragment Shader con color blanco para las figuras
//recibir Vcolor y dar de salida color
static const char* fShader = "                              \n\
#version 330                                                \n\
out vec4 color;                                             \n\
void main()                                                 \n\
{                                                           \n\
    color = vec4(1.0f, 1.0f, 1.0f, 1.0f);                   \n\
}";

void CrearTriangulo()
{
    GLfloat vertices[] = {
        // Como estamos en 2D el ultimo valor (Z) 
        // lo dejamos siempre en 0 ----     X,Y,Z
        
        //ROMBO: Conformado por dos triángulos
        // Triángulo de arriba
        -0.8f,  0.0f,  0.0f,  // Izq
        -0.5f,  0.45f, 0.0f,  // Arriba
        -0.2f,  0.0f,  0.0f,  // Der
        // Triángulo de abajo
        -0.8f,  0.0f,  0.0f,  // Izq
        -0.5f, -0.45f, 0.0f,  // Abajo
        -0.2f,  0.0f,  0.0f,  // Der

        //TRAPECIO: Conformado por 4 triángulos 
        //  (2 para el rectángulo central 
        //  y 1 para cada triángulo lateral
        // Rectángulo central (2 triángulos)
            //Izquierdo
        0.0f, -0.2f, 0.0f,    
        0.4f, -0.2f, 0.0f,    
        0.0f, 0.3f, 0.0f,
            //Derecho
        0.0f,  0.3f, 0.0f,    
        0.4f, -0.2f, 0.0f,    
        0.4f, 0.3f, 0.0f,
        // Triángulo izquierdo
        -0.2f, -0.2f, 0.0f,    
        0.0f, -0.2f, 0.0f,    
        0.0f, 0.3f, 0.0f,
        // Triángulo derecho
        0.4f, -0.2f, 0.0f,    
        0.6f, -0.2f, 0.0f,    
        0.4f, 0.3f, 0.0f
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


void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Funci n para agregar los shaders a la tarjeta gr fica

//the Program recibe los datos de theShader

{
    GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
    const GLchar* theCode[1];
    theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);//longitud del texto
    glShaderSource(theShader, 1, theCode, codeLength);//Se le asigna al shader el c digo
    glCompileShader(theShader);//Se comila el shader
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
    //Inicialización de GLFW
    if (!glfwInit()) return 1;
    //****  LAS SIGUIENTES 4 L NEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSI N DE OPENGL QUE TIENE ****/
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

    // Controladores de tiempo para cambiar el color cada 1.5 segundos
    float tiempoAnterior = 0.0f;
    int indiceColor = 0;
    float colores[3][3] = {
        {1.0f, 0.0f, 0.0f}, // Rojo
        {0.0f, 1.0f, 0.0f}, // Verde
        {0.0f, 0.0f, 1.0f}  // Azul
    };

    //Loop mientras no se cierra la ventana
    while (!glfwWindowShouldClose(mainWindow))
    {
        //Recibir eventos del usuario
        glfwPollEvents();

        float tiempoActual = (float)glfwGetTime();
        if (tiempoActual - tiempoAnterior >= 1.5f) {
            indiceColor = (indiceColor + 1) % 3;
            tiempoAnterior = tiempoActual;
        }

        //Limpiar la ventana
        glClearColor(colores[indiceColor][0], colores[indiceColor][1], colores[indiceColor][2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(VAO);

        // DIBUJAR: Ahora son 18 vértices en total (6 del rombo + 12 del trapecio)
        glDrawArrays(GL_TRIANGLES, 0, 18);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
        //NO ESCRIBIR NINGUNA LINEA DESPUES DE glfwSwapBuffers(mainWindow); 

    }

    return 0;
}