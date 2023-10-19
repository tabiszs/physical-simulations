#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    Shader(const char* vertexPath, const char* fragmentPath, const char* tessControlPath, const char* tessEvaluationPath);
    void use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void set4Float(const std::string& name, GLfloat* value) const;
    void set3Float(const std::string& name, glm::vec3 value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setMatrix4F(const std::string& name, glm::mat4 value) const;
    void setTextureAtUnit(const std::string& name, int value) const;
    void activateTexture(int i, int texture) const;
private:
    
    void checkCompileErrors(unsigned int shader, std::string type);
};
#endif

