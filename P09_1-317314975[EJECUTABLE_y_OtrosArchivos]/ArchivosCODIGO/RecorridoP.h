#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

// Cada estructura "vehiculo" esta compuesta por..."
struct Vehiculo {
	// Coordenadas de posición en nuestro escenario
	float posX;
	float posY;
	float posZ;
	// Rotaciones de Euler para alinear nuestros modelos con la pista
	float anguloX;
	float anguloY;
	float anguloZ;
	int etapa;			// Numero de etapa (parte de recorrido) donde estamos
	bool deRegreso;		// ¿Vamos de 0 -> -x o de -x -> 0?
	bool termino;		// ¿Llegamos a la meta?
};

// ====================================================================================
// CLASE: RecorridoP (Gestor de la Animación)
// ====================================================================================
class RecorridoP {
public:

	// --- ATRIBUTOS PÚBLICOS ---
	// Para empaquetar otros modelos que igual ocupen animarse :3
	Vehiculo coche;
	Vehiculo nave;

	// --- MÉTODOS ---
	// 1. Constructor
	RecorridoP();

	// 2. Función de Actualización: 
	// Recibe 3 parámetros necesarios para funcionar:
	// - deltaTime: Para que el movimiento sea fluido sin importar los FPS de la computadora.
	// - velTramoX: La velocidad base del recorrido (ya que X va a ser marcador de distancia)
	// - reiniciaCoche (bool&): La referencia a la tecla 'M'
	void actualizarRuta(float deltaTime, float velTramoX, bool& reiniciaCoche);
	// & -> Paso por referencia... Reseteo del coche
};