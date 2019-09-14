#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>  


class Shader {
    
public:
    GLuint ID;
    Shader(const char* vertexPath, const char* fragmentPath){       
        std::string vShaderCode = readShaderFile(vertexPath);
        std::string fShaderCode = readShaderFile(fragmentPath);
        const char* vShaderCodeCstr = vShaderCode.c_str();
        const char* fShaderCodeCstr = fShaderCode.c_str();

        GLuint vertex = compileVertexShader(vShaderCodeCstr);
        GLuint fragment = compileFragmentShader(fShaderCodeCstr);
        linkProgramAndDeleteShaders(vertex, fragment);
    }

    void use() { 
        glUseProgram(ID); 
    }

    void setBool(const std::string &name, bool value) const {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }

    void setInt(const std::string &name, int value) const { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }

    void setFloat(const std::string &name, float value) const { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }

private:

    GLuint compileVertexShader(const char * vShaderCodeCstr){
        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCodeCstr, NULL);
        glCompileShader(vertex);
        checkCompilerErrorsShader(vertex, "VERTEX");
        return vertex;
    }

    GLuint compileFragmentShader(const char * fShaderCodeCstr){
        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCodeCstr, NULL);
        glCompileShader(fragment);
        checkCompilerErrorsShader(fragment, "FRAGMENT");
        return fragment;
    }    

    void linkProgramAndDeleteShaders(GLuint vertex, GLuint fragment){
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompilerErrorsProgram(ID);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    
    std::string readShaderFile(const char* path){
        std::ifstream shaderFile;
        std::stringstream shaderStream;
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            shaderFile.open(path);
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();
        } catch (const std::exception &e){
            throw std::runtime_error(std::string("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"));
        }
        if(shaderStream.str() == ""){
            throw std::runtime_error(std::string("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ_EMPTY"));
        }
        return shaderStream.str();
    }

    void checkCompilerErrorsShader(GLuint shader, std::string type){
        int success;
        char infoLog[1024];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            throw std::runtime_error(std::string("ERROR::SHADER_COMPILATION_ERROR of type: ") + type);
        }
    }
    
    void checkCompilerErrorsProgram(GLuint shader){
        int success;
        char infoLog[1024];
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success){
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            throw std::runtime_error(std::string("ERROR::PROGRAM_LINKING_ERROR"));
        }
    }
};
#endif