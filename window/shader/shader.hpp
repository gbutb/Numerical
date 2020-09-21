/**
 * shader.hpp
 * ----------
 * 
 * Contains code for shader.
 */

#ifndef WINDOW_SHADER_HPP_
#define WINDOW_SHADER_HPP_

#include <memory>
using std::shared_ptr;

#include <string>
using std::string;

class Shader {
 private:
    unsigned int _shader_id;

 public:
    Shader(const char* vertex_source, const char* fragment_source);
    ~Shader();

    /**
     * Same as glUseProgram()
     */
    void use();

    /**
     * Renders shader;
     */
    virtual void render() {}

    /////////////
    // Setters //
    /////////////

    void setBool(const string& name, bool value) const;
    void setInt(const string& name, int value) const;
    void setFloat(const string& name, float value) const;
    void setMatrix(const string& name, const float* ptr) const;
};

// Shader pointer to Shader
typedef shared_ptr<Shader> ptr_Shader;

#endif  // WINDOW_SHADER_HPP_
