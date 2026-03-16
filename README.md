# GLRenderer

## Requisiti:
- [GLFW](https://www.glfw.org/) (OpenGL versione 3.3)
- [glad](http://glad.dav1d.de/) (Versione 1.0)
- [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
- [GLM](https://github.com/g-truc/glm)

## Risorse
- Texture
  - [AwesomeFace](https://learnopengl.com/img/textures/awesomeface.png)
  - [Container](https://learnopengl.com/img/textures/container.jpg)

## Come compilare
- Creare e copiare nella cartella libraries le librerie GLFW e glad. 
- Per stb_image, creare una cartella "include" per il file .h e una cartella "src". Creare il file stb_image.cpp all'interno della cartella src, con il seguente contenuto
```c++
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
```
- Installare glm con Vcpkg
