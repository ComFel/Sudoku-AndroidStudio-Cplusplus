/*
 * HELP SCENE
 * Copyright © 2021+ Felipe Vallejo Molina
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * felipevm07@gmail.com
 */


#include <map>
#include <list>
#include <memory>

#include <basics/Canvas>
#include <basics/Id>
#include <basics/Scene>
#include <basics/Texture_2D>

#include "Sprite.hpp"

namespace example
{
    using basics::Id;
    using basics::Canvas;
    using basics::Texture_2D;

    class Help_Scene : public basics::Scene
    {
        // Estos typedefs pueden ayudar a hacer el código más compacto y claro:

        typedef std::shared_ptr < Sprite     >     Sprite_Handle;
        typedef std::list< Sprite_Handle     >     Sprite_List;
        typedef std::shared_ptr< Texture_2D  >     Texture_Handle;
        typedef std::map< Id, Texture_Handle >     Texture_Map;


    private:

        /**
         * Array de estructuras con la información de las texturas (Id y ruta) que hay que cargar.
         */
        static struct   Texture_Data{
            Id id;
            const char * path;
        } textures_data[];

        /**
         * Número de items que hay en el array textures_data.
         */
        static unsigned textures_count;

    public:

        enum State
        {
            LOADING,
            RUNNING,
        };

        State          state;
        bool           suspended;
        float          x,y;                                 ///Coordenadas referencia para dibujar las texturas
        float posX,posY;                                    ///Coordenadas donde pulsa el jugador
        Point2f posFinal;

        unsigned       canvas_width;                        ///< Ancho de la resolución virtual usada para dibujar.
        unsigned       canvas_height;                       ///< Alto  de la resolución virtual usada para dibujar.

        Texture_Map    textures;                            ///< Mapa  en el que se guardan shared_ptr a las texturas cargadas.
        Sprite_List    sprites;                             ///< Lista en la que se guardan shared_ptr a los sprites creados.

        Texture_Handle textureHelp;                         ///< Textura de la imagen con las reglas del juego.
        Texture_Handle textureBack;                         ///< Textura para el boton de atras.


    public:

        /**
         * Solo inicializa los atributos que deben estar inicializados la primera vez, cuando se
         * crea la escena desde cero.
         */
        Help_Scene();

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

        bool initialize () override;
        void suspend() override;
        void resume() override;

        void handle     (basics::Event & event) override;

        /**
         * Este método se invoca automáticamente una vez por fotograma para que la escena
         * actualize su estado.
         */
        void update (float time) override;

        /**
         * Este método se invoca automáticamente una vez por fotograma para que la escena
         * dibuje su contenido.
         */
        void render (basics::Graphics_Context::Accessor & context) override;

    private:

        /**
         * En este método se cargan las texturas (una cada fotograma para facilitar que la
         * propia carga se pueda pausar cuando la aplicación pasa a segundo plano).
         */
        void load_textures ();

    };
}

