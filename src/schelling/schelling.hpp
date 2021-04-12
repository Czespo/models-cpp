
/** schelling.hpp, based on
 * 'Models of Segregation' Schelling (1969)
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

#ifndef SCHELLING_HPP
#define SCHELLING_HPP

#include "../ModelCA.hpp"

#include <vector>

class Schelling : public ModelCA
{
    public:
        Schelling(int _width, int _height, float _red, float _blue, float _threshold, int _n, bool _fill);
        ~Schelling() {}

        void init();
        bool update();

    private:
        const unsigned char EMPTY = 0,
                            RED = 1,
                            BLUE = 2;

        std::vector<std::pair<int, int>> empty;

        float threshold, red, blue;
        int n;
        bool fill;
};

#endif // SCHELLING_HPP
