#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;

const double MAX_INT=9999999999.0, MIN_INT=-9999999999.0;           // Valores m�ximos y m�nimos
const double UN_PUNTO = 1, DOS_PUNTOS = 50, TRES_PUNTOS = 1000;     // Valores para la ponderaci�n

// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}

// Funci�n de valoraci�n para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    // Gana el jugador que pide la valoraci�n
    if (ganador==jugador)
       return MAX_INT;
    // Pierde el jugador que pide la valoraci�n
    else if (ganador!=0)
            return MIN_INT;
    // Hay un empate global y se ha rellenado completamente el tablero
    else if (estado.Get_Casillas_Libres()==0)
            return 0;
    else
          return Puntuacion(jugador,estado);
}
// Los tres m�todos anteriores no se pueden modificar(para la pr�ctica)

bool Player::HayHuecoVertical(const Environment &estado, int jugador, int row, int col){
    bool hay_hueco = true;
    int valor_casilla;

    for(int i = row; i < row+4; i++){
        if(i >= 0 && i < 7){
            // As� agrupamos las fichas normales y la bomba del jugador en un mismo n�mero
            valor_casilla = VerCasilla(estado, i, col)%3;
            hay_hueco = hay_hueco && (valor_casilla == 0 || valor_casilla == jugador);
        }
        else{
            hay_hueco = false;
        }
    }

    return hay_hueco;
}

bool Player::HayHuecoHorizontal(const Environment &estado, int jugador, int row, int col){
    bool hay_hueco = true;
    int valor_casilla;

    for(int i = col; i < col+4; i++){
        if(i >= 0 && i < 7){
            // As� agrupamos las fichas normales y la bomba del jugador en un mismo n�mero
            valor_casilla = VerCasilla(estado, row, i)%3;
            hay_hueco = hay_hueco && (valor_casilla == 0 || valor_casilla == jugador);
        }
        else{
            hay_hueco = false;
        }
    }

    return hay_hueco;
}

bool Player::HayHuecoDiagonalAscendente(const Environment &estado, int jugador, int row, int col){
    bool hay_hueco = true;
    int valor_casilla;

    for(int i = 0; i < 4; i++){
        if((row+i) >= 0 && (row+i) < 7 && (col+i) >= 0 && (col+i) < 7){
            // As� agrupamos las fichas normales y la bomba del jugador en un mismo n�mero
            valor_casilla = VerCasilla(estado, row+i, col+i)%3;
            hay_hueco = hay_hueco && (valor_casilla == 0 || valor_casilla == jugador);
        }
        else{
            hay_hueco = false;
        }
    }

    return hay_hueco;
}

bool Player::HayHuecoDiagonalDescendente(const Environment &estado, int jugador, int row, int col){
    bool hay_hueco = true;
    int valor_casilla;

    for(int i = 0; i < 4; i++){
        if((row-i) >= 0 && (row-i) < 7 && (col+i) >= 0 && (col+i) < 7){
            // As� agrupamos las fichas normales y la bomba del jugador en un mismo n�mero
            valor_casilla = VerCasilla(estado, row-i, col+i)%3;
            hay_hueco = hay_hueco && (valor_casilla == 0 || valor_casilla == jugador);
        }
        else{
            hay_hueco = false;
        }
    }

    return hay_hueco;
}

int Player::ContarVertical(const Environment &estado, int jugador, int row, int col){
    int valor_casilla, fichas;

    for(int i = row; i < row+4; i++){
        // As� agrupamos las fichas normales y la bomba del jugador en un mismo n�mero
        valor_casilla = VerCasilla(estado, i, col)%3;
        fichas += (valor_casilla == jugador)? 1: 0;
    }

    return fichas;
}

int Player::ContarHorizontal(const Environment &estado, int jugador, int row, int col){
    int valor_casilla, fichas;

    for(int i = col; i < col+4; i++){
        // As� agrupamos las fichas normales y la bomba del jugador en un mismo n�mero
        valor_casilla = VerCasilla(estado, row, i)%3;
        fichas += (valor_casilla == jugador)? 1: 0;
    }

    return fichas;
}

int Player::ContarDiagonalAscendente(const Environment &estado, int jugador, int row, int col){
    int valor_casilla, fichas;

    for(int i = 0; i < 4; i++){
        // As� agrupamos las fichas normales y la bomba del jugador en un mismo n�mero
        valor_casilla = VerCasilla(estado, row+i, col+i)%3;
        fichas += (valor_casilla == jugador)? 1: 0;
    }

    return fichas;
}

int Player::ContarDiagonalDescendente(const Environment &estado, int jugador, int row, int col){
    int valor_casilla, fichas;

    for(int i = 0; i < 4; i++){
        // As� agrupamos las fichas normales y la bomba del jugador en un mismo n�mero
        valor_casilla = VerCasilla(estado, row-i, col+i)%3;
        fichas += (valor_casilla == jugador)? 1: 0;
    }

    return fichas;
}

// Esta funci�n est� pensada para ser llamada desde cada una de las fichas del tablero, por ello, los �ndices
// del bucle van desde 0 hasta 4(no incluido), para asegurarnos que est� contenida al menos una ficha
int Player::PuntosVertical(const Environment &estado, int jugador, int row, int col) {
    int puntuacion_vertical = 0;


    for (int i=0; i<4; i++) {
        if (HayHuecoVertical(estado, jugador, row-i, col)){
            switch(ContarVertical(estado, jugador, row-i, col)){
                case 1: puntuacion_vertical += UN_PUNTO;
                break;
                case 2: puntuacion_vertical += DOS_PUNTOS;
                break;
                case 3: puntuacion_vertical += TRES_PUNTOS;
                break;
                case 4: puntuacion_vertical += MAX_INT;
                break;
            }
        }
    }
    
    return puntuacion_vertical;
}

// Esta funci�n est� pensada para ser llamada desde cada una de las fichas del tablero, por ello, los �ndices
// del bucle van desde 0 hasta 4(no incluido), para asegurarnos que est� contenida al menos una ficha
int Player::PuntosHorizontal(const Environment &estado, int jugador, int row, int col){
    int puntuacion_horizontal = 0;

    for (int i=0; i<4; i++) {
        if (HayHuecoHorizontal(estado, jugador, row, col-i)){
            switch(ContarHorizontal(estado, jugador, row, col-i)){
                case 1: puntuacion_horizontal += UN_PUNTO;
                break;
                case 2: puntuacion_horizontal += DOS_PUNTOS;
                break;
                case 3: puntuacion_horizontal += TRES_PUNTOS;
                break;
                case 4: puntuacion_horizontal += MAX_INT;
                break;
            }
        }
    }

    return puntuacion_horizontal;
}

// Esta funci�n est� pensada para ser llamada desde cada una de las fichas del tablero, por ello, los �ndices
// del bucle van desde 0 hasta 4(no incluido), para asegurarnos que est� contenida al menos una ficha
int Player::PuntosDiagonal(const Environment &estado, int jugador, int row, int col){
    int puntuacion_diagonal = 0;

    for (int i=0; i<4; i++) {
        if (HayHuecoDiagonalAscendente(estado, jugador, row-i, col-i)){
            switch(ContarDiagonalAscendente(estado, jugador, row-i, col-i)){
                case 1: puntuacion_diagonal += UN_PUNTO;
                break;
                case 2: puntuacion_diagonal += DOS_PUNTOS;
                break;
                case 3: puntuacion_diagonal += TRES_PUNTOS;
                break;
                case 4: puntuacion_diagonal += MAX_INT;
                break;
            }
        }
        if (HayHuecoDiagonalDescendente(estado, jugador, row+i, col-i)){
            switch(ContarDiagonalDescendente(estado, jugador, row+i, col-i)){
                case 1: puntuacion_diagonal += UN_PUNTO;
                break;
                case 2: puntuacion_diagonal += DOS_PUNTOS;
                break;
                case 3: puntuacion_diagonal += TRES_PUNTOS;
                break;
                case 4: puntuacion_diagonal += MAX_INT;
                break;
            }
        }
    }

    return puntuacion_diagonal;
}


// Esta funci�n, cuenta los puntos del tablero actual y el jugador indicado que afectan a la
// casilla especificada.
int Player::PuntosCasilla(const Environment &estado, int jugador, int row, int col){
    return PuntosVertical(estado, jugador, row, col) + PuntosHorizontal(estado, jugador, row, col) + PuntosDiagonal(estado, jugador, row, col);
}

// Funcion heur�stica
double Player::Valoracion(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    // Gana el jugador que pide la valoraci�n
    if (ganador==jugador)
        return MAX_INT;
    // Pierde el jugador que pide la valoraci�n
    else if (ganador!=0)
        return MIN_INT;
    // Hay un empate global y se ha rellenado completamente el tablero
    else if (estado.Get_Casillas_Libres()==0)
        return 0;
    else{
        //Si no he ganado ni perdido ni empatado aplicamos la heur�stica
        int valor_casilla;
        double puntuacion = 0;

        int adversario = jugador == 1? 2: 1;

        for(int i = 0; i < 7; i++){
            for(int j = 0; j < 7; j++){
                valor_casilla = VerCasilla(estado, i, j)%3; // As� agrupamos las fichas normales y la bomba del jugador en un mismo n�mero

                // La puntuaci�n de un tablero es igual al n�mero de tus puntos menos los de tu adversario
                if(valor_casilla == jugador){
                    puntuacion += PuntosCasilla(estado, jugador, i, j);
                }
                else if(valor_casilla == adversario){
                    puntuacion -= PuntosCasilla(estado, adversario, i, j);
                }
            }
        }

        return puntuacion;
    }
}

// Esta funci�n, permite ilustrar el comportamiento del juego, no implementa ni MiniMax ni poda AlfaBeta.
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}

// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral m�ximo de profundidad para el m�todo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral m�ximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acci�n que se va a devolver
    bool aplicables[8]; /* Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretaci�n es:
                           aplicables[0]==true si PUT1 es aplicable
                           aplicables[1]==true si PUT2 es aplicable
                           aplicables[2]==true si PUT3 es aplicable
                           aplicables[3]==true si PUT4 es aplicable
                           aplicables[4]==true si PUT5 es aplicable
                           aplicables[5]==true si PUT6 es aplicable
                           aplicables[6]==true si PUT7 es aplicable
                           aplicables[7]==true si BOOM es aplicable
                        */

    // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double valor;

    //Acciones posibles en el estado actual
    int n_act;

    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;

    // Descomentar uno de los 3:

    // Opci�n: Juego Aleatorio
    /*cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }

    */

    // Opci�n: M�todo MiniMax
    /*valor = MiniMax(actual_, jugador_, 0, PROFUNDIDAD_MINIMAX, accion);
    cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;
    */

    // Opci�n: Poda AlfaBeta
    valor = Poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, MIN_INT-1, MAX_INT+1);
    cout << "Valor AlfaBeta: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

    return accion;
}

// M�todo MiniMax
double Player::MiniMax(const Environment & tablero, int jugador, int profundidad, const int MAX_PROFUNDIDAD, Environment::ActionType & accion){
    if (profundidad == MAX_PROFUNDIDAD || tablero.JuegoTerminado()){
        return Valoracion(tablero, jugador);
    }
    else{
        int ultima_accion = -1;
        double mejor, candidato;
        Environment::ActionType accion_anterior;        

        if(profundidad%2 == 0){
            mejor = MIN_INT;

            for(Environment hijo = tablero.GenerateNextMove(ultima_accion); !(hijo == tablero); hijo = tablero.GenerateNextMove(ultima_accion)){
                candidato = MiniMax(hijo, jugador, profundidad+1, MAX_PROFUNDIDAD, accion_anterior);

                if (candidato > mejor){
                    mejor = candidato;
                    accion = static_cast <Environment::ActionType > (ultima_accion);
                }
            }
        }
        else{
            mejor = MAX_INT;

            for(Environment hijo = tablero.GenerateNextMove(ultima_accion); !(hijo == tablero); hijo = tablero.GenerateNextMove(ultima_accion)){
                candidato = MiniMax(hijo, jugador, profundidad+1, MAX_PROFUNDIDAD, accion_anterior);

                if (candidato < mejor){
                    mejor = candidato;
                    accion = static_cast <Environment::ActionType > (ultima_accion);
                }
            }
        }

        return mejor;
    }
}

// Poda AlfaBeta
double Player::Poda_AlfaBeta(const Environment & tablero, int jugador, int profundidad, const int MAX_PROFUNDIDAD, Environment::ActionType & accion, double alpha, double beta){
    if (profundidad == MAX_PROFUNDIDAD || tablero.JuegoTerminado()){
        return Valoracion(tablero, jugador);
    }
    else{
        int ultima_accion = -1;
        double candidato;
        Environment::ActionType accion_anterior;

        if(profundidad%2 == 0){
            for(Environment hijo = tablero.GenerateNextMove(ultima_accion); !(hijo == tablero); hijo = tablero.GenerateNextMove(ultima_accion)){
                candidato = Poda_AlfaBeta(hijo, jugador, profundidad+1, MAX_PROFUNDIDAD, accion_anterior, alpha, beta);

                if (candidato > alpha){
                    alpha = candidato;
                    accion = static_cast <Environment::ActionType > (ultima_accion);
                }                
                if(alpha >= beta){
                    break;
                }
            }

            return alpha;
        }
        else{
            for(Environment hijo = tablero.GenerateNextMove(ultima_accion); !(hijo == tablero); hijo = tablero.GenerateNextMove(ultima_accion)){
                candidato = Poda_AlfaBeta(hijo, jugador, profundidad+1, MAX_PROFUNDIDAD, accion_anterior, alpha, beta);

                if (candidato < beta){
                    beta = candidato;
                    accion = static_cast <Environment::ActionType > (ultima_accion);
                }
                if(alpha >= beta){
                    break;
                }
            }

            return beta;
        }
    }
}

