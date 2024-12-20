#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <iostream>
#include <glad/gl.h> // For OpenGL functions like glGenTextures, glBindTexture


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

// Declare the TextureFromFile function prototype above the Model class
unsigned int TextureFromFile(const char* path, const std::string& directory);

class Model {
public:
    // Constructor: loads a model from a file
    Model(const std::string& path) {
        loadModel(path);
    }

    // Draws the model
    void Draw(GLuint shaderProgram) {
        for (auto& mesh : meshes) {
            mesh.Draw(shaderProgram);
        }
    }

    // Optional: Draws the model using a Shader class
    void Draw(const Shader& shader) {
        for (auto& mesh : meshes) {
            mesh.Draw(shader);
        }
    }

private:
    std::vector<Mesh> meshes;   // List of meshes in the model
    std::string directory;      // Directory of the model
    std::vector<Texture> texturesLoaded; // Caches loaded textures

    // Loads a model from a file
    void loadModel(const std::string& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
    }

    // Processes a node recursively
    void processNode(aiNode* node, const aiScene* scene) {
        // Process all the meshes in the node
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        // Recursively process child nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    // Processes a single mesh
    Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // Process vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;

            // Positions
            vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

            // Normals
            vertex.Normal = mesh->mNormals ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) : glm::vec3(0.0f);

            // Texture coordinates
            if (mesh->mTextureCoords[0]) { // Check if texture coordinates exist
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            } else {
                vertex.TexCoords = glm::vec2(0.0f);
            }

            // Tangents
            vertex.Tangent = mesh->mTangents ? glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z) : glm::vec3(0.0f);

            // Bitangents
            vertex.Bitangent = mesh->mBitangents ? glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z) : glm::vec3(0.0f);

            vertices.push_back(vertex);
        }

        // Process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        // Process materials
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            // Diffuse textures
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // Specular textures
            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(vertices, indices, textures);
    }

    // Loads all textures of a specific type from a material
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) {
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);

            // Check if texture was already loaded
            bool alreadyLoaded = false;
            for (const auto& loadedTexture : texturesLoaded) {
                if (std::strcmp(loadedTexture.path.c_str(), str.C_Str()) == 0) {
                    textures.push_back(loadedTexture);
                    alreadyLoaded = true;
                    break;
                }
            }

            if (!alreadyLoaded) {
                // Load texture
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                texturesLoaded.push_back(texture); // Add to loaded textures
            }
        }

        return textures;
    }
};

// Define the TextureFromFile function after the Model class
unsigned int TextureFromFile(const char* path, const std::string& directory);


#endif
