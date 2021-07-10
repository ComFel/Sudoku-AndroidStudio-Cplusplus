/*
 * INTRO SCENE
 * Copyright © 2021+ Felipe Vallejo Molina
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * felipevm07@gmail.com
 */

#include "GameScene.h"
#include "Menu_Scene.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Transformation>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "EndGameScene.h"

using namespace basics;
using namespace std;
using namespace example;

namespace Game
{

    GameScene::GameScene()
    {
        canvas_width = 1280;
        canvas_height = 720;
        suspended = false;
    }

    bool GameScene::initialize()
    {
        state = LOADING;
        x = 10;
        y = 10;
        botX = 300;
        botY = 100;
        selectedValue = 0;
        correctCells = 0;
        gamePaused = false;
        return true;
    }

    void GameScene::suspend()
    {
        suspended = true;
    }

    void GameScene::resume()
    {
        suspended = false;
    }

    void GameScene::handle (Event & event)
    {
        if (state == RUNNING)  // Se descartan los eventos cuando la escena no está en RUNNING
        {
            switch (event.id)
            {
                case ID(touch-started):         // El usuario toca la pantalla
                case ID(touch-moved):
                case ID(touch-ended):  // El usuario deja de tocar la pantalla
                {
                    posX = *event[ID(x)].as< var::Float > ();
                    posY = *event[ID(y)].as< var::Float > ();
                    posFinal = {posX, posY};  // Guarda el último punto tocado

                    Box *touchBoardBox = checkBoardSelection(posFinal); //Referencia a la celda del tablero que el usuario selecciona

                    //Comprobamos que no sea null, y si ha pulsado en el tablero o en lo botones
                    if(touchBoardBox)
                    {
                        touchBoardBox->set_value(selectedValue,textures); //Actualiza el valor de la celda, con su correspondiente textura
                    }
                    else{
                        //Referencia para controlar que boton pulsa el usuario, para recoger el valor del mismo y pintarlo en el tablero
                        Box *touchColumn = checkColumnSelection(posFinal);

                        if(touchColumn)
                        {
                            selectedValue = touchColumn->boxvalue;
                        }
                    }

                    //Comprobación de victoria del juego
                    checkEndGame();

                    //Boton Back
                    if (posFinal.coordinates.x() > 0 && posFinal.coordinates.y()>0 && posFinal.coordinates.x() < 70 && posFinal.coordinates.y()<200)
                    {
                        //state = PAUSE;
                        //gamePaused = true;
                        director.run_scene(shared_ptr<Scene>(new Menu_Scene));
                    }

                    break;
                }
            }
        }
    }

    void GameScene::update(float time)
    {
        switch (state)
        {
            case LOADING: loadTextures(); break;
            case RUNNING: checkEndGame(); break;
            case WIN: break;
            case PAUSE: break;
            case ERROR: break;

        }
    }

    void GameScene::render(basics::Graphics_Context::Accessor &context)
    {
        if (!suspended)
        {
            // El canvas se puede haber creado previamente, en cuyo caso solo hay que pedirlo:

            Canvas * canvas = context->get_renderer<Canvas>(ID(canvas));

            // Si no se ha creado previamente, hay que crearlo una vez:

            if (!canvas)
            {
                canvas = Canvas::create(ID(canvas), context, {{canvas_width, canvas_height}});
            }

            if (canvas)
            {
                canvas->clear();


                if (state == PAUSE)
                {
                    //Actualiza el canvas durante la pausa
                    drawGameInfo(*canvas);
                }
                else
                {
                    //Creamos los elementos visuales de la escena
                    drawGameInfo(*canvas);
                    drawCells(*canvas);
                    drawNumbers(*canvas);
                }
            }
        }
    }

    void GameScene::loadTextures()
    {
        if (!suspended)
        {
            //Creación del contexto gráfico
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                //Forma de crear las texturas de la escena, se las añade a una lista
                textures.push_back(Texture_2D::create(0,context,"SpritesGame/Empty.png"));      //0, Para poder reemplazar las texturas por id y asiganarse correspondientemente
                context->add(textures.back());
                textures.push_back(Texture_2D::create(0,context,"SpritesGame/Number1.png"));
                context->add(textures.back());
                textures.push_back(Texture_2D::create(0,context,"SpritesGame/Number2.png"));
                context->add(textures.back());
                textures.push_back(Texture_2D::create(0,context,"SpritesGame/Number3.png"));
                context->add(textures.back());
                textures.push_back(Texture_2D::create(0,context,"SpritesGame/Number4.png"));
                context->add(textures.back());
                textures.push_back(Texture_2D::create(0,context,"SpritesGame/Number5.png"));
                context->add(textures.back());
                textures.push_back(Texture_2D::create(0,context,"SpritesGame/Number6.png"));
                context->add(textures.back());
                textures.push_back(Texture_2D::create(0,context,"SpritesGame/Number7.png"));
                context->add(textures.back());
                textures.push_back(Texture_2D::create(0,context,"SpritesGame/Number8.png"));
                context->add(textures.back());
                textures.push_back(Texture_2D::create(0,context,"SpritesGame/Number9.png"));
                context->add(textures.back());

                PauseButton = Texture_2D::create(0,context,"SpritesGame/back_arrow.png");
                context->add(PauseButton);

                PauseBg = Texture_2D::create(0,context,"SpritesGame/BgPause.png");
                context->add(PauseBg);

                font.reset (new Raster_Font("fonts/impact.fnt", context));

                //Creación de las texturas del tablero, que toma una matriz / nivel predefinido, a partir de una posición y con un tamaño predefinidos
                x = 100;
                y = 100;
                for (int i = 0; i < N; ++i)
                {
                    y = 100;
                    for (int j = 0; j < N; ++j) {
                        sudokuboard[i][j].x = x;
                        sudokuboard[i][j].y = y;
                        sudokuboard[i][j].set_value(level[i][j],textures);
                        y+= 130;
                    }
                    x+=40;
                }

                state = RUNNING;
            }
        }
    }

    //Se dibuja el tablero donde se colocan las fichas
    void GameScene::drawCells(basics::Canvas &canvas)
    {
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j) {
                sudokuboard[i][j].render(canvas);
            }

        }
    }

    //Crear los botones para que el usuario decida que valor poner/elegir
    void GameScene::drawNumbers(basics::Canvas &canvas)
    {
        botX = 600;
        botY = 50;
        for(int i = 0;i < 10;++i){
            sudokuNumbers[i].x = botX;
            sudokuNumbers[i].y = botY;
            sudokuNumbers[i].set_value(i,textures);
            botY +=130;
        }

        //Una vez cargado las texturas y valores de los botones, se crean en el canvas
        for (int i = 0; i < 10; ++i) {
            sudokuNumbers[i].render(canvas);
        }

    }

    ///Creación de los elementos de UI (boton, tiempo, texto)
    void GameScene::drawGameInfo(basics::Canvas &canvas)
    {
        if(!suspended){
            x = 25;
            y= 100;
            canvas.fill_rectangle({ x, y }, { 50,150 }, PauseButton.get());
        }
        if(gamePaused){ ///Si el juego esta pausado
            x = 25;
            y= 100;
            canvas.fill_rectangle({ x, y }, { 50,150 }, PauseButton.get());
            if (font && PauseBg)
            {
                // Se dibujan textos con diferentes puntos de anclaje a partir de una cadena simple
                Text_Layout sample_text(*font, L"Pause");
                canvas.draw_text ({500, 500}, sample_text,    CENTER);
                canvas.fill_rectangle({ 500, 500 }, { 720,1820 }, PauseBg.get());
                gamePaused = false;
                state = RUNNING;
            }

        }

    }

    ///Comprobamos si el jugador ha tocado la pantalla en el tablero
    GameScene::Box* GameScene::checkBoardSelection(Point2f &posToCheck)
    {
        float posy = posToCheck.coordinates.y();
        float posx= posToCheck.coordinates.x();

        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j) {
                if(sudokuboard[i][j].contains(posx,posy))
                    return &sudokuboard[i][j]; ///Devuelve la referencia a la celda del tablero pulsada
            }
        }
        return nullptr;
    }

    ///Comprobamos si el jugador ha tocado la pantalla en los botones numéricos
    GameScene::Box* GameScene::checkColumnSelection(Point2f &posSelect) {
        float posy = posSelect.coordinates.y();
        float posx= posSelect.coordinates.x();

        for (int j = 0; j < 10; ++j) {
            if(sudokuNumbers[j].contains(posx,posy))
                return &sudokuNumbers[j]; ///Referencia al boton pulsado
        }
        return nullptr;
    }

    ///Comprobación de victoria
    bool GameScene::checkEndGame() {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (sudokuboard[i][j].boxvalue == lvlSol[i][j]) { ///Si el valor de la matriz del nivel, corresponde al de su solución
                    ++correctCells; ///Condición para victoria
                }
            }
        }

        ///Si todas las celdas se corresponden con la solucion, carga la pantalla de completado
        if (correctCells != 81)return false;
        else {
            //director.run_scene(shared_ptr<Scene>(new EndGameScene));
            return true;
        }
    }

    bool GameScene::checkColum(int j, int value)
    {
        for (int i = 0; i < N; i++)
            if (sudokuboard[i][0].boxvalue == value)
                return true;
        return 0;
    }

    bool GameScene::checkRow(int i, int value) {
        for (int j = 0; j < N; j++)
            if (sudokuboard[0][j].boxvalue == value)
              return true;
        return 0;
    }

    bool GameScene::checkSquare(int i, int j, int value)
    {
        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++)
                if (sudokuboard[i][j].boxvalue == value)
                    return true;
        return 0;
    }



}