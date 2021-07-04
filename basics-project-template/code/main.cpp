/*
 * MAIN
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include <basics/Director>
#include <basics/enable>
#include <basics/Graphics_Resource_Cache>
#include <basics/Texture_2D>
#include <basics/Window>
#include <basics/opengles/Context>
#include <basics/opengles/Canvas_ES2>
#include <basics/opengles/OpenGL_ES2>

#include "Sample_Scene.hpp"
#include "IntroScene.h"

using namespace basics;
using namespace example;
using namespace std;
using namespace intro;

int main ()
{
    // Es necesario habilitar un backend gráfico antes de nada:

    enable< basics::OpenGL_ES2 > ();

    // Se crea una escena y se inicia mediante el Director:

    //director.run_scene (shared_ptr< Scene >(new Sample_Scene));
    director.run_scene(shared_ptr< Scene >(new IntroScene));

    return 0;
}
