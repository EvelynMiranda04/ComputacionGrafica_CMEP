#include "RecorridoP.h"


// CONSTRUCTOR: Valores Iniciales
RecorridoP::RecorridoP() {

	// INICIALIZACION COCHE Y NAVE: 
	// Orden: posX, posY, posZ, anguloX, anguloY, anguloZ, etapa, deRegreso, termino
	coche = { 0.0f, -1.0f, 0.0f, 90.0f, 90.0f, 0.0f, 1, false, false };
	nave = { -177.46f, 25.389f, -23.568f, 96.958f, 70.654f, 1.6187f, 17, true, false };
}


// FUNCIÓN PRINCIPAL: Actualización de la Máquina de Estados
void RecorridoP::actualizarRuta(float deltaTime, float velTramoX, bool& reiniciaCoche) {

	// 1. Empaquetamos los vehículos en un arreglo de punteros. 
	// Al usar punteros (&), lo que modifiquemos aquí afectará a las variables originales.
	Vehiculo* vehiculos[2] = { &coche, &nave };

	// 2. DECLARACIÓN DE VARIABLES DE APOYO (Reutilizables en cada ciclo del for)
	int i_vehiculo;		// 0: Primera vuelta - 1: Segunda vuelta
	Vehiculo* v;		// Coche o Nave
	float dtV;			// Deltatime V (+deltatime: Ida - -deltatime: regreso)
	float distX;		// Guardamos el tramo en X a recorrer

	// 3. CICLO: Primero el coche (0) y luego la nave (1)
	for (i_vehiculo = 0; i_vehiculo < 2; i_vehiculo++) {
		v = vehiculos[i_vehiculo]; // 'v' ahora apunta al vehículo en turno

		// ¿El vehículo ya llegó a su destino y está apagado?
		if (v->termino) {

			// Si es el coche (0) y el usuario presionó la tecla 'M' (reiniciaCoche = true)
			if (i_vehiculo == 0 && reiniciaCoche) {
				v->termino = false;    // Le quitamos la pausa
				v->deRegreso = false;  // Lo ponemos en dirección de ida
				v->etapa = 1;          // Lo reseteamos a la etapa 1

				// Apagamos gracias al paso por referencia (&)
				reiniciaCoche = false;
			}
			else {
				// Si es la nave (que no se reinicia) o no apretamos 'M' usamos 'continue'
				continue;
			}
		}

		// TRUCO EN Ping-Pong
		// Si es TRUE, el tiempo corre hacia atrás (-deltaTime). 
		// Si es FALSE, el tiempo corre normal (deltaTime).
		dtV = v->deRegreso ? -deltaTime : deltaTime;


		// ====================================================================================
		// ETAPA 1: Inicio del recorrido (Matemática Base)
		// ====================================================================================
		if (v->etapa == 1) {
			// distX: Distancia total a recorrer en el eje X durante esta etapa.
			// Sirve como divisor para calcular las proporciones exactas de Y, Z y ángulos.
			distX = 10.544f;

			// ---  Movimiento ---
			// El vehículo avanza a velocidad constante en X (velTramoX).
			v->posX -= velTramoX * dtV;

			// Para Y y Z calculamos la proporción: (Destino - Origen) / distX.
			// Al multiplicar esto por velTramoX, obligamos a que alcancen su destino a la par.
			v->posY += (velTramoX * ((-1.0f - -1.0f) / distX)) * dtV;
			v->posZ += (velTramoX * ((0.61976f - 0.0f) / distX)) * dtV;

			// --- Rotación ---
			// Misma lógica matemática para que el modelo rote suavemente mientras avanza.
			v->anguloX += (velTramoX * ((90.0f - 90.0f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((93.59f - 90.0f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((0.0f - 0.0f) / distX)) * dtV;

			if (!v->deRegreso) {
				// --- MODO IDA ---
				// Si rebasamos la meta en X (<= porque avanzamos hacia los negativos), anclamos
				// los valores exactos  y pasamos a la siguiente etapa.
				if (v->posX <= -10.544f) {
					v->posX = -10.544f; v->posY = -1.0f; v->posZ = 0.61976f;
					v->anguloX = 90.0f; v->anguloY = 93.59f; v->anguloZ = 0.0f;
					v->etapa = 2;
				}
			}
			else {
				// --- MODO VUELTA (Reversa) ---
				// Si viajamos en reversa y llegamos al origen (0.0), detenemos el vehículo.
				if (v->posX >= 0.0f) {
					v->posX = 0.0f; v->posY = -1.0f; v->posZ = 0.0f;
					v->anguloX = 90.0f; v->anguloY = 90.0f; v->anguloZ = 0.0f;
					v->termino = true; // Ambos V entran en pausa al llegar al inicio
				}
			}
		}

		// ====================================================================================
		// ETAPA 2:
		// ====================================================================================
		else if (v->etapa == 2) {
			distX = 13.056f;
			v->posX -= velTramoX * dtV;
			// calculamos la proporción: (Destino - Origen) / distX.
			v->posY += (velTramoX * ((-1.0f - -1.0f) / distX)) * dtV;
			v->posZ += (velTramoX * ((1.4759f - 0.61976f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((90.0f - 90.0f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((98.094f - 93.59f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((0.0f - 0.0f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -23.6f) { // Meta de Etapa 2
					v->posX = -23.6f; v->posY = -1.0f; v->posZ = 1.4759f;
					v->anguloX = 90.0f; v->anguloY = 98.094f; v->anguloZ = 0.0f;
					v->etapa = 3;
				}
			}
			else {
				if (v->posX >= -10.544f) { // Origen de Etapa 2 (Para la vuelta)
					v->posX = -10.544f; v->posY = -1.0f; v->posZ = 0.61976f;
					v->anguloX = 90.0f; v->anguloY = 93.59f; v->anguloZ = 0.0f;
					v->etapa = 1;
				}
			}
		}

		// --- ETAPA 3 ---
		else if (v->etapa == 3) {
			distX = 10.437f;
			v->posX -= velTramoX * dtV;
			v->posY += (velTramoX * ((-1.0f - -1.0f) / distX)) * dtV;
			v->posZ += (velTramoX * ((3.3737f - 1.4759f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((90.0f - 90.0f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((104.08f - 98.094f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((0.0f - 0.0f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -34.037f) {
					v->posX = -34.037f; v->posY = -1.0f; v->posZ = 3.3737f;
					v->anguloX = 90.0f; v->anguloY = 104.08f; v->anguloZ = 0.0f;
					v->etapa = 4;
				}
			}
			else {
				if (v->posX >= -23.6f) {
					v->posX = -23.6f; v->posY = -1.0f; v->posZ = 1.4759f;
					v->anguloX = 90.0f; v->anguloY = 98.094f; v->anguloZ = 0.0f;
					v->etapa = 2;
				}
			}
		}

		// --- ETAPA 4 ---
		else if (v->etapa == 4) {
			distX = 9.383f;
			v->posX -= velTramoX * dtV;
			v->posY += (velTramoX * ((-1.0f - -1.0f) / distX)) * dtV;
			v->posZ += (velTramoX * ((6.5839f - 3.3737f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((90.0f - 90.0f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((113.53f - 104.08f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((0.0f - 0.0f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -43.42f) {
					v->posX = -43.42f; v->posY = -1.0f; v->posZ = 6.5839f;
					v->anguloX = 90.0f; v->anguloY = 113.53f; v->anguloZ = 0.0f;
					v->etapa = 5;
				}
			}
			else {
				if (v->posX >= -34.037f) {
					v->posX = -34.037f; v->posY = -1.0f; v->posZ = 3.3737f;
					v->anguloX = 90.0f; v->anguloY = 104.08f; v->anguloZ = 0.0f;
					v->etapa = 3;
				}
			}
		}

		// --- ETAPA 5 ---
		else if (v->etapa == 5) {
			distX = 4.644f;
			v->posX -= velTramoX * dtV;
			v->posY += (velTramoX * ((-1.0f - -1.0f) / distX)) * dtV;
			v->posZ += (velTramoX * ((9.4087f - 6.5839f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((90.0f - 90.0f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((97.444f - 113.53f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((0.0f - 0.0f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -48.064f) {
					v->posX = -48.064f; v->posY = -1.0f; v->posZ = 9.4087f;
					v->anguloX = 90.0f; v->anguloY = 97.444f; v->anguloZ = 0.0f;
					v->etapa = 6;
				}
			}
			else {
				if (v->posX >= -43.42f) {
					v->posX = -43.42f; v->posY = -1.0f; v->posZ = 6.5839f;
					v->anguloX = 90.0f; v->anguloY = 113.53f; v->anguloZ = 0.0f;
					v->etapa = 4;
				}
			}
		}

		// --- ETAPA 6 ---
		else if (v->etapa == 6) {
			distX = 6.776f;
			v->posX -= velTramoX * dtV;
			v->posY += (velTramoX * ((0.4627f - -1.0f) / distX)) * dtV;
			v->posZ += (velTramoX * ((9.4087f - 9.4087f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((83.506f - 90.0f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((77.925f - 97.444f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((1.3952f - 0.0f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -54.84f) {
					v->posX = -54.84f; v->posY = 0.4627f; v->posZ = 9.4087f;
					v->anguloX = 83.506f; v->anguloY = 77.925f; v->anguloZ = 1.3952f;
					v->etapa = 7;
				}
			}
			else {
				if (v->posX >= -48.064f) {
					v->posX = -48.064f; v->posY = -1.0f; v->posZ = 9.4087f;
					v->anguloX = 90.0f; v->anguloY = 97.444f; v->anguloZ = 0.0f;
					v->etapa = 5;
				}
			}
		}

		// --- ETAPA 7 ---
		else if (v->etapa == 7) {
			distX = 1.907f;
			v->posX -= velTramoX * dtV;
			v->posY += (velTramoX * ((0.7826f - 0.4627f) / distX)) * dtV;
			v->posZ += (velTramoX * ((7.7146f - 9.4087f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((83.506f - 83.506f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((42.135f - 77.925f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((1.3952f - 1.3952f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -56.747f) {
					v->posX = -56.747f; v->posY = 0.7826f; v->posZ = 7.7146f;
					v->anguloX = 83.506f; v->anguloY = 42.135f; v->anguloZ = 1.3952f;
					v->etapa = 8;
				}
			}
			else {
				if (v->posX >= -54.84f) {
					v->posX = -54.84f; v->posY = 0.4627f; v->posZ = 9.4087f;
					v->anguloX = 83.506f; v->anguloY = 77.925f; v->anguloZ = 1.3952f;
					v->etapa = 6;
				}
			}
		}

		// --- ETAPA 8 ---
		else if (v->etapa == 8) {
			distX = 7.694f;
			v->posX -= velTramoX * dtV;
			v->posY += (velTramoX * ((1.9633f - 0.7826f) / distX)) * dtV;
			v->posZ += (velTramoX * ((0.59452f - 7.7146f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((80.775f - 83.506f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((42.484f - 42.135f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((6.8825f - 1.3952f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -64.441f) {
					v->posX = -64.441f; v->posY = 1.9633f; v->posZ = 0.59452f;
					v->anguloX = 80.775f; v->anguloY = 42.484f; v->anguloZ = 6.8825f;
					v->etapa = 9;
				}
			}
			else {
				if (v->posX >= -56.747f) {
					v->posX = -56.747f; v->posY = 0.7826f; v->posZ = 7.7146f;
					v->anguloX = 83.506f; v->anguloY = 42.135f; v->anguloZ = 1.3952f;
					v->etapa = 7;
				}
			}
		}

		// --- ETAPA 9 ---
		else if (v->etapa == 9) {
			distX = 8.773f;
			v->posX -= velTramoX * dtV;
			v->posY += (velTramoX * ((4.3884f - 1.9633f) / distX)) * dtV;
			v->posZ += (velTramoX * ((-8.1342f - 0.59452f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((75.46f - 80.775f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((59.278f - 42.484f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((4.3401f - 6.8825f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -73.214f) {
					v->posX = -73.214f; v->posY = 4.3884f; v->posZ = -8.1342f;
					v->anguloX = 75.46f; v->anguloY = 59.278f; v->anguloZ = 4.3401f;
					v->etapa = 10;
				}
			}
			else {
				if (v->posX >= -64.441f) {
					v->posX = -64.441f; v->posY = 1.9633f; v->posZ = 0.59452f;
					v->anguloX = 80.775f; v->anguloY = 42.484f; v->anguloZ = 6.8825f;
					v->etapa = 8;
				}
			}
		}

		// --- ETAPA 10 ---
		else if (v->etapa == 10) {
			distX = 7.314f;
			v->posX -= velTramoX * dtV;
			v->posY += (velTramoX * ((6.7213f - 4.3884f) / distX)) * dtV;
			v->posZ += (velTramoX * ((-10.532f - -8.1342f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((69.25f - 75.46f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((80.778f - 59.278f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((-2.4842f - 4.3401f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -80.528f) {
					v->posX = -80.528f; v->posY = 6.7213f; v->posZ = -10.532f;
					v->anguloX = 69.25f; v->anguloY = 80.778f; v->anguloZ = -2.4842f;
					v->etapa = 11;
				}
			}
			else {
				if (v->posX >= -73.214f) {
					v->posX = -73.214f; v->posY = 4.3884f; v->posZ = -8.1342f;
					v->anguloX = 75.46f; v->anguloY = 59.278f; v->anguloZ = 4.3401f;
					v->etapa = 9;
				}
			}
		}

		// --- ETAPA 11 ---
		else if (v->etapa == 11) {
			distX = 12.931f;
			v->posX -= velTramoX * dtV;
			v->posY += (velTramoX * ((12.137f - 6.7213f) / distX)) * dtV;
			v->posZ += (velTramoX * ((-8.2756f - -10.532f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((67.14f - 69.25f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((105.3f - 80.778f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((-12.828f - -2.4842f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -93.459f) {
					v->posX = -93.459f; v->posY = 12.137f; v->posZ = -8.2756f;
					v->anguloX = 67.14f; v->anguloY = 105.3f; v->anguloZ = -12.828f;
					v->etapa = 12;
				}
			}
			else {
				if (v->posX >= -80.528f) {
					v->posX = -80.528f; v->posY = 6.7213f; v->posZ = -10.532f;
					v->anguloX = 69.25f; v->anguloY = 80.778f; v->anguloZ = -2.4842f;
					v->etapa = 10;
				}
			}
		}

		// --- ETAPA 12 ---
		else if (v->etapa == 12) {
			distX = 17.811f;
			v->posX -= velTramoX * dtV;
			v->posY += (velTramoX * ((20.323f - 12.137f) / distX)) * dtV;
			v->posZ += (velTramoX * ((-5.2718f - -8.2756f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((68.94f - 67.14f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((91.386f - 105.3f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((-2.1826f - -12.828f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -111.27f) {
					v->posX = -111.27f; v->posY = 20.323f; v->posZ = -5.2718f;
					v->anguloX = 68.94f; v->anguloY = 91.386f; v->anguloZ = -2.1826f;
					v->etapa = 13;
				}
			}
			else {
				if (v->posX >= -93.459f) {
					v->posX = -93.459f; v->posY = 12.137f; v->posZ = -8.2756f;
					v->anguloX = 67.14f; v->anguloY = 105.3f; v->anguloZ = -12.828f;
					v->etapa = 11;
				}
			}
		}

		// --- ETAPA 13 ---
		else if (v->etapa == 13) {
			distX = 11.11f;
			v->posX -= velTramoX * dtV;
			v->posY += (velTramoX * ((24.268f - 20.323f) / distX)) * dtV;
			v->posZ += (velTramoX * ((-6.3708f - -5.2718f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((77.82f - 68.94f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((76.554f - 91.386f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((-0.5779f - -2.1826f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -122.38f) {
					v->posX = -122.38f; v->posY = 24.268f; v->posZ = -6.3708f;
					v->anguloX = 77.82f; v->anguloY = 76.554f; v->anguloZ = -0.5779f;
					v->etapa = 14;
				}
			}
			else {
				if (v->posX >= -111.27f) {
					v->posX = -111.27f; v->posY = 20.323f; v->posZ = -5.2718f;
					v->anguloX = 68.94f; v->anguloY = 91.386f; v->anguloZ = -2.1826f;
					v->etapa = 12;
				}
			}
		}

		// --- ETAPA 14 ---
		else if (v->etapa == 14) {
			distX = 20.11f;
			v->posX -= velTramoX * dtV;
			v->posY += (velTramoX * ((27.339f - 24.268f) / distX)) * dtV;
			v->posZ += (velTramoX * ((-11.258f - -6.3708f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((86.84f - 77.82f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((70.695f - 76.554f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((0.89183f - -0.5779f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -142.49f) {
					v->posX = -142.49f; v->posY = 27.339f; v->posZ = -11.258f;
					v->anguloX = 86.84f; v->anguloY = 70.695f; v->anguloZ = 0.89183f;
					v->etapa = 15;
				}
			}
			else {
				if (v->posX >= -122.38f) {
					v->posX = -122.38f; v->posY = 24.268f; v->posZ = -6.3708f;
					v->anguloX = 77.82f; v->anguloY = 76.554f; v->anguloZ = -0.5779f;
					v->etapa = 13;
				}
			}
		}

		// --- ETAPA 15 ---
		else if (v->etapa == 15) {
			distX = 11.98f;
			v->posX -= velTramoX * dtV;
			v->posY += (velTramoX * ((27.491f - 27.339f) / distX)) * dtV;
			v->posZ += (velTramoX * ((-14.96f - -11.258f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((89.988f - 86.84f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((70.676f - 70.695f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((-0.21197f - 0.89183f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -154.47f) {
					v->posX = -154.47f; v->posY = 27.491f; v->posZ = -14.96f;
					v->anguloX = 89.988f; v->anguloY = 70.676f; v->anguloZ = -0.21197f;
					v->etapa = 16;
				}
			}
			else {
				if (v->posX >= -142.49f) {
					v->posX = -142.49f; v->posY = 27.339f; v->posZ = -11.258f;
					v->anguloX = 86.84f; v->anguloY = 70.695f; v->anguloZ = 0.89183f;
					v->etapa = 14;
				}
			}
		}

		// --- ETAPA 16 ---
		else if (v->etapa == 16) {
			distX = 11.38f;
			v->posX -= velTramoX * dtV;
			v->posY += (velTramoX * ((26.779f - 27.491f) / distX)) * dtV;
			v->posZ += (velTramoX * ((-18.545f - -14.96f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((94.994f - 89.988f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((70.722f - 70.676f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((2.3065f - -0.21197f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -165.85f) {
					v->posX = -165.85f; v->posY = 26.779f; v->posZ = -18.545f;
					v->anguloX = 94.994f; v->anguloY = 70.722f; v->anguloZ = 2.3065f;
					v->etapa = 17;
				}
			}
			else {
				if (v->posX >= -154.47f) {
					v->posX = -154.47f; v->posY = 27.491f; v->posZ = -14.96f;
					v->anguloX = 89.988f; v->anguloY = 70.676f; v->anguloZ = -0.21197f;
					v->etapa = 15;
				}
			}
		}

		// ====================================================================================
		// ETAPA 17: Final del recorrido y Lógica de Retorno
		// ====================================================================================
		else if (v->etapa == 17) {
			distX = 11.61f;
			v->posX -= velTramoX * dtV;
			v->posY += (velTramoX * ((25.389f - 26.779f) / distX)) * dtV;
			v->posZ += (velTramoX * ((-23.568f - -18.545f) / distX)) * dtV;
			v->anguloX += (velTramoX * ((96.958f - 94.994f) / distX)) * dtV;
			v->anguloY += (velTramoX * ((70.654f - 70.722f) / distX)) * dtV;
			v->anguloZ += (velTramoX * ((1.6187f - 2.3065f) / distX)) * dtV;

			if (!v->deRegreso) {
				if (v->posX <= -177.46f) { // Llegamos al final absoluto de la pista
					v->posX = -177.46f; v->posY = 25.389f; v->posZ = -23.568f;
					v->anguloX = 96.958f; v->anguloY = 70.654f; v->anguloZ = 1.6187f;

					// --- DECISIÓN FINAL ---
					if (i_vehiculo == 1) {
						// Si es la Nave (índice 1), se queda pausada en la meta.
						v->termino = true;
					}
					else {
						// Si es el Coche (índice 0), se activa la reversa y empieza a retroceder.
						// NOTA: NO cambiamos de etapa, se queda en la 17 para recorrerla hacia atrás.
						v->deRegreso = true;
					}
				}
			}
			else {
				if (v->posX >= -165.85f) { // Origen de la etapa 17 (Para la vuelta del coche)
					v->posX = -165.85f; v->posY = 26.779f; v->posZ = -18.545f;
					v->anguloX = 94.994f; v->anguloY = 70.722f; v->anguloZ = 2.3065f;
					v->etapa = 16;
				}
			}
		}
	}
}