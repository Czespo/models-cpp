
/** ModelCA.hpp
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

#ifndef MODELCA_HPP
#define MODELCA_HPP

#include "Model2D.hpp"

#include <SDL.h>

#include <cstdlib>

class ModelCA;

struct ModelCAFrame : public ModelFrame
{
    ModelCAFrame(const ModelCA* model);
    ~ModelCAFrame();

    unsigned char* cells;
};

class ModelCA : public Model2D
{
    public:
        static const SDL_Colour COLOURS[];

        /**
         * Returns `choices[i]` with probability `weights[i]`.
         */
        template<typename T>
        static T choice(int count, T* choices, float* weights)
        {
            float random = (float) rand() / RAND_MAX;
            float weight = 0.0f;
            for(int i = 0; i < count; i++)
            {
                weight += weights[i];
                if(random < weight) return choices[i];
            }

            return choices[0];
        }

        ModelCA(int width, int height);
        virtual ~ModelCA();

        /**
         * Initializes each cell of the model with the return value
         * of `ModelCA::choice(count, choices, weights)`
         */
        void init_cells(int count, unsigned char* choices, float* weights)
        {
            for(std::size_t i = 0, end = width * height; i < end; i++)
                cells[i] = ModelCA::choice(count, choices, weights);
        }

        unsigned char& cell(int x, int y);

        ModelFrame* frame();
        void render(SDL_Surface* surface, SDL_Rect* dest, const ModelFrame* frame=nullptr);

    protected:
        unsigned char* cells;

    friend ModelCAFrame::ModelCAFrame(const ModelCA* model);
};

#endif // MODELCA_HPP
