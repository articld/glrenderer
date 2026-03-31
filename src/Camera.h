#pragma once

#include<glm/glm.hpp>
#include<glm/ext.hpp>
#include <glad/glad.h>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
    private:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 right;
        glm::vec3 worldUp;
        glm::vec3 up;

        float yaw;
        float pitch;
        float FOV;
        float aspectRatio;

        //per ora const, potrei cambiarlo se voglio modificare l'impostazione
        const float movementSpeed = 4.0f;
        const float sensitivity = 0.1f;

        void updateCameraVectors();

    public:
        Camera(glm::vec3 newPosition = glm::vec3(0.0f, 0.0f, 3.0f)
              , glm::vec3 newFront = glm::vec3(0.0f, 0.0f, -1.0f)
              , glm::vec3 newWorldUp = glm::vec3(0.0f, 1.0f, 0.0f)
              , float newAspectRatio = 4.0f/3.0f
              , float newYaw = -90.0f
              , float newPitch = 0.0f
              , float newFOV = 45.0f)
        {
            position = newPosition;
            front = newFront;
            worldUp = newWorldUp;
            yaw = newYaw;
            pitch = newPitch;
            aspectRatio = newAspectRatio;
            FOV = newFOV;
            updateCameraVectors();
        }

        void processKeyboard(Camera_Movement direction, float deltaTime);
        void processMouse(float xoffset, float yoffset, bool constrainPitch);
        void processScroll(float yoffset);
        glm::vec3 getPosition() const;
        glm::vec3 getFront() const;
        glm::mat4 getPerspectiveMatrix() const;
        glm::mat4 getViewMatrix() const;
};

