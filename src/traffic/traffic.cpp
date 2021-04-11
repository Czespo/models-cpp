
/** traffic.cpp, a visualization of the Biham-Middleton-Levine traffic model.
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

#include "traffic.hpp"

#include "../ModelCA.hpp"
#include "../Viewer.hpp"

#include <cstdlib>
#include <cstdio>
#include <ctime>


int main(int argc, char** argv)
{
    // -1) Quick termination if the user wants help.
    if(argc > 1 && strcmp(argv[1], "-h") == 0)
    {
        printf("usage: traffic [options]\n");
        printf("-w width height (width and height of the viewer window. Default 800x600)\n");
        printf("-f              (makes the viewer window fullscreen)\n");
        printf("-fps fps        (frames per second of the viewer window. Default 30)\n");
        printf("-s width height (width and height of the model. Default 60x40)\n");
        printf("-d percent      (the percentage of cells which contain 'cars'. Each car is equally likely to be red or blue. Default 0.3)\n");
        printf("-seed seed      (the seed used for pseudo-random number generation. Default RANDOM)\n");
        return 0;
    }

    // 1) Initialise model viewer.
    if(!Viewer::Init("Biham-Middleton-Levine traffic model", argc, argv))
    {
        fprintf(stderr, "Failed to initialise model viewer!\n");
        return 1;
    }

    // 2) Parse command-line arguments.
    int seed = 0;
    int width = 60, height = 40;
    float density = 0.3f;
    for(int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-s") == 0)
        {
            width = atoi(argv[i + 1]);
            height = atoi(argv[i + 2]);
        }
        else if(strcmp(argv[i], "-d") == 0)
        {
            density = atof(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-seed") == 0)
        {
            seed = atoi(argv[i + 1]);
        }
    }

    // 3) Initialise the model.
    if(seed) srand(seed);
    else srand(time(NULL));

    Traffic traffic = Traffic(width, height, density);

    // 4) Attach the model to a viewer.
    Viewer::Get()->run(&traffic);
    Viewer::Quit();
    return 0;
}


const SDL_Colour ModelCA::COLOURS[3] = {
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0x40, 0x40, 0xFF},
    {0x40, 0x40, 0xFF, 0xFF}
};

Traffic::Traffic(int _width, int _height, float _density) : ModelCA(_width, _height), density(_density)
{
    init();
}

void Traffic::init()
{
    // Initialise the model's state. //
    int choices[3] = {EMPTY, RED, BLUE};
    float weights[3] = {1.0f - density, density / 2, density / 2};

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            cell(x, y) = ModelCA::choice(3, choices, weights);
        }
    }
}

bool Traffic::update()
{
    //bool jam = false;

    unsigned char* old_cells = new unsigned char[width * height];
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            old_cells[y * width + x] = cell(x, y);
        }
    }

    // Move all red cars.
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if(old_cells[y * width + x] == RED)
            {
                if(old_cells[y * width + (x + 1) % width] == EMPTY)
                {
                    cell((x + 1) % width, y) = RED;
                    cell(x, y) = EMPTY;
                }
                else
                {
                    //jam = true;
                }
            }
        }
    }

    for(int y = 0; y < height; y++)
        for(int x = 0; x < width; x++)
            old_cells[y * width + x] = cell(x, y);

    // Move all blue cars.
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if(old_cells[y * width + x] == BLUE)
            {
                if(old_cells[(y + 1) % height * width + x] == EMPTY)
                {
                    cell(x, (y + 1) % height) = BLUE;
                    cell(x, y) = EMPTY;
                }
                else
                {
                    //jam = true;
                }
            }
        }
    }

    delete[] old_cells;
    return false;
}
