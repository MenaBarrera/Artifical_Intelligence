#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <iostream>
#include <cmath>
#include <queue>
#include <list>
#include <set>
#include <algorithm>

struct estado {
  int fila;
  int columna;
  int orientacion;
};

struct Mov{

  estado posACT;
  list<Action> plan;
  int f=0;

};


class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      lastACT = actIDLE;
      tengoPlan = false;


    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      lastACT = actIDLE;
      tengoPlan = false;

    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}
  private:
    int fil, col, brujula;
    estado destino;
    list<Action> plan;
    estado now;
    bool tengoPlan;
    Action lastACT;

    void explorar_derecha(Mov &next_mov,Mov &now, set<estado> &explorados, priority_queue<Mov> &movAbiertos);
    void explorar_izquierda(Mov &next_mov,Mov &now, set<estado> &explorados, priority_queue<Mov> &movAbiertos);
    bool pathFinding(const estado &, const estado &, list<Action> &);
    void PintaPlan(list<Action>);
    estado avanza(estado position);
    bool esPosibleAvanzar(estado position);
    estado turnR(estado position);
    estado turnL(estado position);
    int distancia_en_manhattan(estado origen, estado destino);

};

#endif
