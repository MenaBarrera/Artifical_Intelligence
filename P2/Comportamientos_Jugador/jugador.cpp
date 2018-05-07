#include "../Comportamientos_Jugador/jugador.hpp"
#include <queue>
#include <list>
#include "motorlib/util.h"
#include <iostream>
#include <set>
#include <cmath>

using namespace std;

void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}

/**          _
     /\   /\| |/\
    /  \  \ ` ' /
   / /\ \|_     _|
  / ____ \/ , . \
 /_/    \_\/|_|\/


En mi caso para la implementacion del A* he utilizado basicamente dos estructuras
de datos: priority_queue y set.  La cola con prioridad almacena la lista de abiertos
y en el set tenemos los estados que ya hemos explorado, usando el set nos
evitamos duplicarlos. En la cola con prioridad hemos metido objetos del struct
Movimiento que acmacena la posicion, plan y la f (para la heuristica).
Exploramos los nodos (avanzo. izquierda y derecha) se insertan en abiertos
y pasamos a cerrados el now. Si posicion == destino quiere decir que hemos
encontrado un plan por lo que devolvemos true, si por tanto no encontrase plan
se devolveria false.

/**/


/*
ALGORITMO:

ABIERTOS := [INICIAL] //inicialización
CERRADOS := []
f'(INICIAL) := h'(INICIAL)
repetir
si ABIERTOS = [] entonces FALLO
si no // quedan nodos
extraer MEJORNODO de ABIERTOS con f' mí­nima
// cola de prioridad
mover MEJORNODO de ABIERTOS a CERRADOS
si MEJORNODO contiene estado_objetivo entonces
SOLUCION_ENCONTRADA := TRUE
si no
generar SUCESORES de MEJORNODO
para cada SUCESOR hacer TRATAR_SUCESOR
hasta SOLUCION_ENCONTRADA o FALLO

*/
bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
	Mov now;
	Mov next_mov;

	//estructuras de datos
	priority_queue<Mov> movAbiertos;
	set<estado> explorados;
	now.posACT = origen;
	plan.clear();

	// aqui estamos basicamente metiendo el nodo padre de todos
	movAbiertos.push(now);

	while(!movAbiertos.empty() ){
		//sacamos el mov mas prioritario
		now = movAbiertos.top();
		// y lo quitamos de la cola
		movAbiertos.pop();

		//Avanzar
		next_mov = now;
		next_mov.posACT = avanza(next_mov.posACT);

		// Si no esta en explorados y puedo avanzar meto la accion en el plan y calculo la heuristica
		if(explorados.find(next_mov.posACT) == explorados.end()){
			if(esPosibleAvanzar(next_mov.posACT)){

				next_mov.plan.push_back(actFORWARD);
				next_mov.f = distancia_en_manhattan(next_mov.posACT,destino) + next_mov.plan.size();

				//si es detino he encontrado un plan fenomeno para llegar
				if((next_mov.posACT.fila == destino.fila) && (next_mov.posACT.columna == destino.columna)){
					cout<<" Ya tengo un plan!!"<<endl;
					//copiamos el plan al plan general y devolvemos true para salir del bucle
					plan=next_mov.plan;
					return true;
				}
				//lo metemos en la cola
				//para explorarlo mas adelante
				movAbiertos.push(next_mov);
			}
		}
		//Izquierda
		explorar_izquierda(next_mov, now, explorados,movAbiertos);
		//Derecha
		explorar_derecha(next_mov, now, explorados,movAbiertos);
		//Insercion en el conjunto de nodos cerrados
		explorados.insert(now.posACT);
	}
	return false;
}


/**/
//  ______                          _    _
// |  ____|         /\             (_)_ (_)
// | |__           /  \  _   ___  ___| |_  __ _ _ __ ___  ___
// |  __|         / /\ \| | | \ \/ / | | |/ _` | '__/ _ \/ __|
// | |       _   / ____ \ |_| |>  <| | | | (_| | | |  __/\_\_
// |_|      (_) /_/    \_\__,_/_/\_\_|_|_|\__,_|_|  \___||___/
//
/**/

/*

	Explora el nodo que genera girar a la izquierda y avanzar
	Despues lo meten en cerrados (o movAbiertos)
	Todo es por referencia por lo que las modificaciones pòr supuesto se
	mantienen en las Variables

*/

void ComportamientoJugador::explorar_izquierda(Mov &next_mov,Mov &now, set<estado> &explorados, priority_queue<Mov> &movAbiertos){
	next_mov = now;//Hacemos una copia
	next_mov.posACT = turnR(next_mov.posACT);

	if(explorados.find(next_mov.posACT)==explorados.end()){
		next_mov.plan.push_back(actTURN_L);
		next_mov.f = distancia_en_manhattan(next_mov.posACT,destino)+ next_mov.plan.size();


		movAbiertos.push(next_mov);
	}
}

/*

	Explora el nodo que genera girar a la derecha y avanzar
	Despues lo meten en cerrados (o movAbiertos)
	Todo es por referencia por lo que las modificaciones pòr supuesto se
	mantienen en las Variables

*/

void ComportamientoJugador::explorar_derecha(Mov &next_mov,Mov &now, set<estado> &explorados, priority_queue<Mov> &movAbiertos){
	next_mov = now;
	next_mov.posACT = turnL(next_mov.posACT);

	if(explorados.find(next_mov.posACT) == explorados.end()){
		next_mov.plan.push_back(actTURN_R);
		next_mov.f = distancia_en_manhattan(next_mov.posACT,destino)+ next_mov.plan.size();


		movAbiertos.push(next_mov);
	}
}

/*
	Funcion auxiliar que basicamente hace avanzar al jugador
	 - si mira al norte decrementa fila
	 - si mira al este incrementa la columna
	 - si mira al sur incrementa la fila
	 - si mira al oeste decrementa la columna

	 y devuelve un nuevo estado con la posicion avanzada (esto lo usamos para esPosibleAvanzar)
*/

estado ComportamientoJugador::avanza(estado position){
	estado pos_ret;

	if(position.orientacion==0){ // NORTH
		pos_ret.fila = position.fila - 1;
		pos_ret.columna = position.columna;

	}
	else if(position.orientacion==1){ // EAST
		pos_ret.fila =  position.fila;
		pos_ret.columna = position.columna +1;
	}
	else if(position.orientacion==2){	// SOUTH
		pos_ret.fila = position.fila+1;
		pos_ret.columna = position.columna;
	}
	else if(position.orientacion==3){	// WEST
		pos_ret.fila = position.fila;
		pos_ret.columna = position.columna-1;

	}
	pos_ret.orientacion = position.orientacion;

	return pos_ret;

}

/*
	Comprueba que es posible avanzar:
		es decir que se pise terreno admisible para el jugador
		que filas y columnas sean validas
*/

bool ComportamientoJugador::esPosibleAvanzar(estado position){
	bool ret = false;

	bool surface = (mapaResultado[position.fila][position.columna]=='T' || mapaResultado[position.fila][position.columna]=='S' || mapaResultado[position.fila][position.columna]=='K');
	bool fila_en_rango = (position.fila<mapaResultado.size())&&(position.fila>0);
	bool columna_en_rango = (position.columna<mapaResultado[0].size())&&(position.columna>0);
	bool limits = (fila_en_rango || columna_en_rango);

	if(surface && limits){
		ret = true;
	}
	return ret;
}

/*
	Se le pasa un estado y devuelve uno nuevo con la orientacion cambiada

*/

estado ComportamientoJugador::turnR(estado position){
	estado ret = position;

	ret.columna = position.columna;
	ret.orientacion =(ret.orientacion + 3)%4;
	ret.fila = position.fila;

	return ret;
}

/*
	Se le pasa un estado y devuelve uno nuevo con la orientacion cambiada

*/
estado ComportamientoJugador::turnL(estado position){
	estado ret = position;

	ret.columna = position.columna;
	ret.orientacion =(ret.orientacion + 1)%4;
	ret.fila = position.fila;

	return ret;
}

/*
	Da la distancia_en_manhattan para calcular la heuristica

*/
int ComportamientoJugador::distancia_en_manhattan(estado orig, estado destino){
	return (abs(orig.fila - destino.fila) + abs(orig.columna - destino.columna));
}

bool operator<(const Mov &movement1,const Mov &movement2){
	return movement1.f > movement2.f;
}

bool operator<(const estado &est1, const estado &est2){
	 if(est1.fila < est2.fila){
		return true;
	 }
	 if(est1.fila == est2.fila && est1.columna < est2.columna){
		return true;
	 }
	 if(est1.fila == est2.fila && est1.columna == est2.columna && est1.orientacion < est2.orientacion){
		return true;
	 }

	 return false;
}

/*

  ______ ____            _
 |  ____|  _ \          (_)
 | |__  | |_) | __ _ ___ _  ___ __ _ ___
 |  __| |  _ < / _` / __| |/ __/ _` / __|
 | |_   | |_) | (_| \__ \ | (_| (_| \__ \
 |_(_)  |____/ \__,_|___/_|\___\__,_|___/


*/


void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  	AnularMatriz(mapaConPlan);
	estado cst = st;


	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;

				case 1: cst.columna++; break;

				case 2: cst.fila++; break;

				case 3: cst.columna--; break;
			}

			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}

		it++;
	}
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

Action ComportamientoJugador::think(Sensores sensores) {

  if (sensores.mensajeF != -1){
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
	}


	switch (lastACT){
		case actTURN_R: brujula = (brujula+1)%4; break;
		case actTURN_L: brujula = (brujula+3)%4; break;
		case actFORWARD:
			switch (brujula){
				case 0: fil--; break;

				case 1: col++; break;

				case 2: fil++; break;

				case 3: col--; break;
			}
	}
	now.fila = fil;

	now.columna = col;

	now.orientacion = brujula;
	PintaPlan(plan);
	VisualizaPlan(now,plan);


	// Determinar si ha cambiado el destino desde la ultima planificacion
	if (tengoPlan and (sensores.destinoF != destino.fila or sensores.destinoC != destino.columna)){
		cout << "El destino ha cambiado\n";
		tengoPlan = false;
	}



	// Determinar si tengo que construir un plan
	if (!tengoPlan){
		//estado origen;
		now.fila = fil;
		now.columna = col;
		now.orientacion = brujula;

		destino.fila = sensores.destinoF;
		destino.columna = sensores.destinoC;

		tengoPlan = pathFinding(now,destino,plan);
		VisualizaPlan(now,plan);
		PintaPlan(plan);
	}
	cout << endl;
	PintaPlan(plan);
	cout << endl;

	// Ejecutar el plan
	Action nextACT;

	if(tengoPlan && plan.size() > 0){

		nextACT = plan.front();

		if( sensores.terreno[0]=='K'){
			cout << "Fila_PK_ -> " << fil <<endl;
			cout << "col_PK_ -> " << col <<endl;
			fil = sensores.mensajeF;
			col = sensores.mensajeC;
			cout << "Fila_PK_tras -> " << fil <<endl;
			cout << "col_PK_tras -> " << col <<endl;
		}

		if(nextACT == actFORWARD && sensores.superficie[2] == 'a'){
			//now = avanza(now);
			cout<<"Tengo un aldeano asi que voy a esperar" << endl;
			nextACT = actIDLE;
		}
		else{
			plan.erase(plan.begin());
		}
	}
	else{
		nextACT = actIDLE;

	}
	lastACT = nextACT;
	return nextACT;
}

void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}
