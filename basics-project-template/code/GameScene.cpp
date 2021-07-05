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
#include "MainMenuScene.h"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Transformation>

//Definiendo la dimension de 9 valores para el sudoku
#define N 9

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

    void GameScene::handle(basics::Event &event)
    {

    }

    void GameScene::update(float time)
    {
        switch (state)
        {
            case LOADING: ; break;
            case RUNNING: ; break;
            case PAUSE: suspend(); break;

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

            BoxEmpty.textureCell = Texture_2D::create(0,context,"SpritesGame/Empty.png");
            BoxEmpty.textureCell = Texture_2D::create(0,context,"SpritesGame/Number1.png");
            BoxEmpty.textureCell = Texture_2D::create(0,context,"SpritesGame/Number2.png");
            BoxEmpty.textureCell = Texture_2D::create(0,context,"SpritesGame/Number3.png");
            BoxEmpty.textureCell = Texture_2D::create(0,context,"SpritesGame/Number4.png");
            BoxEmpty.textureCell = Texture_2D::create(0,context,"SpritesGame/Number5.png");
            BoxEmpty.textureCell = Texture_2D::create(0,context,"SpritesGame/Number6.png");
            BoxEmpty.textureCell = Texture_2D::create(0,context,"SpritesGame/Number7.png");
            BoxEmpty.textureCell = Texture_2D::create(0,context,"SpritesGame/Number8.png");
            BoxEmpty.textureCell = Texture_2D::create(0,context,"SpritesGame/Number9.png");

        }
    }


    bool GameScene::checkColum(int c, int value)
    {
        for (int i = 0; i < N; i++)
            //if (sudokuboard[i] == value)
              //  return true;
        return 0;
    }

    bool GameScene::checkRow(int r, int value) {
        for (int i = 0; i < N; i++)
            //if (sudokuboard[r] == value)
            //  return true;
        return 0;
    }


    bool GameScene::checkSquare(int row, int column, int value)
    {
        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++)
                //   if (sudokuboard[r+row] == value)
                //  return true;
        return 0;
    }

    bool canEnter(int r, int c, int value)
    {
        // return !checkRow(r, value) && !checkColumn(c, value) && !checkBox(r-r%3,c-c%3,value);
    }
}