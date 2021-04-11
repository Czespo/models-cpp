
/** Model.hpp
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

#ifndef MODEL_HPP
#define MODEL_HPP

#include <SDL.h>

#include <cstdlib>

struct ModelFrame
{
    virtual ~ModelFrame() {}
};

class Model
{
    public:
        static int randint(int min, int max)
        {
            return rand() % (min - max) + min;
        }

        virtual ~Model() {}

        virtual void init() {}

        virtual ModelFrame* frame() = 0;
        virtual bool update() = 0;
        virtual void render(SDL_Surface* surface, SDL_Rect* dest, const ModelFrame* frame=nullptr) = 0;
};

#endif // MODEL_HPP
