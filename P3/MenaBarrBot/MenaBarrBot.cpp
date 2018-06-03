/*
 * MenaBarrBot.cpp
 *
 *  Created on: 2 JUN. 2018
 *      Author: Miguel Angel Mena Barrera
 */

#include "MenaBarrBot.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <limits>
#include <algorithm>    // std::max

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



string mostrarJugador(const Player & p) {
	if(p == J1) {
		return "J1";
	}
	else {
		return "J2";
	}
}


int MenaBarrBot::peso3(const GameState& st){
	contrario = (this->getPlayer()==J1) ? J2 : J1;

	int mias =0 ;int otro = 0;
	Player yo = this->getPlayer();


	mias += st.getSeedsAt(yo,(Position)0);
	mias += st.getSeedsAt(yo,(Position)1);
	mias += st.getSeedsAt(yo,(Position)2);
	mias += st.getSeedsAt(yo,(Position)3);
	mias += st.getSeedsAt(yo,(Position)4);
	mias += st.getSeedsAt(yo,(Position)5);
	mias += st.getSeedsAt(yo,(Position)6);
	
	otro += st.getSeedsAt(contrario,(Position)0);
	otro += st.getSeedsAt(contrario,(Position)1);
	otro += st.getSeedsAt(contrario,(Position)2);
	otro += st.getSeedsAt(contrario,(Position)3);
	otro += st.getSeedsAt(contrario,(Position)4);
	otro += st.getSeedsAt(contrario,(Position)5);
	otro += st.getSeedsAt(contrario,(Position)6);
	

	mias*=10;
	otro*=10;
	
	if(st.getWinner()==yo){
		mias+=20;
	}
	if(st.getWinner()==contrario){
		otro+=20;
	}
	
	return  mias - otro;
	
}


int MenaBarrBot::peso1(const GameState &st){
	
	int g1 , g2;

	Player yo = (*this).getPlayer();
	contrario = (this->getPlayer()==J1) ? J2 : J1;


	g1 = g2 = 0;

	g1 = st.getScore(yo);
	g2 = st.getScore(contrario);
	int lo_que_falta_oponente = 24 - g2;

	

	return (g1 - g2 + lo_que_falta_oponente) ;
}

int MenaBarrBot::pesoMaxmov(GameState st){
	contrario = (this->getPlayer()==J1) ? J2 : J1;
	Player yo = this->getPlayer();
	int semillas_yo = 0;
	int semillas_contrario = 0 ;
	int mi_granero = st.getScore(yo) * 3.45;
	int su_granero = st.getScore(contrario) * 3.45;

	for (int i = 1; i<=6; i++){
		semillas_yo += st.getSeedsAt(yo,(Position)i);
		semillas_contrario += st.getSeedsAt(contrario,(Position)i);
	}
	semillas_yo *= 0.45;
	semillas_contrario *= 0.45;

	return (semillas_yo + mi_granero) - (su_granero + semillas_contrario);
	
}
	
//
// Funcion auxiliar que ayuda a la depuracion
//
string desplazar(int nivel) {
	string r;
	for(int i = 0; i < nivel; i++) {
		r.push_back(' ');
		r.push_back(' ');
		r.push_back(' ');
		r.push_back(' ');
	}
	return r;
}

/////////////////////////
//
//	Este codigo se ha diseñado partiendo del siguiente pseucodigo
//	https://es.wikipedia.org/wiki/Poda_alfa-beta
//
/////////////////////////

int MenaBarrBot::podaAlfaBeta(int limit,const GameState &status, Move & move, const Player & pla,  int alpha, int beta, int contador){
	int v;
	// cerr << desplazar(2-limit) << "A&B (" << contador << ") = " << mostrarJugador(j) << ", " << mostrarJugador(getPlayer()) << "(getPlayer)" << endl;

	///////////////////////////////////////////////////////////////
	//
	// si es status final devuelvo el valor heristico
	// o si he llegado a la profundidad que se me ha indicado
	//
	//////////////////////////////////////////////////////////////

	Move movimientoRec;

	if (limit > 0 && !status.isFinalState()){

		if(pla == getPlayer() ){   
			// estamos en un nodo max -> vamos a maximizar

			// cerr << desplazar(2-limit) << "MAX || >>" << endl;
			for(int i = 1; i <= 6; i++){
				// recorremos cada uno de los semilleros

				// cerr << desplazar(2-limit) << " Posicion=" << i << " tiene en granero " << (int)status.getSeedsAt(j,(Position)i) << endl;
				if(status.getSeedsAt(pla,(Position)i) > 0){
					// si hay semillas -> se simula el status
					// cerr << desplazar(2-limit) << " ACEPTADO" << endl;
					
					GameState next_st = status.simulateMove((Move)i);

					v = podaAlfaBeta(limit-1,next_st,movimientoRec,next_st.getCurrentPlayer(),alpha,beta, contador+1);

					// maximo entre alfa y valor anterior
					if(alpha < v){
						move = (Move)i;
						alpha = v;
						// cerr << desplazar(2-limit) << " |||| SELECCIONADO " << i << " |||" << endl;
					}
					
					if(beta <= alpha) { //poda
						// cerr << desplazar(2-limit) << "MAX || << (PODADO) " << move << endl;
						return beta;
					}
				}
			}
			// cerr << desplazar(2-limit) << "MAX || << " << move << endl;
			return alpha;
		}
		else{
			// cerr << desplazar(2-limit) << "MIN || <<" << endl;
		for(int i = 1; i < 7; i++){
			// cerr << desplazar(2-limit) << " Posicion=" << i << " tiene en granero " << (int)status.getSeedsAt(j,(Position)i) << endl;
			// cerr << desplazar(2-limit) << " ACEPTADO" << endl;
			GameState next_st =  status.simulateMove((Move)i);
			v = podaAlfaBeta(limit-1,next_st,movimientoRec,next_st.getCurrentPlayer(),alpha,beta, contador+1);


			if(beta > v){
				move  = (Move)i;
				beta = v;
			}

			if ( beta <= alpha){ // poda
				// cerr << desplazar(2-limit) << "MIN || << (PODADO) " << move << endl;
				return alpha;
				//
			}
			//}

		}
			// cerr << desplazar(2-limit) << "MIN || << " << move << endl;
			return beta;
			//
		}
	}
	else{
		//evaluio nodo final 
		return peso1(status);
	}
}


Move MenaBarrBot::nextMove(const vector<Move> &adversary, const GameState &state) {

	Player turno= state.getCurrentPlayer() ;
	Move movimiento= M_NONE;

	int v;

    // Declaramos alpha y beta como +inf y -inf
	int a = numeric_limits<int>::min();
	int b = numeric_limits<int>::max();

	v = podaAlfaBeta(12, state,movimiento, turno ,a,b, 0);

	return movimiento;
}
