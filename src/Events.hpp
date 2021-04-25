
/** Events.hpp
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

#ifndef EVENTS_HPP
#define EVENTS_HPP

namespace Events
{
    static const int MOUSE_LEFT = 0,
                     MOUSE_MIDDLE = 1,
                     MOUSE_RIGHT = 2;

    bool init();
    void quit();
    void update();

    /** Returns true iff the specified key is down. */
    bool key_down(int key);

    /** Returns true iff the specified key was pressed and released. */
    bool key_pressed(int key);

    /** Returns true iff the specified mouse button is down. */
    bool button_down(int button);

    /** Writes the mouse position to the x and y out-arguments. */
    void get_mouse_pos(int* x, int* y);

    /** Returns true iff an SDL_QUIT event was recieved. */
    bool quit_requested();
}

#endif // EVENTS_HPP
