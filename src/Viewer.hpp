
/** Viewer.hpp
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

#ifndef VIEWER_HPP
#define VIEWER_HPP

#include "Model2D.hpp"
#include "Args.hpp"

#include <SDL.h>

#include <cstdlib>
#include <cstring>
#include <cstdio>

class Viewer
{
    public:
        static Viewer* viewer;

        static bool Init(const char* title, int argc, char** argv)
        {
            if(viewer == nullptr) Get();

            // Process command-line arguments. //
            bool fullscreen = false;
            for(int i = 0; i < argc; i++)
            {
                if(strcmp(argv[i], "-w") == 0)
                {
                    fullscreen = false;
                    if(i + 2 < argc)
                    {
                        viewer->width = atoi(argv[i + 1]);
                        viewer->height = atoi(argv[i + 2]);
                    }
                    else
                    {
                        viewer->width = 800;
                        viewer->height = 600;
                    }
                }
                else if(strcmp(argv[i], "-f") == 0)
                {
                    fullscreen = true;
                    viewer->width = 0;
                    viewer->height = 0;
                }
                else if(strcmp(argv[i], "-fps") == 0)
                {
                    int fps = atoi(argv[i + 1]);
                    if(fps)
                    {
                        viewer->delay = 1000 / fps;
                    }
                    else
                    {
                        viewer->delay = 0;
                    }
                }
            }

            // Initialise SDL. //
            if(SDL_Init(SDL_INIT_VIDEO) < 0)
            {
                printf("Init() failed to initialise SDL: %s\n", SDL_GetError());
                return false;
            }

            // Initialise window. //
            int flags = SDL_WINDOW_SHOWN;
            if(fullscreen)
            {
                flags |= SDL_WINDOW_FULLSCREEN;
            }

            viewer->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, viewer->width, viewer->height, flags);
            if(viewer->window == NULL)
            {
                printf("Viewer::Init() Window could not be created! SDL Error: %s\n", SDL_GetError());
                return false;
            }

            if(fullscreen)
            {
                // Set width and height.
                SDL_GL_GetDrawableSize(viewer->window, &viewer->width, &viewer->height);
            }

            // Initialise the model's drawing buffer.
            viewer->buffer = SDL_GetWindowSurface(viewer->window);

            // Let the model draw over the entire window.
            viewer->target_rect.x = viewer->target_rect.y = 0;
            viewer->target_rect.w = viewer->width;
            viewer->target_rect.h = viewer->height;

            // Initialise renderer. //
            viewer->renderer = SDL_CreateRenderer(viewer->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(viewer->renderer == NULL)
            {
                printf("Viewer::Init() Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                return false;
            }

            return true;
        }

        static void Quit()
        {
            delete viewer;
        }

        static Viewer* Get()
        {
            if(viewer == nullptr)
                viewer = new Viewer();

            return viewer;
        }

        ~Viewer();

        SDL_Renderer* getRenderer();

        bool isPlaying();
        int getWidth();
        int getHeight();

        void previous();
        void next();

        void render();

        void run(Model* _model);

    private:
        Viewer();

        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        SDL_Surface* buffer = NULL;

        Model* model = nullptr;

        int width = 800, height = 600;

        int delay = 33; // ~30 FPS.

        SDL_Rect target_rect;

        bool running = true;
        bool playing = false;
};

#endif // VIEWER_HPP
