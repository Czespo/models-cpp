
/** diffusion.cpp, based on
 *  'The chemical basis of morphogenesis' Alan Turing (1952)
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

#include "diffusion.hpp"

#include "../Model2D.hpp"
#include "../Viewer.hpp"

#include <cstdlib>
#include <cstdio>
#include <ctime>


int main(int argc, char** argv)
{
    // 1) Initialise model viewer.
    if(!Viewer::Init("Diffusion model", argc, argv))
    {
        fprintf(stderr, "Failed to initialise model viewer!\n");
        return 1;
    }

    // 2) Parse command-line arguments.
    int seed = 0;
    int width = 9, height = 9;
    float r = 0.01;
    for(int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-s") == 0)
        {
            width = atoi(argv[i + 1]);
            height = atoi(argv[i + 2]);
        }
        else if(strcmp(argv[i], "-r") == 0)
        {
            p = atof(argv[i + 1]);
        }
    }

    // 3) Initialise the model.
    if(seed) srand(seed);
    else srand(time(NULL));

    Diffusion diffusion = Diffusion(width, height, r);

    // 4) Attach the model to a viewer.
    Viewer::Get()->run(&diffusion);
    Viewer::Quit();
    return 0;
}

Diffusion::Diffusion(int _width, int _height, float _r)
    : Model2D(_width, _height), r(_r), cells(new float[_width * _height]{})
{
    /// TODO
}

void Diffusion::init()
{
    /// TODO
}

ModelFrame* Diffusion::frame()
{
    return new DiffusionFrame(this);
}

bool Diffusion::update()
{
    /// TODO
    return true;
}

void Diffusion::render(SDL_Surface* surface, SDL_Rect* dest, const ModelFrame* frame)
{
    /// TODO
}

// D I F F U S I O N  F R A M E //
// ============================ //

DiffusionFrame::DiffusionFrame(const Diffusion* model)
    : cells(new float[model->width * model->height])
{
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            cells[y * model->width + x] = model->cells[y * model->width + x];
        }
    }
}

~DiffusionFrame()
{
    delete[] cells;
}
