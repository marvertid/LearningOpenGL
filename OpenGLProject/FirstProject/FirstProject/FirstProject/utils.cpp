#include "utils.h"
#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { 
	glViewport(0, 0, width, height); //aggiusta il viewport con quello desiderato
}


void changeColor(unsigned int shaderProgram) {
	float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	glUseProgram(shaderProgram);
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
}

void processMovement(GLFWwindow* window, Camera& camera, const float& deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void fpsCounter(int &nbFrames, double &lastTime) {
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
		// printf and reset timer
		printf("%f ms/frame\n", 1000.0 / double(nbFrames));
		cout << 1000 / (1000.0 / double(nbFrames)) << endl;
		nbFrames = 0;
		lastTime += 1.0;
	}
}
