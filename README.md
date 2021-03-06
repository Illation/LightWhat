# LightWhat

## Description
A growing raytracing project that might one day render beautiful images :3

LightWhat version 0.7.2 alpha

by [Robert Lindner](https://www.artstation.com/artist/illation)

## Acknowledgments
Using SDL2, SDL_TFF (with freetype), Assimp, devIL => find full licenses under acknowledgments.txt

While I wrote all the code myself, the structure of the Bottom Layer is heavily inspired by the
[Digital Arts and Entertainment](http://www.digitalartsandentertainment.be/) GAME_ENGINE

## Key bindings
* press "L" to load a collada (.dae) file
* press "T" to load a test scene with spheres and a plane
* press "C" to clear the current scene
* press "ENTER" / "RETURN" to start rendering
* press "S" to save your result as a bitmap (.bmp)
* press "P" to toggle between high performance mode and progress viewing mode
* press "B" to toggle backface culling on and off
* press "D" to display the first texture in the list, or a test texture if no images where loaded

There is also button support now :smiling_imp:

## How to use
Note: scenes do not clear automatically, so before you load a different scene or file, press "Clear" to clear it

* you can try the models from the "TestFiles" folder.
* you can also try rendering your own models by exporting them in Collada or 3Ds format.  
* you can edit the '.lwtf' file in any text editor to change render settings and camera position

## About the structure of the code:
There are three layers to allow for easy changes:
* Bottom layer: ProgramControll with Init, Tick and Paint handles an AbstractProgram and Provides Window and Painting operations via a GUI_ENGINE singleton
* Middle layer: LightWhat base that handles a concret UserInterface and manages the renderer
* Top layer: LWRenderer handles all Rendering tasks and provides an interface for managing it and getting the results

If you find any bugs tell me about them please

Have fun!
