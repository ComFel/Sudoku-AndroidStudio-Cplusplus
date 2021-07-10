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
#include "Menu_Scene.hpp"

using namespace basics;
using namespace std;
using namespace example;

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

        canvas_width  = 720;
        canvas_height =  1820;

    }

    bool Help_Scene::initialize()
    {
        canvas_width  = 720;
        canvas_height =  1820;
        state = LOADING;
        suspended = false;
        y         = 720;
        x         = 370;

        return true;
    }

    void Help_Scene::suspend(){
        suspended = true;
    }

    void Help_Scene::resume(){
        suspended = false;
    }

    void Help_Scene::update (float time)
    {
        switch (state)
        {
            case LOADING: load_textures  ();     break;
            case RUNNING: run  (time); break;
        }

    }

    void Help_Scene::handle(Event &event) {
        if (state == RUNNING)
        {
            switch (event.id)
            {
                case ID(touch-started):
                    break;
                case ID(touch-moved):
                    break;
                case ID(touch-ended):
                    posX = *event[ID(x)].as< var::Float > ();
                    posY = *event[ID(y)].as< var::Float > ();
                    posFinal = {posX, posY};  // Guarda el último punto tocado

                    if (posFinal.coordinates.x() > 0 && posFinal.coordinates.y()>0 && posFinal.coordinates.x() < 80 && posFinal.coordinates.y()<250)
                    {
                        director.run_scene(shared_ptr<Scene>(new Menu_Scene));
                    }
                    break;
            }
        }
    }


    void Help_Scene::render (basics::Graphics_Context::Accessor & context)
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

                if(textureHelp && textureBack){
                    canvas->fill_rectangle({x,y},{800,1800},textureHelp.get());
                    canvas->fill_rectangle({25,100},{70,200},textureBack.get());
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

    void Help_Scene::load_textures ()
    {
        if (textures.size () < textures_count)          // Si quedan texturas por cargar...
        {
            // Las texturas se cargan y se suben al contexto gráfico, por lo que es necesario disponer
            // de uno:

            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                if(!textureBack){
                    textureBack = Texture_2D::create(ID(Help),context,"help-scene/back_arrow.png");
                    if (textureBack)
                    {
                        context->add (textureBack);
                    }
                }else if (!textureHelp){
                    textureHelp = Texture_2D::create(ID(Help),context,"help-scene/help_window.png");
                    if (textureHelp) {
                        context->add(textureHelp);
                    }
                }else{
                    state = RUNNING;
                }

            }
        }
    }

    void Help_Scene::run(float time) {

    }

}