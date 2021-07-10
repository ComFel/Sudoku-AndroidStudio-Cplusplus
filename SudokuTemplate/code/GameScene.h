/*
 * INTRO SCENE
 * Copyright © 2021+ Felipe Vallejo Molina
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * felipevm07@gmail.com
 */

#include <memory>
#include <basics/Scene>
#include <basics/Texture_2D>
#include <basics/Timer>
#include <basics/Canvas>
#include <basics/Id>
#include <map>
#include <list>
#include <basics/Raster_Font>

#include "Sprite.hpp"


using namespace basics;
using namespace std;
using namespace example;



namespace Game
{
    using basics::Timer;

    class GameScene : public basics::Scene
    {
        // Estos typedefs pueden ayudar a hacer el código más compacto y claro:
        typedef std::shared_ptr< basics::Texture_2D > Texture_Handle;
        typedef shared_ptr < Sprite             >  Sprite_Handle;
        typedef list< Sprite_Handle             >  Sprite_List;
        typedef map< Id, Texture_Handle         >  Texture_Map;
        typedef basics::Graphics_Context::Accessor Context;
        typedef std::unique_ptr< basics::Raster_Font > Font_Handle;

    public:

        enum State
        {
            LOADING,
            RUNNING,
            PAUSE,
            WIN,
            ERROR
        };

        State          state;
        bool           suspended;
        bool           gamePaused;

        unsigned       canvas_width;
        unsigned       canvas_height;

        Timer timer;
        Canvas *canvasRef;

        //Definición de constantes
        static constexpr unsigned N = 9;
        static constexpr unsigned EmptyId = 0;


        float x,y;                                          ///Origen del tablero


        float botX;                                         ///Origen Coordenadas Botones
        float botY;

        float posX,posY;                                    ///Coordenadas para los inputs del jugador, separadas en ejes
        Point2f posFinal;

        int selectedValue;                                  ///Valor Númerico que tiene selecionado el jugador
        int correctCells;                                  ///Valor Númerico que indica las celdas correctas

        /**
         * Clase Box o Celda, crea las celdas del tablero
         * y para los "botones" de los numeros
         */
        struct Box{
            Texture_Handle textureCell;
            float x;                            ///Posición en eje X de la celda
            float y;                            ///Posición en eje Y de la celda
            float width = 40;                   ///Anchura de la celda
            float height = 130;                 ///Altura en eje X de la celda
            int boxvalue = 0;                   ///Valor numérico de la celda, con el que se realizan las comprobaciones para la victoria

            ///Función que controla los valores de las celdas, para por debajo, hacer todas las comprobaciones numericas para que el juego cumpla las reglas y condiciones de victoria
            void set_value(int value, std::vector<Texture_Handle> &texturesBox){
                boxvalue = value;
                textureCell = texturesBox[boxvalue];
            }

            ///Función propia para dibujar las texturas de las celdas
            void render(basics::Canvas &canvas){
                if(textureCell)
                    canvas.fill_rectangle({x,y},{width,height},textureCell.get());
            }

            ///Función para comprobar si el usuario a pulsado sobre un espacio de una celda
            bool contains(float pointX,float pointY){ return pointX> x && pointY>y && pointX<x+width &&pointY<y+height;}
        };

        std::vector<Texture_Handle> textures;                               ///Lista de texturas

        Texture_Handle PauseButton;                                         ///Textura para pausar y volver al menu
        Texture_Handle PauseBg;                                         ///Textura fondo pausa y volver al menu
        Font_Handle font;

        Box sudokuboard[N][N];                        ///Array de 9x9 que almacena los valores numéricos del juego

        ///Nivel 1 Sudoku
        int level[N][N] = {
                {0, 0, 0, 2, 6, 0, 7, 0, 1},
                {6, 8, 0, 0, 7, 0, 0, 9, 0},
                {1, 9, 0, 0, 0, 4, 5, 0, 0},

                {8, 2, 0, 1, 0, 0, 0, 4, 0},
                {0, 0, 4, 6, 0, 2, 9, 0, 0},
                {0, 5, 0, 0, 0, 3, 0, 2, 8},

                {0, 0, 9, 3, 0, 0, 0, 7, 4},
                {0, 4, 0, 0, 5, 0, 0, 3, 6},
                {7, 0, 3, 0, 1, 8, 0, 0, 0}
        };

        ///Solucion al Nivel 1
        int lvlSol[N][N] = {
                {4, 3, 5, 2, 6, 9, 7, 8, 1},
                {6, 8, 2, 5, 7, 1, 4, 9, 3},
                {1, 9, 7, 8, 3, 4, 5, 6, 2},

                {8, 2, 6, 1, 9, 5, 3, 4, 7},
                {3, 7, 4, 6, 8, 2, 9, 1, 5},
                {9, 5, 1, 7, 4, 3, 6, 2, 8},

                {5, 1, 9, 3, 2, 6, 8, 7, 4},
                {2, 4, 8, 9, 5, 7, 1, 3, 6},
                {7, 6, 3, 4, 1, 8, 2, 5, 9}

        };

        //Array que almacena las opciones que puede seleccionar el usuario
        Box sudokuNumbers[10];


    public:

        GameScene();

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

        bool initialize () override;
        void suspend    () override;
        void resume     () override;

        void handle     (basics::Event & event) override;
        void update     (float time) override;
        void render     (basics::Graphics_Context::Accessor & context) override;

    private:

        /**
          * En este método se cargan las texturas (una cada fotograma para facilitar que la
          * propia carga se pueda pausar cuando la aplicación pasa a segundo plano).
          */
        void loadTextures();

        ///Método que renderiza las texturas para el tablero
        void drawCells(basics::Canvas &canvas);

        ///Método que dibuja los botones numéricos que el jugador usa
        void drawNumbers(basics::Canvas &canvas);

        ///Método encargado de pintar información adicional para el jugador (boton de pausa,tiempo)
        void drawGameInfo(basics::Canvas &canvas);

        /**
          * En este método se detecta si el jugador ha presionado en algun lugar del tablero
          */
        Box* checkBoardSelection(Point2f &posToCheck);

        /**
          * En este método se detecta si el jugador ha presionado en algun lugar de los botones
          */
        Box* checkColumnSelection(Point2f &posSelect);

        /**
          * Este método comprueba si el jugador ha ganado
          */
        bool checkEndGame();

        //Funciones auxiliares para comprobar el correcto uso de las mecánicas o reglas del juego
        bool checkColum(int c, int value);
        bool checkRow(int r, int value);
        bool checkSquare(int row, int column, int value);


    };

}

