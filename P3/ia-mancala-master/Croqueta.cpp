/*
 * Croqueta.cpp
 * Implementación de un Bot que juega a Mancala utilizando la estrategia
 * conocida como "poda alfa-beta".
 *
 *  Created on: 15 mayo 2018
 *      Author: jmml
 */

#include <string>
#include <cstdlib>
#include <iostream>
#include <limits>       // std::numeric_limits
#include <algorithm>    // std::max
#include <utility>      // std::pair, std::make_pair

#include "Croqueta.h"
using namespace std;

Croqueta::Croqueta()
{
    // Inicializar las variables necesarias para ejecutar la partida

}

Croqueta::~Croqueta()
{
    // Liberar los recursos reservados (memoria, ficheros, etc.)
}

void Croqueta::initialize()
{
    // Inicializar el bot antes de jugar una partida
}

string Croqueta::getName()
{
    return "Croqueta"; // Sustituir por el nombre del bot
}

std::pair<int, Move> Croqueta::AlphaBeta(Node node, int depth, int alpha,
        int beta, bool maximize)
{

    if (depth == 0 || node.state.isFinalState()) {

        Player player = this->getPlayer();

        // La heurística que vamos a utilizar valora dos elementos del juego:
        //
        // - Nuestra puntuación respecto a la del adversario, que queremos
        //   maximizar
        // - Los puntos que necesita el adversario para ganar, que queremos
        //   minimizar
        //
        //  Nuestra heurística será por tanto:
        //    puntuación_jugador - puntuación_adversario
        //                       + puntos_que_necesita_adversario

        int score = node.state.getScore(player);
        int opponent_score;

        if (player == Player::J1) {
            opponent_score = node.state.getScore(Player::J2);
        } else {
            opponent_score = node.state.getScore(Player::J1);
        }

        int opponent_win_short_by = 24 - opponent_score;

        int value = score - opponent_score + opponent_win_short_by;

        return std::make_pair(value, M_NONE);
    }

    if (maximize) {
        int best_value = std::numeric_limits<int>::min();
        Move best_move;

        for (int i = 1; i <= 6; i++) {
            Node child_node = {node.state.simulateMove((Move) i), (Move) i};

            int value;

            // Tener que maximizar o minimizar el siguiente nodo del árbol
            // dependerá de si vuelve a ser el turno del bot.
            if (child_node.state.getCurrentPlayer() == node.state.getCurrentPlayer()) {
                value = AlphaBeta(child_node, depth - 1, alpha, beta, true).first;
            } else {
                value = AlphaBeta(child_node, depth - 1, alpha, beta, false).first;
            }

            if (value > best_value) {
                best_value = value;
                best_move = (Move) i;
            }

            alpha = std::max(alpha, best_value);

            if (beta <= alpha) {
                break;
            }
        }

        return std::make_pair(best_value, best_move);
    } else {

        int best_value = std::numeric_limits<int>::max();
        Move best_move;

        for (int i = 1; i <= 6; i++) {
            Node child_node = {node.state.simulateMove((Move) i), (Move) i};

            int value;

            // Tener que maximizar o minimizar el siguiente nodo del árbol
            // dependerá de si vuelve a ser el turno del bot.
            if (child_node.state.getCurrentPlayer() == node.state.getCurrentPlayer()) {
                value = AlphaBeta(child_node, depth - 1, alpha, beta, false).first;
            } else {
                value = AlphaBeta(child_node, depth - 1, alpha, beta, true).first;
            }


            if (value < best_value) {
                best_value = value;
                best_move = (Move) i;
            }

            beta = std::min(beta, best_value);

            if (beta <= alpha) {
                break;
            }
        }

        return std::make_pair(best_value, best_move);
    }

}

Move Croqueta::nextMove(const vector<Move>& adversary, const GameState& state)
{

    int depth = 12;

    Player turno = this->getPlayer();
    long timeout = this->getTimeOut();

    Node origin = {state, M_NONE};
    auto path = AlphaBeta(origin, depth, std::numeric_limits<int>::min(),
                          std::numeric_limits<int>::max(), true);

    return path.second;
}
