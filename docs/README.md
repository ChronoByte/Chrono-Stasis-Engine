# KITTY LEE

Welcome to our website github page!

## About the game and its context

This is a small project made by two videogame students of UPC CITM from Terrassa called Kitty Lee. Kitty Lee a side-scrolling platformer game about exploring a mysterious underground place. Our Hero is a skilled warrior cat with a red band and his name is Lee.
There is two different levels: while the first one is based on the player exploration on the cave, the second one represents the exit of this dangerous place where the player has to escape from. Lee will have to reach the end of the levels avoiding enemies which will start chasing him. Besides, Lee can collect coins that he finds into the levels.

## Team members

### Peña Hernando Carlos:

![](https://github.com/CarlosUPC/Kitty-Lee/blob/master/docs/foto_carlos.jpg)

* GitHub Account: [CarlosUPC](https://github.com/CarlosUPC).

* Role: Main Menu UI programmer, IA/pathfinding programmer, Level designer on Tiled

### Martinez de la Rosa, Christian:

![](https://github.com/CarlosUPC/Kitty-Lee/blob/master/docs/foto_sebas.jpg)

* GitHub Account:[christt105](https://github.com/christt105).

* Role: In-Game UI, Framerate normalization, entity managment, Level designer on Tiled, HUD programmer

See also the list of [contributors](https://github.com/CarlosUPC/Game_Dev_2DPlatformGame/graphs/contributors) who participated in this project.

## Main core subsystems of the game
Our game code is structured in modules. The main module (called j1App.cpp) manages all the other modules calling in a loop its respective awake, preupdate, update, postupdate, cleanup that they connect throught a base class j1Module.

Two of its modules(GUI and Entity Manager) manage a subsystem of modules that follows the same structure that the explained above with the objective to have an independent enitity and UI system that connects with the logic of the core game system.

## Repository Link and last release
* [Repository](https://github.com/CarlosUPC/Kitty-Lee)
* [Release](https://github.com/CarlosUPC/Kitty-Lee/releases/tag/0.3.5)


## Instructions to play

* A to move left.
* D to move right.
* G to change between ghost form and normal form.
* Spacebar to jump.
* S + Spacebar on a platform to go down.

  ### Development controls
  * F1 Start from the very first level.
  * F2 Start from the beginning of the current level.
  * F3 Increase music volume of game.
  * F4 Decrease music volume of game.
  * F5 Save the current state.
  * F6 Load the previous state (even across levels).
  * F8 Display UI debug colliders.
  * F9 To view colliders.
  * F10 God Mode.
  * F11 Cap on/off fps to 30.
  * W in God Mode to move up.
  * S in God Mode to move down.

## License

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/CarlosUPC/Game_Dev_2DPlatformGame/blob/master/LICENSE) file for details

MIT License

Copyright (c) 2018 CarlosUPC and christt105

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
