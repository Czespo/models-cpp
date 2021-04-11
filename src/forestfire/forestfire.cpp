
/** forestfire.cpp, based on
 *  'A forest-fire model and some thoughts on turbulence' Bak, Chen, and Tang (1990)
 *
 *  Copyright (C) 2021 Czespo
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "forestfire.hpp"

#include "../ModelCA.hpp"
#include "../Viewer.hpp"
#include "../Args.hpp"

#include <cstdlib>
#include <cstdio>
#include <ctime>


int main(int argc, char** argv)
{
    // -1) Quick termination if the user wants help.
    if(argc > 1 && strcmp(argv[1], "-h") == 0)
    {
        printf("usage: forestfire [options]\n");
        printf("-w width height (width and height of the viewer window. Default 800x600)\n");
        printf("-f              (makes the viewer window fullscreen)\n");
        printf("-fps fps        (frames per second of the viewer window. Default 30)\n");
        printf("-s width height (width and height of the model. Default 50x50)\n");
        printf("-p prob         (the probability that an empty cell becomes a tree. Default 0.01)\n");
        printf("-f prob         (the probability that a tree catches fire. Default 0.001)\n");
        printf("-seed seed      (the seed used for pseudo-random number generation. Default RANDOM)\n");
        return 0;
    }

    // 0) Init args container.
    //Args args(argc, argv);

    // 1) Initialise model viewer.
    if(!Viewer::Init("Forest fire model", argc, argv))
    {
        fprintf(stderr, "Failed to initialise model viewer!\n");
        return 1;
    }

    // 2) Parse command-line arguments.
    int seed = 0;
    int width = 50, height = 50;
    float p = 0.01f, f = 0.001f;
    for(int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-s") == 0)
        {
            width = atoi(argv[i + 1]);
            height = atoi(argv[i + 2]);
        }
        else if(strcmp(argv[i], "-p") == 0)
        {
            p = atof(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-f") == 0)
        {
            f = atof(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-seed") == 0)
        {
            seed = atoi(argv[i + 1]);
        }
    }

    // 3) Initialise the model.
    if(seed) srand(seed);
    else srand(time(NULL));

    ForestFire forestfire = ForestFire(width, height, p, f);

    // 4) Attach the model to a viewer.
    Viewer::Get()->run(&forestfire);
    Viewer::Quit();
    return 0;
}


const SDL_Colour ModelCA::COLOURS[3] = {
    {250, 253, 255, 255},
    {96, 192, 137, 255},
    {255, 96, 96, 255}
};

ForestFire::ForestFire(int _width, int _height, float _p, float _f) : ModelCA(_width, _height), p(_p), f(_f)
{
    init();
}

void ForestFire::init()
{
    // Initialise model state. //
    int choices[2] = {EMPTY, TREE};
    float weights[2] = {1.0f - p, p};

    for(int y = 0; y < height; y++)
        for(int x = 0; x < width; x++)
            cell(x, y) = ModelCA::choice(2, choices, weights);
}

bool ForestFire::update()
{
    std::vector<std::pair<int, int>> burning;
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if(cell(x, y) == EMPTY)
            {
                // This cell becomes a tree with probability p.
                if((float) rand() / RAND_MAX < p)
                    cell(x, y) = TREE;
            }

            if(cell(x, y) == TREE)
            {
                // This cell catches fire spontaneously with probability f.
                if((float) rand() / RAND_MAX < f)
                    burning.push_back(std::pair<int, int>(x, y));
            }
            else if(cell(x, y) == FIRE)
            {
                // If any trees neighbour this cell, set them on fire.
                if(!outOfBounds(x, y - 1) && cell(x, y - 1) == TREE)
                    burning.push_back(std::pair<int, int>(x, y - 1));

                if(!outOfBounds(x + 1, y) && cell(x + 1, y) == TREE)
                    burning.push_back(std::pair<int, int>(x + 1, y));

                if(!outOfBounds(x, y + 1) && cell(x, y + 1) == TREE)
                    burning.push_back(std::pair<int, int>(x, y + 1));

                if(!outOfBounds(x - 1, y) && cell(x - 1, y) == TREE)
                    burning.push_back(std::pair<int, int>(x - 1, y));

                // Put this fire out.
                cell(x, y) = EMPTY;
            }
        }
    }

    for(auto p : burning)
        cell(p.first, p.second) = FIRE;

    return false;
}
