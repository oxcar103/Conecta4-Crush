#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);

      double Valoracion(const Environment &estado, int jugador);
      double MiniMax(const Environment & tablero, int jugador, int profundidad, const int MAX_PROFUNDIDAD, Environment::ActionType & accion);
      double Poda_AlfaBeta(const Environment & tablero, int jugador, int profundidad, const int MAX_PROFUNDIDAD, Environment::ActionType & accion, double alpha, double beta);
    private:
      int jugador_;
      Environment actual_;

      // Estas funciones comprueban si a partir de la casilla indicada, hay suficiente hueco para
      // que el jugador que indicamos haga 4 en raya en posición vertical, horizontal, diagonal
      // ascendente y descendente, respectivamente.
      bool HayHuecoVertical(const Environment &estado, int jugador, int row, int col);
      bool HayHuecoHorizontal(const Environment &estado, int jugador, int row, int col);
      bool HayHuecoDiagonalAscendente(const Environment &estado, int jugador, int row, int col);
      bool HayHuecoDiagonalDescendente(const Environment &estado, int jugador, int row, int col);


      // Estas funciones, supuesto que hay hueco, cuántas fichas del jugador que indicamos hay ya
      // colocadas en posición vertical, horizontal, diagonal, respectivamente.
      int ContarVertical(const Environment &estado, int jugador, int row, int col);
      int ContarHorizontal(const Environment &estado, int jugador, int row, int col);
      int ContarDiagonalAscendente(const Environment &estado, int jugador, int row, int col);
      int ContarDiagonalDescendente(const Environment &estado, int jugador, int row, int col);


      // Estas funciones, a partir de cuántas fichas del jugador que indicamos hay ya colocadas,
      // le asignan la puntuación en posición vertical, horizontal, diagonal, respectivamente.
      int PuntosVertical(const Environment &estado, int jugador, int row, int col);
      int PuntosHorizontal(const Environment &estado, int jugador, int row, int col);
      int PuntosDiagonal(const Environment &estado, int jugador, int row, int col);

      int PuntosCasilla(const Environment &estado, int jugador, int row, int col);

      inline int VerCasilla(const Environment &estado, int row, int col){
          return static_cast< int > (estado.See_Casilla(row, col));
      };
};
#endif
