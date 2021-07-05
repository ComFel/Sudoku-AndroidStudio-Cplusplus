/*
 * INTRO SCENE
 * Copyright © 2021+ Felipe Vallejo Molina
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * felipevm07@gmail.com
 */

#include <memory>
#include <basics/Scene>
#include <basics/Texture_2D>
#include <basics/Timer>
#include <basics/Canvas>

#define N 9

namespace Game
{
    using basics::Timer;

    class GameScene : public basics::Scene
    {

        typedef std::shared_ptr< basics::Texture_2D > Texture_Handle;

    public:

        enum State
        {
            LOADING,
            RUNNING,
            PAUSE,
        };

        State          state;
        bool           suspended{};

        unsigned       canvas_width;
        unsigned       canvas_height;

        Texture_Handle texture;

        Timer timer;

        struct Box{
            Texture_Handle textureCell;
            float x;
            float y;
        };

        Box BoxEmpty;
        Box Box1;
        Box Box2;
        Box Box3;
        Box Box4;
        Box Box5;
        Box Box6;
        Box Box7;
        Box Box8;
        Box Box9;

        int sudokuboard[N][N];

    public:

        GameScene();

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

        bool initialize () override;
        void suspend    () override;
        void resume     () override;

        void handle     (basics::Event & event) override;
        void update     (float time) override;
        void render     (basics::Graphics_Context::Accessor & context) override;

    private:

        bool checkColum(int c, int value);
        bool checkRow(int r, int value);
        bool checkSquare(int row, int column, int value);

        //void loadTextures();


    };

}

