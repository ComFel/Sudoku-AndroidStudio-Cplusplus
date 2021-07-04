/*
 * INTRO SCENE
 * Copyright © 2021+ Felipe Vallejo Molina
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * felipevm07@gmail.com
 */

#include "IntroScene.h"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>
#include "MainMenuScene.h"
#include "Sample_Scene.hpp"


using namespace basics;
using namespace std;
using namespace Menu;
using namespace intro;
using namespace example;

namespace intro
{

    bool IntroScene::initialize()
    {
        if(state == UNINITIALIZED)
        {
            state = LOADING;
        } else{
            timer.reset();
            opacity = 0.f;

            state = FADING_IN;
        }


        return true;
    }

    void IntroScene::update(float time)
    {
        if(!suspended)
        {
            switch (state)
            {
                case LOADING:    update_loading    (); break;
                case FADING_IN:  update_fading_in  (); break;
                case WAITING:    update_waiting    (); break;
                case FADING_OUT: update_fading_out (); break;
                default: break;
            }
        }

    }

    void IntroScene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)
            {
                canvas->clear ();


                if (Logo_Texture)
                {
                    canvas->set_opacity (opacity);

                    canvas->fill_rectangle
                            (
                                    { canvas_width * .5f, canvas_height * .5f },
                                    { Logo_Texture->get_width(), Logo_Texture->get_height() },
                                    Logo_Texture. get ()
                            );

                }
            }
        }
    }

    void IntroScene::update_loading ()
    {
        Graphics_Context::Accessor context = director.lock_graphics_context ();

        if (context)
        {
            // Se carga la textura del logo:

            Logo_Texture = Texture_2D::create (0, context, "Logo_CFL.png");

            // Se comprueba si la textura se ha podido cargar correctamente:

            if (Logo_Texture)
            {
                context->add (Logo_Texture);

                timer.reset ();

                opacity = 0.f;
                state   = FADING_IN;
            }
            else
                state   = ERROR;
        }
    }

    void IntroScene::update_fading_in ()
    {
        float elapsed_seconds = timer.get_elapsed_seconds ();

        if (elapsed_seconds < 1.f)
        {
            opacity = elapsed_seconds;      // Se aumenta la opacidad del logo a medida que pasa el tiempo
        }
        else
        {
            timer.reset ();

            opacity = 1.f;
            state   = WAITING;
        }
    }

    void IntroScene::update_waiting ()
    {
        // Se esperan dos segundos sin hacer nada:

        if (timer.get_elapsed_seconds () > 2.f)
        {
            timer.reset ();

            state = FADING_OUT;
        }
    }

    void IntroScene::update_fading_out ()
    {
        float elapsed_seconds = timer.get_elapsed_seconds ();

        if (elapsed_seconds < .5f)
        {
            opacity = 1.f - elapsed_seconds * 2.f;      // Se reduce la opacidad de 1 a 0 en medio segundo
        }
        else
        {
            // Cuando el faceout se ha completado, se lanza la siguiente escena:

            state = FINISHED;

            director.run_scene (shared_ptr< Scene >(new MainMenuScene));

        }
    }



}
