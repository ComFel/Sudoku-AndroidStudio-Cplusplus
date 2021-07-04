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

    }

    void GameScene::update(float time)
    {

    }

    void GameScene::render(basics::Graphics_Context::Accessor &context)
    {

    }
}