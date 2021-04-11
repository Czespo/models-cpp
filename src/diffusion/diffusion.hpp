
/** diffusion.hpp, based on
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

#ifndef DIFFUSION_HPP
#define DIFFUSION_HPP

#include "../Model2D.hpp"

class Diffusion;

struct DiffusionFrame : public ModelFrame
{
    DiffusionFrame(const Diffusion* model);
    ~DiffusionFrame();

    float* cells;
};

class Diffusion : public Model2D
{
    public:
        Diffusion(int width, int height, float r);
        ~Diffusion() {}

        void init();

        ModelFrame* frame();
        bool update();
        void render();

    private:
        float* cells;

        float r;
};

#endif // DIFFUSION_HPP
