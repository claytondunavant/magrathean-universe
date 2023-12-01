//https://learnopengl.com/Getting-started/Shaders

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
  

class Shader
{
public:
    // the program ID
    unsigned int ID;
  
    // empty init
    Shader() {
        return;
    }

    // constructor reads and builds the shader
    Shader(const char * v_shader_path, const char * f_shader_path) {

        std::string v_code;
        std::string f_code;
        const char * v_shader_code;
        const char * f_shader_code;
        std::fstream v_shader_file;
        std::fstream f_shader_file;
        
        // ensure ifstream objects can throw exceptions
        v_shader_file.exceptions ( std::ifstream::failbit | std::ifstream::badbit);
        f_shader_file.exceptions ( std::ifstream::failbit | std::ifstream::badbit);
        
        try {
            // open files
            v_shader_file.open(v_shader_path);
            f_shader_file.open(f_shader_path);
            std::stringstream v_shader_stream, f_shader_stream;
            // read file's buffer contents into streams
            v_shader_stream << v_shader_file.rdbuf();
            f_shader_stream << f_shader_file.rdbuf();
            // close files
            v_shader_file.close();
            f_shader_file.close();
            // convert stream into string
            v_code = v_shader_stream.str();
            f_code = f_shader_stream.str();
        } 
        catch ( std::ifstream::failure e ) {
            std::cout << "Shader.cpp: File not successfully read" << e.what() << std::endl;
        }
        
        v_shader_code = v_code.c_str();
        f_shader_code = f_code.c_str();
        
        // compile shaders
        unsigned int vertex, fragment;
        int success;
        char info_log[512];
        
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &v_shader_code, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        check_compile_errors(vertex, "VERTEX");

        // fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &f_shader_code, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        check_compile_errors(fragment, "FRAGMENT");

        // shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        check_compile_errors(ID, "PROGRAM");
        
        // delete shaders
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // use/activate the shader
    void use() {
        glUseProgram(ID);
    }
    
    // utility uniform functions
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    } 
    void setMat4(const std::string &name, glm::mat4 value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));        
    }

private:
    void check_compile_errors(unsigned int shader, std::string type)
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
  
#endif