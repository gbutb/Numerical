/**
 * shader.cpp
 * ----------
 * 
 * Contains implementation of shader.
 */

#include "shader.hpp"

#include <math.h>
#include <GL/glew.h>

#include <glm/glm/glm.hpp>

int buildVertexShader(const char* vertex_source) {
    int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);

    // Handle errors
    int success;
    char info[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info);
        printf("Error: %s\n", info);
    }

    return vertex_shader;;
}

int buildFragmentShader(const char* fragment_source) {
    int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);

    // Handle errors
    int success;
    char info[512];
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info);
        printf("Error: %s\n", info);
    }
    return fragment_shader;
}

Shader::Shader(const char* vertex_source, const char* fragment_source) {
    // Build shdaers
    int vertex_shader = buildVertexShader(vertex_source);
    int fragment_shader = buildFragmentShader(fragment_source);

    // Create program
    _shader_id = glCreateProgram();

    glAttachShader(_shader_id, vertex_shader);
    glAttachShader(_shader_id, fragment_shader);
    glLinkProgram(_shader_id);

    // Handle error
    int success;
    char info[512];
    glGetProgramiv(_shader_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_shader_id, 512, NULL, info);
        printf("Program error: %s\n", info);
    }

    // Delete shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}


Shader::~Shader() {
    glDeleteProgram(_shader_id);
}

void Shader::use() {
    glUseProgram(_shader_id);
}

void Shader::setBool(const string & name, bool value) const {
    glUniform1i(
        glGetUniformLocation(_shader_id, name.c_str()),
        static_cast<int>(value));
}

void Shader::setInt(const string & name, int value) const {
    glUniform1i(
        glGetUniformLocation(_shader_id, name.c_str()),
        value);
}

void Shader::setFloat(const string & name, float value) const {
    glUniform1f(
        glGetUniformLocation(_shader_id, name.c_str()),
        value);
}

void Shader::setMatrix(const string & name, const float * ptr) const {
    glUniformMatrix4fv(
        glGetUniformLocation(_shader_id, name.c_str()),
        1, GL_FALSE, ptr);
}
