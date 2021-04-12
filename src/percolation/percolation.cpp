
/** percolation.cpp
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

#include "percolation.hpp"

#include "../ModelCA.hpp"
#include "../Viewer.hpp"
#include "../Args.hpp"

#include <vector>

#include <cstdlib>
#include <cstdio>
#include <ctime>

int main(int argc, char** argv)
{
    // -1) Quick termination if the user wants help.
    if(argc > 1 && strcmp(argv[1], "-h") == 0)
    {
        printf("usage: percolation [options]\n");
        printf("-w width height (width and height of the viewer window. Default 800x600)\n");
        printf("-f              (makes the viewer window fullscreen)\n");
        printf("-fps fps        (frames per second of the viewer window. Default 30)\n");
        printf("-s width height (width and height of the model. Default 50x50)\n");
        printf("-q prob         (the probability that a cell is porous. Default 0.6)\n");
        printf("-seed seed      (the seed used for pseudo-random number generation. Default RANDOM)\n");
        return 0;
    }

    // 0) Init args container.
    //Args args(argc, argv);

    // 1) Initialise model viewer.
    if(!Viewer::Init("Percolation model", argc, argv))
    {
        fprintf(stderr, "Failed to initialise model viewer!\n");
        return 1;
    }

    // 2) Parse command-line arguments.
    int seed = 0;
    int width = 50, height = 50;
    float q = 0.6f;
    for(int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-s") == 0)
        {
            width = atoi(argv[i + 1]);
            height = atoi(argv[i + 2]);
        }
        else if(strcmp(argv[i], "-q") == 0)
        {
            q = atof(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-seed") == 0)
        {
            seed = atoi(argv[i + 1]);
        }
    }

    // 3) Initialise the model.
    if(seed) srand(seed);
    else srand(time(NULL));

    Percolation percolation = Percolation(width, height, q);

    // 4) Attach the model to a viewer.
    Viewer::Get()->run(&percolation);
    Viewer::Quit();
    return 0;
}


const SDL_Colour ModelCA::COLOURS[3] = {
    {250, 253, 255, 255},
    {226, 237, 247, 255},
    {107, 131, 166, 255}
};

Percolation::Percolation(int _width, int _height, float _q) : ModelCA(_width, _height), q(_q)
{
    init();
}

void Percolation::init()
{
    // Initialise model state. //
    unsigned char choices[2] = {NONPOROUS, POROUS};
    float weights[2] = {1.0f - q, q};

    for(std::size_t i = 0, end = width * height; i < end; i++)
    {
        if(i < (std::size_t) width)
        {
            cells[i] = WET;
        }
        else
        {
            cells[i] = ModelCA::choice(2, choices, weights);
        }
    }
}

bool Percolation::update()
{
    std::vector<std::pair<int, int>> wet;
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if(cell(x, y) != WET)
                continue;

            if(!outOfBounds(x, y - 1) && cell(x, y - 1) == POROUS)
                wet.push_back(std::pair<int, int>(x, y - 1));

            if(!outOfBounds(x + 1, y) && cell(x + 1, y) == POROUS)
                wet.push_back(std::pair<int, int>(x + 1, y));

            if(!outOfBounds(x, y + 1) && cell(x, y + 1) == POROUS)
                wet.push_back(std::pair<int, int>(x, y + 1));

            if(!outOfBounds(x - 1, y) && cell(x - 1, y) == POROUS)
                wet.push_back(std::pair<int, int>(x - 1, y));
        }
    }

    if(wet.size() == 0)
        return true;

    for(auto p : wet)
        cell(p.first, p.second) = WET;

    return false;
}
