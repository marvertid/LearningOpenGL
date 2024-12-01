#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "utils.h"
#include "shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/scalar_multiplication.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "model.h"
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //settiamo la verisone di opengl max a 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //settiamo la verisone di opengl min a 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //diciamo che come profile di opengl vogliamo usare il core_profile

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FirstOpenGLWindow", NULL, NULL); //creiamo l'oggetto window
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


// ----------------------------------------------------------------------------------------------------------------------------

	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	Shader shader("./vertexShader.vs", "./fragmentShader.fs");
	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	Model MeshModel("./Resources/backpack/backpack.obj");

	//camera mouse movement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	camera.MovementSpeed = 3.3f;

	float currentFrame = 0.0f;

	//test
	glfwSetMouseButtonCallback(window, mouseButton_callback);
	glfwSetKeyCallback(window, key_callback);
	
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


	while (!glfwWindowShouldClose(window)) { //while loop che si ripete in continuazione durante l'esecuzione della finestra

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processMovement(window, camera, deltaTime);
		glClearColor(0.08, 0.6, 0.34, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shader.use();

		fpsCounter(nbFrames, lastTime);
		

		// don't forget to enable shader before setting uniforms
		shader.use();

		// view/projection transformations
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		// render the loaded model
		model = glm::rotate(model, glm::radians(90.0f) * deltaTime, glm::vec3(0.0, 1.0, 0.0));
		shader.setMat4("model", model);
		MeshModel.Draw(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == 0 && action == 1 && glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_ESCAPE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(window, NULL);
	}
}