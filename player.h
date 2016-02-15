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

      bool HayHuecoVertical(const Environment &estado, int jugador, int row, int col);
      bool HayHuecoHorizontal(const Environment &estado, int jugador, int row, int col);
      bool HayHuecoDiagonalAscendente(const Environment &estado, int jugador, int row, int col);
      bool HayHuecoDiagonalDescendente(const Environment &estado, int jugador, int row, int col);

      int ContarVertical(const Environment &estado, int jugador, int row, int col);
      int ContarHorizontal(const Environment &estado, int jugador, int row, int col);
      int ContarDiagonalAscendente(const Environment &estado, int jugador, int row, int col);
      int ContarDiagonalDescendente(const Environment &estado, int jugador, int row, int col);

      int PuntosVertical(const Environment &estado, int jugador, int row, int col);
      int PuntosHorizontal(const Environment &estado, int jugador, int row, int col);
      int PuntosDiagonal(const Environment &estado, int jugador, int row, int col);

      int PuntosCasilla(const Environment &estado, int jugador, int row, int col);

      inline int VerCasilla(const Environment &estado, int row, int col){
          return static_cast< int > (estado.See_Casilla(row, col));
      };
};
#endif
