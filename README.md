# YORO - You Only Render Once

## Requisiti:
- [GLFW](https://www.glfw.org/) (OpenGL versione 3.3)
- [glad](http://glad.dav1d.de/) (Versione 1.0)
- [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
- [GLM](https://github.com/g-truc/glm)
 
## Come compilare
- Creare e copiare nella cartella libraries le librerie GLFW e glad. 
- Per stb_image e write, creare una cartella "include" per il file .h e una cartella "src". Creare il file stb_image.cpp all'interno della cartella src, con il seguente contenuto
```c++
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"
```
- Installare glm con Vcpkg
- Installare assimp con Vcpkg

## Utilizzo
- Avviare il programma con argomenti di ingresso path_modello extent_x extent_y extent_z
- Oppure, avviare il programma main.py per fare il rendering in batch