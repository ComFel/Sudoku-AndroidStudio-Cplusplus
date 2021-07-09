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



using namespace basics;
using namespace std;


namespace Game
{

    GameScene::GameScene()
    {
        canvas_width = 720;
        canvas_height = 1280;
        suspended = false;
    }

    bool GameScene::initialize()
    {
        state = LOADING;
        x = 10;
        y = 10;
        botX = 570;
        botY = 100;
        valorSelect = 0;
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
                case ID(touch-started):
                {
                    posX = *event[ID(x)].as< var::Float > ();
                    posY = *event[ID(y)].as< var::Float > ();
                    posInicial = {posX, posY};



                    break;
                }

                case ID(touch-ended):  // El usuario deja de tocar la pantalla
                {
                    posX = *event[ID(x)].as< var::Float > ();
                    posY = *event[ID(y)].as< var::Float > ();
                    posFinal = {posX, posY};  // Guarda el último punto tocado

                    //valorSelect = checkCellValue(posFinal);

                    //Boton PUASE
                    if (posFinal.coordinates.x() > 0 && posFinal.coordinates.y()>0 && posFinal.coordinates.x() < 70 && posFinal.coordinates.y()<200){
                        state = PAUSE;
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
            case LOADING: loadTextures2(); break;
            case RUNNING: break;
            case PAUSE: suspend(); break;
            case ERROR: break;

        }
    }

    void GameScene::render(basics::Graphics_Context::Accessor &context)
    {
        if (!suspended && state == LOADING)
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

                drawGameInfo(*canvas);
                drawCells(*canvas);
                drawNumbers(*canvas);

                state = RUNNING;
                /*switch (state) {
                    case LOADING:
                        render_loading(*canvas);
                        break;
                    case RUNNING:
                        render_playfield(*canvas);
                        break;
                    case ERROR:
                        break;
                }*/
            }
        }
    }

    /*
    void GameScene::loadTextures()
    {
        if (textures.size () < textures_count)          // Si quedan texturas por cargar...
        {
            // Las texturas se cargan y se suben al contexto gráfico, por lo que es necesario disponer
            // de uno:

            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                // Se carga la siguiente textura (textures.size() indica cuántas llevamos cargadas):

                Texture_Data   & texture_data = textures_data[textures.size ()];
                Texture_Handle & texture      = textures[texture_data.id] = Texture_2D::create (texture_data.id, context, texture_data.path);

                // Se comprueba si la textura se ha podido cargar correctamente:

                if (texture) context->add (texture); else state = ERROR;

                // Cuando se han terminado de cargar todas las texturas se pueden crear los sprites que
                // las usarán e iniciar el juego:
            }
        }
        else
        if (timer.get_elapsed_seconds () > 1.f)         // Si las texturas se han cargado muy rápido
        {                                               // se espera un segundo desde el inicio de
            create_sprites ();                          // la carga antes de pasar al juego para que el mensaje de carga no aparezca y desaparezca demasiado rápido.
            //restart_game   ();
            state = RUNNING;
        }
    }

    void GameScene::create_sprites ()
    {
        // Se crean y configuran los sprites del fondo:

        Sprite_Handle    EmptyCell(new Sprite( textures[ID(CeldaVacia)].get () ));

        EmptyCell->set_anchor   (CENTER | LEFT);
        EmptyCell->set_position ({ 0, canvas_height });


        spriteList.push_back (EmptyCell);
        EmptyCell->set_scale(.1);

        // Se guardan punteros a los sprites que se van a usar frecuentemente:
        CeldaVacia    =             EmptyCell.get ();
    }

    void GameScene::render_loading (Canvas & canvas)
    {
        Texture_2D * loading_texture = textures[ID(loading)].get ();

        if (loading_texture)
        {
            canvas.fill_rectangle
                    (
                            { canvas_width * .5f, canvas_height * .5f },
                            { loading_texture->get_width (), loading_texture->get_height () },
                            loading_texture
                    );
        }
    }

    void GameScene::render_playfield (Canvas & canvas)
    {
        for (auto & sprite : spriteList)
        {
            sprite->render (canvas);
        }
    }
    */

    void GameScene::loadTextures2()
    {
        if (!suspended)
        {
            //Creación del contexto gráfico
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                //Forma de crear las texturas de la escena

                textures.push_back(Texture_2D::create(0,context,"SpritesGame/Empty.png"));      //0
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



                BackButton = Texture_2D::create(0,context,"SpritesGame/PauseButton.png");

                context->add(BackButton);

                x = 100;
                y = 100;
                for (int i = 0; i < N; ++i)
                {
                    y = 100;
                    for (int j = 0; j < N; ++j) {
                        sudokuboard[i][j].x = x;
                        sudokuboard[i][j].y = y;
                        sudokuboard[i][j].set_value(0,textures);
                        y+= 130;
                    }
                    x+=40;
                }
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

    //Crear los botones para que el usuario decida que valor poner
    void GameScene::drawNumbers(basics::Canvas &canvas)
    {



    }

    void GameScene::drawGameInfo(basics::Canvas &canvas)
    {
        if(!suspended){
            x = 25;
            y= 100;
            canvas.fill_rectangle({ x, y }, { 50,150 }, BackButton.get());
        }
    }

    GameScene::Box* GameScene::checkCellValue(Point2f &posToCheck)
    {
        float posy = posToCheck.coordinates.y();
        float posx= posToCheck.coordinates.x();

        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j) {
                if(sudokuboard[i][j].contains(posx,posy))
                    return &sudokuboard[i][j];
            }

        }
        return nullptr;
    }

    int GameScene::getCell(Point2f &posSelect) {
        sudokuboard[0][0];
        x = 100;
        y = 100;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if(sudokuboard[i][j].boxvalue)
                {

                }

            }
        }
    }

    void GameScene::drawSelection(int valor, Texture_Handle reTexture) {
        valor = valorSelect;
    }




    bool GameScene::checkColum(int j, int value)
    {
        for (int i = 0; i < N; i++)
            if (sudokuboard[i][0].boxvalue == value)
                return true;
        return 0;
    }

    bool GameScene::checkRow(int i, int value) {
        for (int j = i; j < N; j++)
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

    void GameScene::CellChecks() {
        //checkSquare();
    }

    bool canEnter(int r, int c, int value)
    {
        //return !checkRow(r, value) && !checkColumn(c, value) && !checkBox(r-r%3,c-c%3,value);
    }
}