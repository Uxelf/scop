# Scop
This project consist in making use of a graphic API (openGL in this case) and render some objects in obj format.

https://github.com/user-attachments/assets/0cc03a02-6288-447c-a6b6-c7ee1aa34ec6


## Instructions
Download the project and execute `make` inside the folder
You can execute the program with `./scop [objpath.obj]`
One argument is mandatory, but you can put multiple objects (or the same one more than once)
Example:

    ./scop resources/42.obj resources/teapot.obj


## Controls
Similar to any other first person camera
**WASD** - Move arround  
**Mouse** - Rotate camera  
**Space** - Go up  
**Shift** - Go down  
**1** - Toggle texture  
**2** - Toggle light movement  

## Obj files
Currently, the following data is supported:

 - **v** - vertex
 - **f** - faces (more than 3 vertices are supported)
 - **vn** - vertices normals
 - **vt** - vertices textures

