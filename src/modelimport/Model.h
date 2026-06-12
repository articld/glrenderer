#pragma once

#include <vector>
#include <iostream>

#include<glad/glad.h>
#include<glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>
#include <filesystem>

#include "Mesh.h"
#include "../Shader.h"

class Model {
    public:
        Model(const char *path) {
            if (!loadModel(path)) {
                std::terminate();
            }
        }
        void Draw(Shader &shader);
    private:
        const aiScene* modelscene;
        std::vector<Mesh> meshes;
        std::string directory;
        std::vector<Texture> textures_loaded;

        bool loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
