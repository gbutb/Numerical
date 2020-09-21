/**
 * camera.hpp
 * ----------
 * 
 * Defines a general camera.
 */

#ifndef WINDOW_CAMERA_HPP_
#define WINDOW_CAMERA_HPP_

#include <math.h>
#include <memory>
using std::shared_ptr;

// For matrices
#include <glm/glm/glm.hpp>
#include <glm/glm/ext/matrix_transform.hpp>
#include <glm/glm/ext/matrix_clip_space.hpp>

class Camera {
 private:
    bool _enabled = true;
    double _yaw = 0, _pitch = 0;
    double _x = 0, _y = 0, _z = 0;

    // Camera matrices
    glm::mat4 _model;
    glm::mat4 _projection;

 public:
    Camera(int width, int height);
    virtual ~Camera() = default;

    void enable();
    void disable();

    void rotate(double diff_yaw, double diff_pitch);

    void translate(glm::vec3 translation);

    void reset();

    /////////////
    // Getters //
    ///////////// 

    glm::mat4 getModel() const;
    glm::mat4 getProjection() const;
};

typedef shared_ptr<Camera> ptr_Camera;

#endif  // WINDOW_CAMERA_HPP_
