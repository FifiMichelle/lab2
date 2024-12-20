#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glad/gl.h>

#include <string>
#include <vector>
#include <iostream>

// Texture structure to store texture data
struct Texture {
    unsigned int id;
    std::string type;
    std::string path; // Useful for debugging or avoiding duplicate loads
};

// Vertex structure to store attributes
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

class Shader; // Forward declaration of the Shader class, if used

class Mesh {
public:
    // Mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    // Draw the mesh
    void Draw(GLuint shaderProgram); // For your current system using GLuint program
    void Draw(const Shader& shader); // Optional: For Shader class if implemented

private:
    // Render data
    unsigned int VAO, VBO, EBO;

    // Initialize all the buffer objects/arrays
    void setupMesh();
};

#endif
