
/** forestfire.hpp, based on
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

#ifndef FORESTFIRE_HPP
#define FORESTFIRE_HPP

#include "../ModelCA.hpp"

class ForestFire : public ModelCA
{
    public:
        ForestFire(int _width, int _height, float _p, float _f);
        ~ForestFire() {}

        void init();
        bool update();

    private:
        const int EMPTY = 0;
        const int TREE = 1;
        const int FIRE = 2;

        float p, f;
};

#endif // FORESTFIRE_HPP
