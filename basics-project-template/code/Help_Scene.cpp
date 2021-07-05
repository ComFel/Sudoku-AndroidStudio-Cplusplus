/*
 * HELP SCENE
 * Copyright © 2021+ Felipe Vallejo Molina
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * felipevm07@gmail.com
 */

#include "Help_Scene.hpp"

#include <cstdlib>
#include <basics/Canvas>
#include <basics/Director>

using namespace basics;
using namespace std;

namespace example
{
    // ---------------------------------------------------------------------------------------------
    // ID y ruta de las texturas que se deben cargar para esta escena. La textura con el mensaje de
    // carga está la primera para poder dibujarla cuanto antes:

    Help_Scene::Texture_Data Help_Scene::textures_data[] =
            {
                    { ID(help_window),    "help-scene/help_window.png"         },
                    { ID(back_arrow),     "help-scene/back_arrow.png"          },
            };

    // PÃ¢ra determinar el número de items en el array textures_data, se divide el tamaaño en bytes
    // del array completo entre el tamaaño±o en bytes de un item:

    unsigned Help_Scene::textures_count = sizeof(textures_data) / sizeof(Texture_Data);

    // ---------------------------------------------------------------------------------------------

    Help_Scene::Help_Scene()
    {
        // Se establece la resolución virtual (independiente de la resolución virtual del dispositivo).
        // En este caso no se hace ajuste de aspect ratio, por lo que puede haber distorsión cuando
        // el aspect ratio real de la pantalla del dispositivo es distinto.

        canvas_width  = 1280;
        canvas_height =  720;

        // Se inicia la semilla del generador de números aleatorios:

        srand (unsigned(time(nullptr)));

        // Se inicializan otros atributos:

        initialize ();
    }

    // ---------------------------------------------------------------------------------------------

    void Help_Scene::update (float time)
    {
        load_textures  ();
    }

    // ---------------------------------------------------------------------------------------------

    void Help_Scene::render (Context & context)
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
        }
    }

    // ---------------------------------------------------------------------------------------------
    // En este método solo se carga una textura por fotograma para poder pausar la carga si el
    // juego pasa a segundo plano inesperadamente. Otro aspecto interesante es que la carga no
    // comienza hasta que la escena se inicia para así tener la posibilidad de mostrar al usuario
    // que la carga está en curso en lugar de tener una pantalla en negro que no responde durante
    // un tiempo.

    void Help_Scene::load_textures ()
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

                if (texture)
                {
                    context->add (texture);
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Help_Scene::create_sprites ()
    {
        // Se crean y configuran los sprites del fondo:

        Sprite_Handle         help_window(new Sprite( textures[ID(help_window)].get () ));
        Sprite_Handle         back_arrow (new Sprite( textures[ID (back_arrow)].get () ));

        help_window->set_anchor                                                  (CENTER);
        help_window->set_position              ({ canvas_width / 2, canvas_height / 2 });
        back_arrow->set_anchor                                          (CENTER | BOTTOM);
        back_arrow->set_position                             ({ canvas_width / 2, 100 });

        sprites.push_back        (help_window);
        sprites.push_back         (back_arrow);

        // Se guardan punteros a los sprites que se van a usar frecuentemente:

        h_window   =      help_window.get ();
        b_arrow    =       back_arrow.get ();
    }

    // ---------------------------------------------------------------------------------------------
    // Simplemente se dibujan todos los sprites que conforman la escena.

    void Help_Scene::render_help_scene (Canvas & canvas)
    {
        for (auto & sprite : sprites)
        {
            sprite->render (canvas);
        }
    }
}