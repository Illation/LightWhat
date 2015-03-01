# LightWhat

A small raytracing project that might one day render beautiful images :3

LightWhat version 0.4.2

by Robert Lindner 

using SDL2, Assimp and tinyxml2 => find full licenses under acknowledgments.txt

to run start LightWhat.exe

* press "L" to load a collada (*.dae) file 
* press "T" to load a test scene with spheres and a plane 
* press "C" to clear the current scene 
* press "ENTER" to start rendering 
* press "S" to save your result as a bitmap (*.bmp) 
* press "P" to toggle between high performance mode and progress viewing mode 
* press "B" to toggle backface culling on and off

note: scenes do not clear automatically, so before you load a different scene or file, press "C" to clear it

* you can try the models from the TestFiles folder. "Whirlwind.dae" will probably crash the application 
* you can also try rendering your own models by exporting them in collada format.  
* You might want to position your models so that they are visible from the camera as there is no way of changing its position at the moment 
* ->Camera position: (0, 0, 5) pointing down the Z Axis (0, 0, -1) (right handed coordinate system, Y is up)

Exporting: 
Only tested with files generated with the blender exporter: 
* check the triangulate option 
* dont use textures yet

if you find any bugs tell me about them please

Have fun!
