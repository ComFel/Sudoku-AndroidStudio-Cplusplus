//
// Created by Philip on 05/07/2021.
//

#include <cstdlib>

namespace tablero {

    class Casilla
    {
    private:

        int posX = 0;                           // Coordenada X
        int posY = 0;                           // Coordenada Y


    public:

        /**
          * Constructor por defecto, asigna valores random a valorBomba y valorMultp
          */
        Casilla();

        int getX() { return posX; }
        int getY() { return posY; }

        void setX(int x) { posX = x; }
        void setY(int y) { posY = y; }

    };

    class Tablero
    {
    private:
        int ladoLength = 10;
        int height = 0;
        int width  = 0;

    public:
        Casilla matrizTablero[9][9];                // Tablero de 81 Casillas

        Tablero()
        {
            calcDatosFila();
            calcDatosColumna();
        };


    private:


        //Rellena el array puntos fila con el número de puntos por cada fila (nºfila = pos en el array)
        void calcDatosFila();

        /**
          * Rellena el array bombas columna con el número de bombas por cada columna (nºcolumna = pos en el array)
          * Rellena el array puntos columna con el número de puntos por cada columna (nºcolumna = pos en el array)
          */
        void calcDatosColumna();


    };
}
