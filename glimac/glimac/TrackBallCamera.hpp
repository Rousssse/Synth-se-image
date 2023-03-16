#pragma once
#include <vector>
#include "glm.hpp"
namespace glimac {
class TrackballCamera {
    private :
    float m_fDistance;
    float m_fAngleX;
    float m_fAngleY;

    public : 
    TrackballCamera():m_fDistance(5.0f),m_fAngleX(0.0f),m_fAngleY(0.0f){};
    TrackballCamera(float distance, float anglex, float angley):m_fDistance(distance), m_fAngleX(anglex), m_fAngleY(angley){};

    void moveFront(float delta);
    void rotateLeft(float degrees);
    void rotateUp(float degrees);
    glm::mat4 getViewMatrix() const;


};

void TrackballCamera::moveFront(float delta){
    m_fDistance += delta;
    
}

void TrackballCamera::rotateLeft(float degrees){
    m_fAngleY += degrees;
}

void TrackballCamera::rotateUp(float degrees){
    m_fAngleX += degrees;
}

glm::mat4 TrackballCamera::getViewMatrix() const{
    glm::mat4 MVMatrix;
    MVMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0,0,-this->m_fDistance));
    MVMatrix = glm::rotate(MVMatrix, glm::radians(-this->m_fAngleX), glm::vec3(1,0,0));
    MVMatrix = glm::rotate(MVMatrix, glm::radians(-this->m_fAngleY), glm::vec3(0,1,0));
    return MVMatrix;
}
}