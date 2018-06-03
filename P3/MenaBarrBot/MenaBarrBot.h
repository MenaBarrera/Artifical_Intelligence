/*
 * MenaBarrBot.h
 *
 *  Created on: 2 JUN. 2018
 *      Author: Miguel Angel Mena Barrera
 */

#include "Bot.h"

#ifndef MANUPCBOT_H_
#define MANUPCBOT_H_

class MenaBarrBot:Bot {
private:
	Player contrario;
public:
	MenaBarrBot();
	~MenaBarrBot();

	void initialize();
	string getName();
	Move nextMove(const vector<Move> &adversary, const GameState &state);
	Move Minimax(const GameState &estado, int limite, const Player & j, int & valor );
	int podaAlfaBeta(int limit,const GameState &status, Move & mov, const Player & j,  int alpha, int beta, int contador);	
	
	// Heuristicas --> ver mas info en el pdf
	
	int peso(const GameState &st);
	int peso1(const GameState &st);
	int peso3(const GameState &st);
	int pesoMaxmov(GameState);



	
};

#endif /* MANUPCBOT_H_ */
