/**
 * camera.cpp
 * ----------
 * 
 * Implements camera.
 */

#include "camera.hpp"

Camera::Camera(int width, int height) {
    // Initialize projection matrix
    _projection = glm::perspective<float>(
        glm::radians(45.0f),
        width / static_cast<float>(height), .1f, 100.0f);
}

void Camera::enable() {
    _enabled = true;
}

void Camera::disable() {
    _enabled = false;
}

void Camera::rotate(double diff_yaw, double diff_pitch) {
    if (!_enabled) return;

    _yaw -= diff_yaw;
    _pitch += diff_pitch;
}

void Camera::translate(glm::vec3 translation) {
    if (!_enabled) return;

    // Get camera orientation
    glm::mat4 orientation = glm::mat4(1.0);
    orientation = glm::rotate<float>(
        orientation, _pitch * glm::pi<float>() / 180.0, glm::vec3(1, 0, 0));
    orientation = glm::rotate<float>(
        orientation, _yaw * glm::pi<float>() / 180.0, glm::vec3(0, -1, 0));

    // Extend translation to RP3
    glm::vec4 translation_ext = glm::vec4(translation, 1.0);
    translation_ext = -glm::inverse(orientation) * translation_ext;

    // Translate the camera
    _x += translation_ext.x;
    _y += translation_ext.y;
    _z += translation_ext.z;
}

void Camera::reset() {
    _x = _y = _z = 0;
    _yaw = _pitch = 0;
}

glm::mat4 Camera::getModel() const {
    // Get camera orientation
    glm::mat4 orientation = glm::mat4(1.0);
    orientation = glm::rotate<float>(
        orientation, _pitch * glm::pi<float>() / 180.0, glm::vec3(1, 0, 0));
    orientation = glm::rotate<float>(
        orientation, _yaw * glm::pi<float>() / 180.0, glm::vec3(0, -1, 0));

    // Get camera translation
    glm::mat4 translation = glm::mat4(1.0);
    translation = glm::translate(
        translation, glm::vec3(_x, _y, _z));

    return orientation * translation * glm::mat4(1.0);
}

glm::mat4 Camera::getProjection() const {
    return _projection;
}
