#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "utils.h"

using namespace std;

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //settiamo la verisone di opengl max a 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //settiamo la verisone di opengl min a 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //diciamo che come profile di opengl vogliamo usare il core_profile

	GLFWwindow* window = glfwCreateWindow(800, 600, "FirtOpenGLWindow", NULL, NULL); //creiamo l'oggetto window
	if (window == NULL) { //controlliamo che sia allocato
		cout << "Failed to create window!" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //lo impostiamo sul thread corrente

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { //controlliamo che GLAD sia inizializzato
		cout << "Failed to initialize GLAD :(" << endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //quando la finestra viene ridemensionata chiamiamo la funzione di callback

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}