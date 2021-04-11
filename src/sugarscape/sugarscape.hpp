
/** sugarscape.hpp, based on
 *  'Growing Artificial Societies' Epstein, Axtell (1996)
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

#ifndef SUGARSCAPE_HPP
#define SUGARSCAPE_HPP

#include "../ModelCA.hpp"

#include <utility>
#include <vector>

struct Agent
{
    Agent(int x, int y);

    int x, y;
    int sugar, vision, metabolism;
};

class Sugarscape;

struct SugarscapeFrame : public ModelFrame
{
    SugarscapeFrame(const Sugarscape* model);
    ~SugarscapeFrame();

    unsigned char* cells;
    std::vector<std::pair<int, int>> agents;
};

class Sugarscape : public ModelCA
{
    public:
        template<typename T>
        static void shuffle(std::size_t count, T* array)
        {
            while(count)
            {
                std::size_t index = rand() % count--;

                T temp = array[count];
                array[count] = array[index];
                array[index] = temp;
            }
        }

        Sugarscape(int width, int height, int agents);
        ~Sugarscape();

        void sugarspot(int cx, int cy);
        bool empty(int x, int y);
        void move(Agent& agent);

        void init();

        ModelFrame* frame();
        bool update();
        void render(SDL_Surface* surface, SDL_Rect* dest, const ModelFrame* frame);

    private:
        std::vector<Agent> agents;
        unsigned char* capacity;

        int nagents;

    friend SugarscapeFrame::SugarscapeFrame(const Sugarscape*);
};

#endif // SUGARSCAPE_HPP
