//
// Created by Philip on 05/07/2021.
//

#include "Tablero.h"

namespace tablero {

    Casilla::Casilla()
    {
        int NumeroMin =  1;
        int NumeroMax =  9;

    }


    void Tablero::calcDatosFila()
    {
        int f = 0; // #fila
        int c = 0; // #columna
        for (int i = 0; i <= ladoLength; i++) // Recorre la matriz fila a fila de izquierda a derecha
        {
            if (c == ladoLength && f < ladoLength) // Salta a la siguiente fila siempre que haya una
            {
                c = -1;
                ++f;
            }
        }
    }

    void Tablero::calcDatosColumna()
    {
        int f = 0; // #fila
        int c = 0; // #columna
        for (; f <= ladoLength; ++f) // Recorre la matriz columna a columna de arriba a abajo
        {

            if (f == ladoLength &&
                c < ladoLength) // Salta a la siguiente columna siempre que haya una
            {
                f = -1;
                ++c;
            }
        }
    }

};