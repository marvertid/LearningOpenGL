#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/scalar_multiplication.hpp>
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processMovement(GLFWwindow* window, Camera& camera, const float& deltaTime);
void changeColor(unsigned int shaderProgram);
void fpsCounter(int &nbFrames, double &lastTime);
#endif
