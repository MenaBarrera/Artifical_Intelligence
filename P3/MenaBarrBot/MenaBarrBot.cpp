/*
 * ManuPCBot.cpp
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#include "MenaBarrBot.h"

#include <string>
#include <cstdlib>
#include <iostream>
using namespace std;

MenaBarrBot::MenaBarrBot() {
	// Inicializar las variables necesarias para ejecutar la partida
	srand(time(0));

}

MenaBarrBot::~MenaBarrBot() {
	// Liberar los recursos reservados (memoria, ficheros, etc.)
}

void MenaBarrBot::initialize() {
	// Inicializar el bot antes de jugar una partida
}

string MenaBarrBot::getName() {
	return "MenaBarrBot"; // Sustituir por el nombre del bot
}

int MenaBarrBot::evaluoTablero(const GameState &estado){
	return 
}

Move MenaBarrBot::nextMove(const vector<Move> &adversary, const GameState &state) {


	Player turno= this->getPlayer();
	long timeout= this->getTimeOut();

	Move movimiento= M_NONE;

	// Implementar aquí la selección de la acción a realizar

	// OJO: Recordatorio. NO USAR cin NI cout.
	// Para salidas por consola (debug) utilizar cerr. Ejemplo:
	// cerr<< "Lo que quiero mostrar"<<endl;


	// OJO: . El nombre del bot y de la clase deben coincidir.
	// En caso contrario, el bot no podrá participar en la competición.
	// Se deberá sustituir el nombre MiBot como nombre de la clase por otro
	// seleccionado por el alumno. Se deberá actualizar también el nombre
	// devuelto por el método getName() acordemente.

	int available= 0; // Movimientos disponibles;

	for (int i= 1; i<=6;i++) {
		if (state.getSeedsAt(turno, (Position) i) >0)
			available++;
	}

	int n= 1+ (rand()%available);
	int aux= 0;

	for (int i= 1; i<=6 && movimiento == M_NONE; i++) {
		if (state.getSeedsAt(turno, (Position) i) >0)
			aux++;
		if (aux == n)
			movimiento= (Move)i;
	}


	return movimiento;
}
