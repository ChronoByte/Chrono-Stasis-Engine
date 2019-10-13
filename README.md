# Chrono-Stasis-Engine

Chrono Stasis Engine is a 3D game engine created by two students in the third year of Videogame Design and Development degree at CITM UPC from Terrassa, Spain. 

## Terminology & Origin
As his name says, Chrono Stasis comes from the greek term **Chronostasis** (*chrónos* "Time" and *stásis* "standing"), a temporal illusion in which the first impression following the introduction of a new event or task-demand to the brain can appear to be extended in time. 

That is to say, is an illusion where that person perceives an instance of time as an extremely long or even infinite state of time, hence the word "standing time." Therefore, we can affirm that it is an allegory in the computational world and videogames, where for a user or player an instance of time is a millisecond of time spent playing, whereas for us it is an instance where millions of instructions, operations and calculations happen internally in the engine. 

Metaphorically for us (software engineers and game developers), we live in a chronostasis loop measuring and observing the behavior and time performance in each frame at all the time until the game ends. In addition, we decide to make Chrono Stasis as compound word because it contains the acronym of our names: **C**hrono (Carlos) & **S**tasis (Sebas).

## Features
* Configuration & Model Windows (App, Hardware, renderer,...)
* Load / Save editor configuration with JSON Parser
* Geometry Creation (Cube, Sphere, ...)
* GameObject and Component Design Pattern 
* Model and Texture Loader with Drag & Drop functionality
* Geometry Editors (Hierarchy, Inspector)
* Camera Controlling (Zoom in, zoom out, orbitation,..)

## Innovation System
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque eleifend velit id sodales viverra. Maecenas interdum lorem dolor, congue lobortis mi molestie tincidunt. Lorem ipsum dolor sit amet, consectetur adipiscing elit. 

## Installation
* Go to our github release page: [Click Here!](https://github.com/ChronoByte/Chrono-Stasis-Engine/releases)
* Download the last version **.zip**
* Extract .zip files inside a folder 
* Execute **.exe** file

## Authors
* Carlos Peña [CarlosUPC](https://github.com/CarlosUPC)
* Sebastián López [Sebi Lopez](https://github.com/Sebi-Lopez)

## Programs & Dependencies used
* [Microsoft Visual Studio 2019](https://visualstudio.microsoft.com/es/vs/)
* [OpenGL3](https://opengl.org/)
* [SDL v2.0](https://www.libsdl.org/license.php)
* [STL](https://docs.oracle.com/database/bdb181/html/installation/win_build_stl.html)
* [MathGeoLib v1.5](https://github.com/juj/MathGeoLib)
* [PCG v0.9](http://www.pcg-random.org/)
* [Mmgr](http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml)
* [ImGui](https://github.com/ocornut/imgui)
* [JSON parser v3.7](https://github.com/nlohmann/json)
* [Glew v2](https://github.com/nigels-com/glew)
* [par_shape.h](https://prideout.net/shapes)
* [Assimp v4.1](http://www.assimp.org/) 
* [DevIL v1.8](http://openil.sourceforge.net/) 
* [GPU detect v1.1](https://github.com/MatthewKing/DeviceId)

## Repository Link

* 3D Engine Engine Repository: [Chrono Stasis Engine](https://github.com/ChronoByte/Chrono-Stasis-Engine)
* Our Organization: [ChronoByte](https://github.com/ChronoByte)

## Unity-Like Camera Controls
* **WASD:** fps-like movement and free look around
* **Mouse Right:** Move Camera with static position
* **Mouse Left + Alt:** Move Camera around object (Orbit)
* **Mouse wheel:** Move Camera Zoom in and Zoom out
* **F**: Center the Camera on an object
* **SHIFT:** Duplicates movement speed.

## Updates & Changelog

* 0.3.0
     * Added Basic GameObject  Structure 
     * Reestructured Model Importer 
     * Added Inspector Window
     * Component Design Pattern 
        > Transform, Material, Mesh

* 0.2.5
    * Implemented DevIL 
     * Texture Loader
     * Drag & Drop funcionality (to import DDS textures).


* 0.2.0
    * Implemented Assimp  
    * FBX Model Loader
       > Draw vertices, indices, normals.
    * Drag & Drop funcionality (to import FBX models).


* 0.1.7
     * Add ImGui::Dock 
     * 3D Geometry Creation
       > Vertex Buffers, Index Buffers, Par shapes
     
      
* 0.1.5
   * Implemented JSON Parser
      > Load / Save editor configuration
   * Implemented OpenGL Context with GLEW (init & renderer)
      > Initialization, Renderer, Shutdown 
   * Implemented "Render" options inside "Configuration" window
      > Depth test, lightning, color material, wireframe, axis, smooth polygons and fog mode.
    * Congifuration Window development
      > OpenGL + GLEW info.

* 0.1.0
    * Implemented "Configuration" Window:
      >Frame rate options. Window options (resizable, fullscreen, etc). Renderer options. Hardware info 
    * Added "About us..." Modal Window:
      >Name, description, libraries used.
    * "Console" Window:
      >Logs & Engine inner performance.
      
 * 0.0.5
   * Implemented MathGeoLib Framework
     > Cube, Sphere, Cilinders, Frustum, AABB, etc
   * Implemented PCG Pseudo-Random Helper
     > Float between 0.0 and 1.0. 
     > Int between any min/max
## License

      MIT License

      Copyright (c) 2019 ChronoByte

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

