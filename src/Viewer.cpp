
/** Viewer.cpp
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

#include "Viewer.hpp"

#include "Events.hpp"
#include "Model.hpp"

#include <SDL.h>

#include <cstdlib>
#include <ctime>

Viewer* Viewer::viewer = nullptr;

Viewer::Viewer() {}

Viewer::~Viewer()
{
	// Close SDL. //
	if(SDL_WasInit(0) > 0)
    {
        if(window != NULL)
            SDL_DestroyWindow(window);

        if(renderer != NULL)
            SDL_DestroyRenderer(renderer);

        SDL_Quit();
    }
}

SDL_Renderer* Viewer::getRenderer()
{
    return renderer;
}

bool Viewer::isPlaying()
{
    return playing;
}

int Viewer::getWidth()
{
    return width;
}

int Viewer::getHeight()
{
    return height;
}

void Viewer::previous()
{
    // TODO
}

void Viewer::next()
{
    if(model->update())
    {
        playing = false;
    }

    render();
}

void Viewer::render()
{
    // if(no_window) return;

    // Render black background. //
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    // Render model. //
    if(model != nullptr)
    {
        // Clear buffer.
        SDL_Rect cl{0, 0, width, height};
        SDL_FillRect(buffer, &cl, SDL_MapRGB(buffer->format, 0x00, 0x00, 0x00));

        model->render(buffer, &target_rect);

        // Convert buffer to texture and copy to renderer.
        SDL_Texture* tx = SDL_CreateTextureFromSurface(renderer, buffer);
        SDL_RenderCopy(renderer, tx, NULL, &target_rect);
        SDL_DestroyTexture(tx);
    }

    SDL_RenderPresent(renderer);
}

void Viewer::run(Model* _model)
{
    // if(viewer->no_window) ...

    model = _model;
    render();

    while(running)
    {
        std::clock_t start = std::clock();

        // Process events. //
        Events::update();

        if(Events::quit_requested())
        {
            running = false;
        }
        else
        {
            if(Events::key_pressed(SDL_SCANCODE_SPACE))
            {
                playing = !playing;
            }
            else if(Events::key_pressed(SDL_SCANCODE_LEFT))
            {
                previous();
            }
            else if(Events::key_pressed(SDL_SCANCODE_RIGHT))
            {
                next();
            }
            else if(Events::key_pressed(SDL_SCANCODE_R))
            {
                playing = false;
                model->init();
                render();
            }

            if(playing) next();

            if(delay > 0)
            {
                int dl = delay - (std::clock() - start);
                if(dl > 0) SDL_Delay(dl);
            }
        }
    }
}
