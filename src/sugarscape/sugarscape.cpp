
/** sugarscape.cpp, based on
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

#include "sugarscape.hpp"

#include "../Model.hpp"
#include "../ModelCA.hpp"
#include "../Viewer.hpp"

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctime>

int main(int argc, char** argv)
{
    // -1) Quick termination if the user wants help.
    if(argc > 1 && strcmp(argv[1], "-h") == 0)
    {
        printf("usage: sugarscape [options]\n");
        printf("-w width height (width and height of the viewer window. Default 800x600)\n");
        printf("-f              (makes the viewer window fullscreen)\n");
        printf("-fps fps        (frames per second of the viewer window. Default 30)\n");
        printf("-s width height (width and height of the model. Default 50x50)\n");
        printf("-a agents       (the number of agents to place in the model. Default 400)\n");
        printf("-seed seed      (the seed used for pseudo-random number generation. Default RANDOM)\n");
        return 0;
    }

    // 1) Initialise model viewer.
    if(!Viewer::Init("Sugarscape model", argc, argv))
    {
        fprintf(stderr, "Failed to initialise model viewer!\n");
        return 1;
    }

    // 2) Parse command-line arguments.
    int seed = 0;
    int width = 50, height = 50, agents = 400;
    for(int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-s") == 0)
        {
            width = atoi(argv[i + 1]);
            height = atoi(argv[i + 2]);
        }
        else if(strcmp(argv[i], "-a") == 0)
        {
            agents = atoi(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-seed") == 0)
        {
            seed = atoi(argv[i + 1]);
        }
    }

    // 3) Initialise the model.
    if(seed) srand(seed);
    else srand(time(NULL));

    Sugarscape sugarscape = Sugarscape(width, height, agents);

    // 4) Attach the model to a viewer.
    Viewer::Get()->run(&sugarscape);
    Viewer::Quit();
    return 0;
}


Agent::Agent(int _x, int _y) : x(_x), y(_y)
{
    sugar = Model::randint(5, 26);
    vision = Model::randint(1, 7);
    metabolism = Model::randint(1, 4);
}

const SDL_Colour ModelCA::COLOURS[5] = {
    {0xFF, 0xFF, 0xD6, 0xFF},
    {0xFF, 0xF2, 0xB7, 0xFF},
    {0xFE, 0xE4, 0x99, 0xFF},
    {0xFE, 0xCC, 0x7B, 0xFF},
    {0xFD, 0xB1, 0x69, 0xFF}
};

Sugarscape::Sugarscape(int _width, int _height, int _agents)
    : ModelCA(_width, _height), capacity(new unsigned char[_width * _height]{}), nagents(_agents)
{
    init();
}

Sugarscape::~Sugarscape()
{
    delete[] capacity;
}

void Sugarscape::init()
{
    // Add sugarspots. //
    sugarspot(35, 15);
    sugarspot(15, 35);

    agents.clear();

    // Add agents. //
    unsigned char* positions = new unsigned char[width * height]{};
    for(int i = 0; i < nagents; i++) positions[i] = 1;

    // Shuffle array.
    shuffle(width * height, positions);

    // Use positions to spawn agents.
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if(positions[y * width + x]) agents.push_back(Agent(x, y));
        }
    }

    delete[] positions;
}

void Sugarscape::sugarspot(int cx, int cy)
{
    int thickness = 5;
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            double distance = hypot(cy - y, cx - x);
            int level = ceil(thickness - (distance + thickness - 1) / thickness);
            if(level >= thickness)
                level = 4;

            if(level > cell(x, y))
            {
                cell(x, y) = level;
                capacity[y * width + x] = level;
            }
        }
    }
}

bool Sugarscape::empty(int x, int y)
{
    for(Agent agent : agents)
        if(agent.x == x && agent.y == y)
            return false;

    return true;
}

const std::pair<int, int> INC[4] = {
    { 0, -1}, // Up
    { 1,  0},  // Right
    { 0,  1},  // Down
    {-1,  0}  // Left
};

void Sugarscape::move(Agent& agent)
{
    std::vector<std::pair<int, int>> options;
    int richest = 0;
    for(int d = 0; d < 4; d++)
    {
        int x = agent.x, y = agent.y, m = 0;
        while(m++ < agent.vision)
        {
            x += INC[d].first;
            y += INC[d].second;

            if(outOfBounds(x, y)) break;

            if(empty(x, y))
            {
                if(cell(x, y) > richest)
                {
                    richest = cell(x, y);
                    options.clear();
                    options.push_back(std::pair<int, int>(x, y));
                }
                else if(cell(x, y) == richest)
                {
                    options.push_back(std::pair<int, int>(x, y));
                }
            }
        }
    }

    // The agent cannot move because all of its
    // potential options were occupied.
    if(options.size() == 0) return;

    // Of options, determine the closest locations. //
    std::size_t theta = 0;
    double closest = 99;
    for(auto it = options.crbegin(), end = options.crend(); it != end; it++)
    {
        double distance = hypot(agent.x - (*it).first, agent.y - (*it).second);
        if(distance < closest)
        {
            closest = distance;
            while(theta)
            {
                options.pop_back();
                theta--;
            }

            theta++;
        }
        else if(distance == closest)
        {
            theta++;
        }
        else
        {
            options.erase(--(it.base()));
        }
    }

    std::pair<int, int> choice = options[rand() % options.size()];

    agent.x = choice.first;
    agent.y = choice.second;
}

ModelFrame* Sugarscape::frame()
{
    return new SugarscapeFrame(this);
}

bool Sugarscape::update()
{
    shuffle(agents.size(), (Agent*)(&agents[0]));
    for(auto it = agents.begin(); it != agents.end(); )
    {
        // Move the agent.
        move(*it);

        // Agent harvests sugar.
        (*it).sugar += cell((*it).x, (*it).y) - (*it).metabolism;
        cell((*it).x, (*it).y) = 0;

        // If agent is starving, remove it.
        if((*it).sugar < 0)
        {
            it = agents.erase(it);
        }
        else
        {
            it++;
        }
    }

    // Regrow sugar. //
    for(int y = 0; y < height; y++)
        for(int x = 0; x < width; x++)
            if(cell(x, y) < capacity[y * width + x])
                cell(x, y)++;

    return false;
}

void Sugarscape::render(SDL_Surface* surface, SDL_Rect* dest, const ModelFrame* frame)
{
    int cellSize = std::min(dest->w / width, dest->h / height);
    int xp = ((dest->w - cellSize * width) / 2) + dest->x;
    int yp = ((dest->h - cellSize * height) / 2) + dest->y;

    // Render terrain. //
    unsigned char* _cells = (frame != nullptr) ? ((SugarscapeFrame*) frame)->cells : this->cells;
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            SDL_Colour c = COLOURS[_cells[y * width + x]];
            SDL_Rect r = {x * cellSize + xp, y * cellSize + yp, cellSize, cellSize};
            SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, c.r, c.g, c.b));
        }
    }

    // Render agents. //
    Uint32 color = SDL_MapRGB(surface->format, 0xFF, 0x40, 0x40);
    int ah = cellSize / 2, aq = cellSize / 4;
    xp += aq;
    yp += aq;
    if(frame == nullptr)
    {
        for(Agent agent : agents)
        {
            SDL_Rect r{agent.x * cellSize + xp, agent.y * cellSize + yp, ah, ah};
            SDL_FillRect(surface, &r, color);
        }
    }
    else
    {
        for(std::pair<int, int> agent : ((SugarscapeFrame*) frame)->agents)
        {
            SDL_Rect r{agent.first * cellSize + xp, agent.second * cellSize + yp, ah, ah};
            SDL_FillRect(surface, &r, color);
        }
    }

}

// S U G A R S C A P E  F R A M E //
// ============================== //

SugarscapeFrame::SugarscapeFrame(const Sugarscape* model)
    : cells(new unsigned char[(model->width * 2) * model->height])
{
    for(int y = 0; y < model->height; y++)
    {
        for(int x = 0; x < model->width; x++)
        {
            cells[y * model->width + x] = model->cells[y * model->width + x];
        }
    }

    for(Agent agent : model->agents)
    {
        agents.push_back(std::pair<int, int>(agent.x, agent.y));
    }
}

SugarscapeFrame::~SugarscapeFrame()
{
    delete[] cells;
}
