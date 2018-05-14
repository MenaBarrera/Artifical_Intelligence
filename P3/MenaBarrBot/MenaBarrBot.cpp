/*
 * MenaBarrBot.cpp
 *
 *  Created on: 15 ene. 2018
 *      Author: MenaBarr
 */

#include "MenaBarrBot.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <limits>

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

int MenaBarrBot::evaluoTablero(const GameState &st, const Player & jug){
	int misSemillas , semillasContrario;
	Player contrario = J1;

	if(jug == J1)
		contrario = J2;
	
	misSemillas = semillasContrario = 0;

	for(int i=0; i<=6; i++){
		misSemillas += st.getSeedsAt(jug,(Position)i);
		semillasContrario += st.getSeedsAt(contrario, (Position)i);
	}
	return misSemillas -  semillasContrario;
}

Move MenaBarrBot::Minimax(const GameState &estado, int limite, const Player & j, int & valor ){
	Move mov;


	if ( (limite > 0 || limite <= -1) && !estado.isFinalState()){
		int valorSigmov;
		Move sigMov;

		if(estado.getCurrentPlayer() == j){   // estamos en un nodo max
			valor = numeric_limits<int>::min();
			
			
			for(int i = 1; i < 7; i++){
				GameState sigEstado = estado.simulateMove((Move)i);
				sigMov = Minimax(sigEstado,limite-1,j,valorSigmov);
				cerr << "estoy en nodo max " <<endl;
				cerr << "valor " << valor <<endl;
				cerr << "Valor sigMov " << valorSigmov << endl;
				

				if(valor < valorSigmov){
					cerr << " eeeeee " <<endl;
					valor = valorSigmov;
					mov = sigMov;
				}
			}
		}
		else{
			valor = numeric_limits<int>::max();
			
			for(int i = 1; i < 7; i++){
				GameState sigEstado = estado.simulateMove((Move)i);
				sigMov = Minimax(sigEstado,limite-1,j,valorSigmov);
				if(valor > valorSigmov){
					valor = valorSigmov;
					mov = sigMov;
				}
			}
		}
	}
	else{
		valor = evaluoTablero(estado,j);
	}
	return mov;
}	

Move MenaBarrBot::nextMove(const vector<Move> &adversary, const GameState &state) {

	//evaluoTablero(state);
	/*
	GameState estado1;
	estado1.turno = J2; //con el que vayamos a calcular el tablero

	estado1.piezas[0][6]=1;
	estado1.piezas[0][5]=1;
	estado1.piezas[0][4]=1;
	estado1.piezas[0][3]=1;
	estado1.piezas[0][2]=1;
	estado1.piezas[0][1]=1;
	estado1.piezas[0][0]=1;

	estado1.piezas[1][6]=1;
	estado1.piezas[1][5]=0;
	estado1.piezas[1][4]=1;
	estado1.piezas[1][3]=1;
	estado1.piezas[1][2]=0;
	estado1.piezas[1][1]=0;
	estado1.piezas[1][0]=1;

	assert(evaluoTablero(estado1,J1) == 3);
	cerr << "no he muerto" << endl;
	*/


	Player turno= this->getPlayer();
	long timeout= this->getTimeOut();

	Move movimiento= M_NONE;

	int v;

	movimiento = Minimax(state,6,turno,v);


	// Implementar aquí la selección de la acción a realizar

	// OJO: Recordatorio. NO USAR cin NI cout.
	// Para salidas por consola (debug) utilizar cerr. Ejemplo:
	// cerr<< "Lo que quiero mostrar"<<endl;


	// OJO: . El nombre del bot y de la clase deben coincidir.
	// En caso contrario, el bot no podrá participar en la competición.
	// Se deberá sustituir el nombre MiBot como nombre de la clase por otro
	// seleccionado por el alumno. Se deberá actualizar también el nombre
	// devuelto por el método getName() acordemente.
/*
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
	*/
	return movimiento;
	

	
}
