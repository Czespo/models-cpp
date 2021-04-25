
/** Events.cpp
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

#include "Events.hpp"

#include <SDL.h>

#include <cstdlib>
#include <cstring>

namespace Events
{
    int numkeys = 0;
    Uint8* previous_state = NULL;
    const Uint8* current_state = NULL;

    bool buttons[3] = {false, false, false};

    int mouse_x = 0, mouse_y = 0;

    bool _quit = false;

    bool init()
    {
        current_state = SDL_GetKeyboardState(&numkeys);

        previous_state = (Uint8*) std::malloc(numkeys);
        if(previous_state == NULL) return false;

        std::memcpy(previous_state, current_state, numkeys);

        return true;
    }

    void quit()
    {
        free(previous_state);
    }

    void update()
    {
        SDL_GetMouseState(&mouse_x, &mouse_y);

        // Save current state, only used for keypress checks.
        std::memcpy(previous_state, current_state, numkeys);

        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    _quit = true;
                    break;

                case SDL_MOUSEBUTTONDOWN:

                    if(e.button.button == SDL_BUTTON_LEFT)
                        buttons[MOUSE_LEFT] = true;
                    else if(e.button.button == SDL_BUTTON_MIDDLE)
                        buttons[MOUSE_MIDDLE] = true;
                    else if(e.button.button == SDL_BUTTON_RIGHT)
                        buttons[MOUSE_RIGHT] = true;
                    break;

                case SDL_MOUSEBUTTONUP:

                    if(e.button.button == SDL_BUTTON_LEFT)
                        buttons[MOUSE_LEFT] = false;
                    else if(e.button.button == SDL_BUTTON_MIDDLE)
                        buttons[MOUSE_MIDDLE] = false;
                    else if(e.button.button == SDL_BUTTON_RIGHT)
                        buttons[MOUSE_RIGHT] = false;
                    break;
            }
        }
    }

    bool key_down(int key)
    {
        return current_state[key];
    }

    bool key_pressed(int key)
    {
        return (previous_state[key] == 1) && (current_state[key] == 0);
    }

    bool button_down(int button)
    {
        return buttons[button];
    }

    void get_mouse_pos(int* x, int* y)
    {
        /// TODO: Basically a wrapper for SDL_GetMouseState().
        if(x != NULL) *x = mouse_x;
        if(y != NULL) *y = mouse_y;
    }

    bool quit_requested()
    {
        return _quit;
    }
}
