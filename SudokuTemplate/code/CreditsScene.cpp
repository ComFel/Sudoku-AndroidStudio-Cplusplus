/*
 * CREDITS SCENE
 * Copyright © 2021+ Felipe Vallejo Molina
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * felipevm07@gmail.com
 */

#include "CreditsScene.h"

#include <cstdlib>
#include <basics/Canvas>
#include <basics/Director>
#include "Menu_Scene.hpp"

using namespace basics;
using namespace std;
using namespace example;

namespace example
{
    // ---------------------------------------------------------------------------------------------
    // ID y ruta de las texturas que se deben cargar para esta escena. La textura con el mensaje de
    // carga está la primera para poder dibujarla cuanto antes:

    CreditsScene::Texture_Data CreditsScene::textures_data[] =
            {
                    { ID(credits),    "credits-scene/credits.png"         },
                    { ID(back_arrow),     "credits-scene/back_arrow.png"          },
            };

    // PÃ¢ra determinar el número de items en el array textures_data, se divide el tamaaño en bytes
    // del array completo entre el tamaaño±o en bytes de un item:

    unsigned CreditsScene::textures_count = sizeof(textures_data) / sizeof(Texture_Data);

    CreditsScene::CreditsScene()
    {
        // Se establece la resolución virtual (independiente de la resolución virtual del dispositivo).
        // En este caso no se hace ajuste de aspect ratio, por lo que puede haber distorsión cuando
        // el aspect ratio real de la pantalla del dispositivo es distinto.

        canvas_width  = 720;
        canvas_height =  1820;

    }

    bool CreditsScene::initialize()
    {
        canvas_width  = 720;
        canvas_height =  1820;
        state = LOADING;
        suspended = false;
        y         = 640;
        x         = 360;

        return true;
    }

    void CreditsScene::suspend(){
        suspended = true;
    }

    void CreditsScene::resume(){
        suspended = false;
    }


    void CreditsScene::update (float time)
    {
        switch (state)
        {
            case LOADING: load_textures  ();     break;
            case RUNNING: run  (time); break;
        }

    }

    void CreditsScene::handle(Event &event) {
        if (state == RUNNING)
        {
            switch (event.id)
            {
                case ID(touch-started):
                    break;
                case ID(touch-moved):
                    break;
                case ID(touch-ended):///Al igual que la escena de ayuda, cuando el usuario toque la pantalla regrasa al menu principal
                    director.run_scene (shared_ptr< Scene >(new Menu_Scene));
                    break;
            }
        }
    }


    void CreditsScene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended && state == RUNNING)
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

                if(textureCred && textureBack){
                    canvas->fill_rectangle({x,y},{620,1500},textureCred.get());
                    canvas->fill_rectangle({670,1670},{50,150},textureBack.get());
                }


            }
        }
    }

    // ---------------------------------------------------------------------------------------------
    // En este método solo se carga una textura por fotograma para poder pausar la carga si el
    // juego pasa a segundo plano inesperadamente. Otro aspecto interesante es que la carga no
    // comienza hasta que la escena se inicia para así tener la posibilidad de mostrar al usuario
    // que la carga está en curso en lugar de tener una pantalla en negro que no responde durante
    // un tiempo.

    void CreditsScene::load_textures ()
    {
        if (textures.size () < textures_count)          // Si quedan texturas por cargar...
        {
            // Las texturas se cargan y se suben al contexto gráfico, por lo que es necesario disponer
            // de uno:

            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                if(!textureBack){
                    textureBack = Texture_2D::create(ID(back_arrow),context,"credits-scene/back_arrow.png");
                    if (textureBack)
                    {
                        context->add (textureBack);
                    }
                }else if (!textureCred){
                    textureCred = Texture_2D::create(ID(credits),context,"credits-scene/credits.png");
                    if (textureCred) {
                        context->add(textureCred);
                    }
                }else{
                    state = RUNNING;
                }

            }
        }
    }

    void CreditsScene::run(float time) {

    }

}
