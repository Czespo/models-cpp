
/** schelling.cpp, based on
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

#include "schelling.hpp"

#include "../ModelCA.hpp"
#include "../Viewer.hpp"

#include <cstdlib>
#include <cstdio>
#include <ctime>


int main(int argc, char** argv)
{
    // -1) Quick termination if the user wants help.
    if(argc > 1 && strcmp(argv[1], "-h") == 0)
    {
        printf("usage: schelling [options]\n");
        printf("-w width height (width and height of the viewer window. Default 800x600)\n");
        printf("-f              (makes the viewer window fullscreen)\n");
        printf("-fps fps        (frames per second of the viewer window. Default 30)\n");
        printf("-s width height (width and height of the model. Default 50x50)\n");
        printf("-d density      (the percentage of cells that are not empty. Default 0.9)\n");
        printf("-r red          (the percentage of non-empty cells that are red. Default 0.45)\n");
        printf("-r blue         (the percentage of non-empty cells that are blue. Default 0.45)\n");
        printf("-t blue         (the maximum percentage of opposite-coloured agents before an agent moves. Default 0.4)\n");
        printf("-n size         (the size of each agent's neighbourhood (in Chebyshev distance). Default 1)\n");
        printf("-fill           (when all agents are happy, fills empty cells with agents)\n");
        printf("-seed seed      (the seed used for pseudo-random number generation. Default RANDOM)\n");
        return 0;
    }

    // 0) Init args container.
    // Args args(argc, argv);

    // 1) Initialise model viewer.
    if(!Viewer::Init("Schelling's model of segregation", argc, argv))
    {
        fprintf(stderr, "Failed to initialise model viewer!\n");
        return 1;
    }

    // 2) Parse command-line arguments.
    int seed = 0;
    int width = 50, height = 50;
    float red = 0.45f, blue = 0.45f, threshold = 0.4f;
    int n = 1;
    bool fill = false;
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-s") == 0)
        {
            width = atoi(argv[i + 1]);
            height = atoi(argv[i + 2]);
        }
        else if(strcmp(argv[i], "-d") == 0)
        {
            red = blue = atof(argv[i + 1]) / 2;
        }
        else if(strcmp(argv[i], "-r") == 0)
        {
            red = atof(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-b") == 0)
        {
            blue = atof(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-t") == 0)
        {
            threshold = atof(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-n") == 0)
        {
            n = atoi(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-fill") == 0)
        {
            fill = true;
        }
        else if(strcmp(argv[i], "-seed") == 0)
        {
            seed = atoi(argv[i + 1]);
        }
    }

    // 3) Initialise the model.
    if(seed) srand(seed); else srand(time(NULL));

    Schelling schelling = Schelling(width, height, red, blue, threshold, n, fill);

    // 4) Attach the model to a viewer.
    Viewer::Get()->run(&schelling);
    Viewer::Quit();
    return 0;
}


const SDL_Colour ModelCA::COLOURS[3] = {
    {250, 253, 255, 255},
    {253, 204, 158, 255},
    {96, 137, 192, 255}
};

Schelling::Schelling(int _width, int _height, float _red, float _blue, float _threshold, int _n, bool _fill)
    : ModelCA(_width, _height), threshold(_threshold), red(_red), blue(_blue), n(_n), fill(_fill)
{
    init();
}

void Schelling::init()
{
    // Initialise model state. //
    int choices[3] = {EMPTY, RED, BLUE};
    float weights[3] = {1.0f - (red + blue), red, blue};

    empty.clear();
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            int choice = ModelCA::choice(3, choices, weights);
            if(choice == EMPTY) empty.push_back(std::pair<int, int>(x, y));

            cell(x, y) = choice;
        }
    }
}

bool Schelling::update()
{
    if(!empty.size()) return true;

    std::vector<std::pair<int, int>> unhappy;
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if(cell(x, y) != EMPTY)
            {
                int red = 0, blue = 0;
                for(int sy = y - n; sy <= y + n; sy++)
                {
                    if(sy < 0 || sy >= height)
                        continue;

                    for(int sx = x - n; sx <= x + n; sx++)
                    {
                        if(sx < 0 || sx >= width || (sx == x && sy == y))
                            continue;

                        if(cell(sx, sy) == RED)
                        {
                            red++;
                        }
                        else if(cell(sx, sy) == BLUE)
                        {
                            blue++;
                        }
                    }
                }

                int neighbours = red + blue;
                if(neighbours != 0)
                {
                    float frac_red = (float) red / neighbours;
                    float frac_blue = (float) blue / neighbours;

                    if(cell(x, y) == RED && frac_red < threshold)
                    {
                        unhappy.push_back(std::pair<int, int>(x, y));
                    }
                    else if(cell(x, y) == BLUE && frac_blue < threshold)
                    {
                        unhappy.push_back(std::pair<int, int>(x, y));
                    }
                }
            }
        }
    }

    if(unhappy.size() == 0)
    {
        if(fill)
        {
            // Fill all empty cells with red or blue agents.
            for(int y = 0; y < height; y++)
            {
                for(int x = 0; x < width; x++)
                {
                    if(cell(x, y) != EMPTY) continue;

                    int red = 0, blue = 0;
                    for(int sy = y - n; sy <= y + n; sy++)
                    {
                        if(sy < 0 || sy >= height)
                            continue;

                        for(int sx = x - n; sx <= x + n; sx++)
                        {
                            if(sx < 0 || sx >= width || (sx == x && sy == y))
                                continue;

                            if(cell(sx, sy) == RED)
                            {
                                red++;
                            }
                            else if(cell(sx, sy) == BLUE)
                            {
                                blue++;
                            }
                        }
                    }

                    int neighbours = red + blue;
                    float frac_red = red / neighbours;
                    float frac_blue = blue / neighbours;

                    cell(x, y) = (frac_red > frac_blue) ? RED : BLUE;
                }
            }

            empty.clear();
        }

        return true;
    }

    // Move unhappy agents to random empty locations. //
    for(auto p : unhappy)
    {
        int index = rand() % empty.size();
        int dx = empty[index].first, dy = empty[index].second;

        cell(dx, dy) = cell(p.first, p.second);
        cell(p.first, p.second) = EMPTY;
        empty[index] = p;
    }

    return false;
}
