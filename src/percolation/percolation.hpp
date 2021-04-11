
/** percolation.hpp
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

#ifndef PERCOLATION_HPP
#define PERCOLATION_HPP

#include "../ModelCA.hpp"

class Percolation : public ModelCA
{
    public:
        Percolation(int _width, int _height, float _q);
        ~Percolation() {}

        void init();
        bool update();

    private:
        const int NONPOROUS = 0;
        const int POROUS = 1;
        const int WET = 2;

        float q;
};

#endif // PERCOLATION_HPP
