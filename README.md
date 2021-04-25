
# models-cpp
Some models, mostly cellular automata, written in C++ with [SDL2](http://libsdl.org). If you have any trouble compiling or running the models, please [start an issue](https://github.com/Czespo/models-cpp/issues/new)!

## Compiling
You need the SDL2 headers and static libraries. You can probably get them from <http://libsdl.org/download-2.0.php>.

Each of the models is intended to compile into an independant executable.

## Running
Download the SDL2 runtime binaries, or build them from source: <http://libsdl.org/download-2.0.php>.

## Models
- diffusion - A model of [diffusion](https://en.wikipedia.org/wiki/Diffusion), i.e. the movement of energy from high concentration areas to low concentration areas. Currently unimplemented.
- forestfire - A model as described in Bak, Chen, and Tang's 1990 paper 'A forest-fire model and some thoughts on turbulence'.
- percolation - A model of [percolation](https://en.wikipedia.org/wiki/Percolation).
- schelling - A model as described in Schelling's 1969 paper 'Models of Segregation', rendered in two dimensions.
- sugarscape - A model as described in Epstein and Axtell's 1996 book 'Growing Artificial Societies'.
- traffic - The [Biham-Middleton-Levine traffic model](https://en.wikipedia.org/wiki/Biham%E2%80%93Middleton%E2%80%93Levine_traffic_model).

## Controls

- Use the `left` and `right` arrow keys to step backwards and forwards through the model (currently, stepping backwards is not implemented.)
- Use the `space` key to toggle autoplay of the model.
- Press `r` to reinitialize the model to a completely new state.

## Credits
Many of the above models are presented in Allen Downey's book [Think Complexity](http://greenteapress.com/wp/think-complexity/), 2nd edition. After reading his excellent book, I contrived to implement the models myself, as a learning exercise. If you'd like an in-depth analysis of these models, and complexity science in general, I encourage you to read Mr. Downey's book.
