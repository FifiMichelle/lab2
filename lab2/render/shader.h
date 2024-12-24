#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    GLuint programID; // Program ID
    GLuint vertexShaderID, fragmentShaderID;

    // Constructor: compiles and links shaders
    Shader(const char* vertexPath, const char* fragmentPath) {
        // Load and compile shaders
        vertexShaderID = loadShader(vertexPath, GL_VERTEX_SHADER);
        fragmentShaderID = loadShader(fragmentPath, GL_FRAGMENT_SHADER);

        // Link shaders into program
        programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);

        // Check for linking errors
        GLint success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(programID, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // Clean up shader objects after linking
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }

    // Destructor: Clean up the shader program
    ~Shader() {
        glDeleteProgram(programID);
    }

    // Use the shader program (bind it)
    void use() const {
        glUseProgram(programID);
    }

    // Set a 4x4 matrix uniform (MVP, for example)
    void setUniformMatrix4fv(const std::string& name, const glm::mat4& matrix) const {
        GLint location = glGetUniformLocation(programID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
    }

    // Set an integer uniform
    void setUniform1i(const std::string& name, GLint value) const {
        GLint location = glGetUniformLocation(programID, name.c_str());
        glUniform1i(location, value);
    }

    // Set a float uniform
    void setUniform1f(const std::string& name, GLfloat value) const {
        GLint location = glGetUniformLocation(programID, name.c_str());
        glUniform1f(location, value);
    }

    // Set a vec3 uniform
    void setUniform3f(const std::string& name, const glm::vec3& value) const {
        GLint location = glGetUniformLocation(programID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void use() 
    { 
        glUseProgram(programID); 
    }

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(programID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(programID, name.c_str()), value); 
    }
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(programID, name.c_str()), x, y, z); 
    }
     
    



private:
    // Helper function to load and compile a shader
    GLuint loadShader(const char* path, GLenum shaderType) {
        std::string shaderCode;
        std::ifstream shaderFile(path);
        if (shaderFile.is_open()) {
            std::stringstream buffer;
            buffer << shaderFile.rdbuf();
            shaderCode = buffer.str();
        }
        shaderFile.close();

        const char* shaderCodeCStr = shaderCode.c_str();
        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderCodeCStr, NULL);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        return shader;
    }
};

GLuint LoadShadersFromFile(const char *vertex_file_path, const char *fragment_file_path);
GLuint LoadShadersFromString(std::string VertexShaderCode, std::string FragmentShaderCode);


#endif // SHADER_H
