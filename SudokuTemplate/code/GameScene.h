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

    public:

        enum State
        {
            LOADING,
            RUNNING,
            PAUSE,
            ERROR
        };

        State          state;
        bool           suspended;

        unsigned       canvas_width;
        unsigned       canvas_height;

        Timer timer;


        static constexpr unsigned N = 9;
        static constexpr unsigned EmptyId = 0;

        //Origen del tablero
        float x,y;

        //Origen Botones
        float botX;
        float botY;

        float posX,posY;
        Point2f posInicial;
        Point2f posFinal;
        int valorSelect;


        struct Box{
            Texture_Handle textureCell;
            float x;
            float y;
            float width = 40;
            float height = 130;
            int boxvalue = 0;

            void set_value(int value, std::vector<Texture_Handle> &texturesBox){
                boxvalue = value;
                textureCell = texturesBox[value];
            }

            void render(basics::Canvas &canvas){
                if(textureCell)
                    canvas.fill_rectangle({x,y},{width,height},textureCell.get());
            }

            bool contains(float pointX,float pointY){ return pointX> x && pointY>y && pointX<x+width &&pointY<y+height;}
        };

        std::vector<Texture_Handle> textures;


        Texture_Handle BackButton;

        //Array de 9x9 que almacena los valores numéricos del juego
        Box sudokuboard[N][N];


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

        //Método que renderiza las texturas para el tablero
        void drawCells(basics::Canvas &canvas);

        //Método que dibuja los botones numéricos que el jugador usa
        void drawNumbers(basics::Canvas &canvas);

        //Método encargado de pintar información adicional para el jugador (boton de pausa,tiempo)
        void drawGameInfo(basics::Canvas &canvas);

        void drawSelection(int valor,Texture_Handle reTexture);
        int getCell(Point2f &posSelect);

        Box* checkCellValue(Point2f &posToCheck);

        bool checkColum(int c, int value);
        bool checkRow(int r, int value);
        bool checkSquare(int row, int column, int value);

        void CellChecks();


        /**
             * En este método se cargan las texturas (una cada fotograma para facilitar que la
             * propia carga se pueda pausar cuando la aplicación pasa a segundo plano).
             */
        void loadTextures();





    };

}

