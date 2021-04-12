
/** ModelCA.cpp
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

#include "ModelCA.hpp"

#include "Model2D.hpp"

#include <SDL.h>

#include <algorithm>

#include <cstring>


ModelCA::ModelCA(int _width, int _height)
    : Model2D(_width, _height), cells(new unsigned char[_width * _height]{}) { }

ModelCA::~ModelCA()
{
    delete[] cells;
}

unsigned char& ModelCA::cell(int x, int y)
{
    return cells[y * width + x];
}

ModelFrame* ModelCA::frame()
{
    return new ModelCAFrame(this);
}

void ModelCA::render(SDL_Surface* surface, SDL_Rect* dest, const ModelFrame* frame)
{
    const unsigned char* _cells = (frame != nullptr) ? ((ModelCAFrame*) frame)->cells : this->cells;

    int cellSize = std::min(dest->w / width, dest->h / height);
    int xp = (dest->w - cellSize * width) / 2;
    int yp = (dest->h - cellSize * height) / 2;

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            SDL_Colour c = COLOURS[_cells[y * width + x]];
            SDL_Rect r = {x * cellSize + xp + dest->x, y * cellSize + yp + dest->y, cellSize, cellSize};
            SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, c.r, c.g, c.b));
        }
    }
}

// M O D E L C A  F R A M E //
// ======================== //

ModelCAFrame::ModelCAFrame(const ModelCA* model) : cells(new unsigned char[model->width * model->height])
{
    std::memcpy(cells, model->cells, model->width * model->height);
}

ModelCAFrame::~ModelCAFrame()
{
    delete[] cells;
}
