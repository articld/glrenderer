#include "Camera.h"

void Camera::processKeyboard(Camera_Movement direction, float deltaTime) {
    const float cameraSpeed = movementSpeed * deltaTime;
    if (direction == FORWARD)
        position += cameraSpeed * front;
    if (direction == BACKWARD)
        position -= cameraSpeed * front;
    if (direction == LEFT)
        position -= right * cameraSpeed;
    if (direction == RIGHT)
        position += right * cameraSpeed;
}
void Camera::processMouse(float xoffset, float yoffset, bool constrainPitch = true) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;
    //limiti per evitare che l'utente riesca a guardare più alto di 89 gradi o più in basso di -89 gradi
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        else if (pitch < -89.0f)
            pitch = -89.0f;
    }
    updateCameraVectors();
}

void Camera::processScroll(float yoffset) {
    FOV -= (float)yoffset;
    if (FOV < 1.0f)
        FOV = 1.0f;
    if (FOV > 45.0f)
        FOV = 45.0f;
}

glm::mat4 Camera::getViewMatrix() const {
    glm::mat4 view = glm::mat4(1.0f);
    return view = glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getPerspectiveMatrix() const {
    glm::mat4 projection = glm::mat4(1.0f);
    return projection = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);
}

void Camera::updateCameraVectors() {
    glm::vec3 direction;
    direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    direction.y = glm::sin(glm::radians(pitch));
    direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front = glm::normalize(direction);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

glm::vec3 Camera::getPosition() const {
    return position;
}
