/*
 * REFERENCES
 * Copyright © 2019+ Ángel Rodríguez Ballesteros
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

using namespace basics;

// El linker tiende a eliminar código que cree que no se usa y luego, cuando se termina usando, lo echa
// en falta provocando errores. Dejar estas referencias aquí por el momento para evitar esos problemas:

void keep_links ()
{
    (void)Window::can_be_instantiated;
    Window::Accessor window;
    Window::create_window (0);
    Window::get_window (0);
    Graphics_Resource_Cache cache;
    opengles::Context::create(window, &cache);
    Canvas::Factory f = opengles::Canvas_ES2::create;
    Texture_2D::register_factory (0, 0);
}
