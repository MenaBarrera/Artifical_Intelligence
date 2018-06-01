/*
 * MenaBarrBot.cpp
 *
 *  Created on: 2 JUN. 2018
 *      Author: MenaBarr
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
	
	int mias =0 ;int otro = 0;
	Player contrario;
	Player yo = this->getPlayer();

	yo==J1 ? contrario=J2:contrario=J1;

	mias += st.getSeedsAt(yo,(Position)0);
	mias += st.getSeedsAt(yo,(Position)1);
	mias += st.getSeedsAt(yo,(Position)2);
	mias += st.getSeedsAt(yo,(Position)3);
	mias += st.getSeedsAt(yo,(Position)4);
	mias += st.getSeedsAt(yo,(Position)5);
	
	otro += st.getSeedsAt(contrario,(Position)0);
	otro += st.getSeedsAt(contrario,(Position)1);
	otro += st.getSeedsAt(contrario,(Position)2);
	otro += st.getSeedsAt(contrario,(Position)3);
	otro += st.getSeedsAt(contrario,(Position)4);
	otro += st.getSeedsAt(contrario,(Position)5);
	

	mias*=10;
	otro*=10;
	
	if(st.getWinner()==yo){
		mias+=500;
	}
	if(st.getWinner()==contrario){
		otro+=500;
	}
	
	return  mias - otro;

	
}

int MenaBarrBot::peso1(const GameState &st){
	/*
		H4: Maximize the amount of counters in a players own store.
	 	This heuristic aims to pick a move that will maximize the amount of counters captured. 
	 	It has a look ahead of one.
	 */

	int g1 , g2;
	Player yo = (*this).getPlayer();
	Player contrario = J1;

	if(this->getPlayer() == J1)
		contrario = J2;
	
	g1 = g2 = 0;

	g1 = st.getScore(yo);
	g2 = st.getScore(contrario);

	return g1 - g2;

}

int MenaBarrBot::pesoMaxmov(GameState st){
	int maximo=0;

	for(int i=1; i<7; i++){
		max(maximo,(int)st.getSeedsAt(this->getPlayer(),(Position)i)) ;
	}

	return maximo;

}



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

int MenaBarrBot::podaAlfaBeta(int limit,const GameState &status, Move & mov, const Player & j,  int alpha, int beta, int contador){

	int valor;
	Move m_aux;

	// cerr << desplazar(2-limit) << "A&B (" << contador << ") = " << mostrarJugador(j) << ", " << mostrarJugador(getPlayer()) << "(getPlayer)" << endl;


	//
	// si es status final devuelvo el valor heristico
	// o si he llegado a la profundidad que se me ha indicado
	// si la prof es <0 entonces -> sin profundidad
	//

	if ( (limit > 0 || limit <= -1) && !status.isFinalState()){

		if(j == getPlayer() ){   
			// estamos en un nodo max -> vamos a maximizar
			// cerr << desplazar(2-limit) << "MAX || >>" << endl;
			for(int i = 1; i < 7; i++){
				// recorremos cada uno de los semilleros

				// cerr << desplazar(2-limit) << " Posicion=" << i << " tiene en granero " << (int)status.getSeedsAt(j,(Position)i) << endl;
				if(status.getSeedsAt(j,(Position)i) > 0){
					// si hay semillas -> se simula el status

					// cerr << desplazar(2-limit) << " ACEPTADO" << endl;
					GameState next_st = status.simulateMove((Move)i);

					valor = podaAlfaBeta(limit-1,next_st,m_aux,next_st.getCurrentPlayer(),alpha,beta, contador+1);


					if(alpha < valor){
						mov = (Move)i;
						alpha = valor;
						// cerr << desplazar(2-limit) << " |||| SELECCIONADO " << i << " |||" << endl;
					}
					
					if(beta <= alpha) { //poda
						// cerr << desplazar(2-limit) << "MAX || << (PODADO) " << mov << endl;
						return beta;
					}
				}
			}
			// cerr << desplazar(2-limit) << "MAX || << " << mov << endl;
			return alpha;
		}
		else{
			// cerr << desplazar(2-limit) << "MIN || <<" << endl;
		for(int i = 1; i < 7; i++){
			// cerr << desplazar(2-limit) << " Posicion=" << i << " tiene en granero " << (int)status.getSeedsAt(j,(Position)i) << endl;
			// cerr << desplazar(2-limit) << " ACEPTADO" << endl;
			GameState next_st =  status.simulateMove((Move)i);
			valor = podaAlfaBeta(limit-1,next_st,m_aux,next_st.getCurrentPlayer(),alpha,beta, contador+1);


			if(beta > valor){
				mov  = (Move)i;
				beta = valor;
			}

			if ( beta <= alpha){ // poda
				// cerr << desplazar(2-limit) << "MIN || << (PODADO) " << mov << endl;
				return alpha;
				//
			}
			//}

		}
			// cerr << desplazar(2-limit) << "MIN || << " << mov << endl;
			return beta;
			//
		}
	}
	else{
		//evaluio nodo final 
		return peso3(status);
	}
}


Move MenaBarrBot::nextMove(const vector<Move> &adversary, const GameState &state) {

	Player turno= state.getCurrentPlayer() ;
	Move movimiento= M_NONE;

	int v;

    // Declaramos alpha y beta como +inf y -inf
	int a = numeric_limits<int>::min();
	int b = numeric_limits<int>::max();

	v = podaAlfaBeta(9, state,movimiento, turno ,a,b, 0);

	return movimiento;
}
