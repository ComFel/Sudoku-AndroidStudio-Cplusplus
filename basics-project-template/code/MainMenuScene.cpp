/*
 * INTRO SCENE
 * Copyright © 2021+ Felipe Vallejo Molina
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * felipevm07@gmail.com
 */

#include "MainMenuScene.h"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Transformation>

#include "GameScene.h"

using namespace basics;
using namespace std;
//using namespace Game;

namespace Menu
{
    MainMenuScene::MainMenuScene()
    {
        canvas_height = 1280;
        canvas_width = 720;
        suspended = true;
        state = LOADING;
    }

    bool MainMenuScene::initialize()
    {
        for (auto & option : options)
        {
            option.is_pressed = false;
        }

        return true;
    }

    void MainMenuScene::handle (basics::Event & event)
    {
        if (state == READY) // Se descartan los eventos cuando la escena está LOADING
        {
            switch (event.id)
            {
                case ID(touch-started): // El usuario toca la pantalla
                case ID(touch-moved):
                {
                    // Se determina qué opción se ha tocado:

                    Point2f touch_location = { *event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };
                    int     option_touched = option_at (touch_location);

                    // Solo se puede tocar una opción a la vez (para evitar selecciones múltiples),
                    // por lo que solo una se considera presionada (el resto se "sueltan"):

                    for (int i = 0; i < number_of_options; ++i)
                    {
                        options[i].is_pressed = i == option_touched;
                    }

                    break;
                }

                case ID(touch-ended): // El usuario deja de tocar la pantalla
                {
                    // Se "sueltan" todas las opciones:

                    for (auto & option : options) option.is_pressed = false;

                    // Se determina qué opción se ha dejado de tocar la última y se actúa como corresponda:

                    Point2f touch_location = { *event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };

                    if (option_at (touch_location) == PLAY)
                    {
                        state = FINISH;
                        //director.run_scene(shared_ptr<Scene>(new GameScene));----------------------------------------------------------------------------------------------------
                    }

                    break;
                }
            }
        }
    }

    void MainMenuScene::update (float time)
    {
        if (!suspended || state == LOADING)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                // Se carga el atlas:

                atlas.reset (new Atlas("MainMenu/main-menu.sprites", context));

                // Si el atlas se ha podido cargar el estado es READY y, en otro caso, es ERROR:

                state = atlas->good () ? READY : ERROR;

                // Si el atlas está disponible, se inicializan los datos de las opciones del menú:

                if (state == READY)
                {
                    OptionsConfig();
                }
            }
        }
    }


    void MainMenuScene::render(Graphics_Context::Accessor &context)
    {
        if (!suspended)
        {
            // El canvas se puede haber creado previamente, en cuyo caso solo hay que pedirlo:

            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            // Si no se ha creado previamente, hay que crearlo una vez:

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            // Si el canvas se ha podido obtener o crear, se puede dibujar con él:

            if (canvas)
            {
                canvas->clear ();

                if (state == READY)
                {
                    // Se dibuja el slice de cada una de las opciones del menú:

                    for (auto & option : options)
                    {
                        canvas->set_transform
                        (
                            scale_then_translate_2d
                            (
                                option.is_pressed ? 0.75f : 1.f, // Escala de la opción
                                {
                                    option.position[0], option.position[1] // Traslación
                                }
                            )
                        );

                        canvas->fill_rectangle ({ 0.f, 0.f }, { option.slice->width, option.slice->height }, option.slice, CENTER | TOP);
                    }

                    // Se restablece la transformación aplicada a las opciones para que no afecte a
                    // dibujos posteriores realizados con el mismo canvas:

                    canvas->set_transform(Transformation2f());
                }
            }
        }
    }

    void MainMenuScene::OptionsConfig()
    {
        options[PLAY].slice = atlas->get_slice(ID(PLAY));
        options[SCORES].slice = atlas->get_slice(ID(SCORES));
        options[HELP].slice = atlas->get_slice(ID(HELP));
        options[CREDITS].slice = atlas->get_slice(ID(CREDITS));


        float MenuHeight = 0;

        for(auto &option : options)
        {
            MenuHeight += option.slice->height;
        }

        float FirstOption = canvas_width / 2.f + MenuHeight / 2.f;

        for (unsigned i = 0; i < number_of_options; ++i)
        {
            options[i].position = Point2f{ canvas_width/2, FirstOption};
            FirstOption -= options[i].slice->height;
        }

        initialize();

    }

    int MainMenuScene::option_at(const Point2f &point)
    {
        for (int i = 0; i < number_of_options; ++i)
        {
            const Option & option = options[i];

            if(point[0] > option.position[0] - option.slice->width  &&
            point[0] < option.position[0] + option.slice->width  &&
            point[1] > option.position[1] - option.slice->height &&
            point[1] < option.position[1] + option.slice->height)
            {
                return i;
            }
        }

        return -1;
    }

}
