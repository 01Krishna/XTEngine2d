#pragma once 
#include <glad/glad.h>
#include "Core.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>


namespace XTEngine2d
{
    class XT_API Shader
    {
    public:
        unsigned int ID;
		Shader() = default;
        Shader(std::string vertexPath, std::string fragmentPath);
        void use();

        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setMat4(const std::string& name, float* value) const;
        void setVec4(const std::string& name, float* value) const;

    private:
        std::string vertexShaderPath, fragmentShaderpath;
    private:

        void checkCompileErrors(unsigned int shader, std::string type)
        {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM")
            {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                    //std::cout << "ShaderPath: " << vertexShaderPath << " : " << fragmentShaderpath << std::endl;
                }
            }
            else
            {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
        }
    };
};