
/** traffic.hpp, an visualization of the Biham-Middleton-Levine traffic model.
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

#ifndef TRAFFIC_HPP
#define TRAFFIC_HPP

#include "../ModelCA.hpp"

class Traffic : public ModelCA
{
    public:
        Traffic(int _width, int _height, float _density);
        ~Traffic() {}

        void init();
        bool update();

    private:
        const int EMPTY = 0;
        const int RED = 1;
        const int BLUE = 2;

        float density;
};

#endif // TRAFFIC_HPP
