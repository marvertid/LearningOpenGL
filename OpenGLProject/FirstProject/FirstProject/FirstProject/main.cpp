#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "utils.h"

using namespace std;

int main() {

	unsigned int VBO; //oggetto vertex buffer, che ci permette di mandare tutti i vertici dalla cpu al gpu, notare che e` preribile mandare poche volte ma mandare tante informazioni
						// perche il trasferimento di dati e` lento
	unsigned int VAO; //oggetto vertex array, 
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	int success;
	char infoLog[512];

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //settiamo la verisone di opengl max a 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //settiamo la verisone di opengl min a 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //diciamo che come profile di opengl vogliamo usare il core_profile

	GLFWwindow* window = glfwCreateWindow(800, 600, "FirstOpenGLWindow", NULL, NULL); //creiamo l'oggetto window
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

	float vertices[] = { //array che contiene i vertici del triangolo che andremo a disegnare sullo schermo.
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	
	/*BUFFER HANDLING*/

	glGenBuffers(1, &VBO); //generiamo l'identificativo del buffer
	glGenVertexArrays(1, &VAO); //generiamo l'indentificativo per il VAO

	glBindVertexArray(VAO); //bindiamo il VAO al VertexArray
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bindiamo "VBO" al buffer di tipo GL_ARRAY_BUFFER.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
	/*
	questa funzione e` fatta apposta per copiare dei dati definiti dall'utente all'interno del buffer della scheda video
	quindi come primo parametro gli passiamo il tipo di buffer, come secondo la grandezza del dato che vogliamo passargli, il terzo e` il dato
	il quarto invece specifica come la scheda video deve gestire questi dati, nel nostro caso: GL_STATIC_DRAW perche verra settato una volta sola
	ma usato tante volte
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	/*VERTEX SHADER*/

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //creiamo l'id per la nostra shader.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //impostiamo il sorgente della nostra shader
	glCompileShader(vertexShader); //compiliamo la shader

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //andiamo ad impostare un valore alla variabile success, dato in base al fatto che la compilazione sia riuscita o meno
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); //imposta il log della shader alla variabile infoLog
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	/*FRAGMENT SHADER*/

	//stesso processo della vertex shader

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); 
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); 
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	/*SHADER PROGRAM*/

	unsigned int shaderProgram = glCreateProgram(); //creiamo lo shaderProgram
	glAttachShader(shaderProgram, vertexShader); //attacchiamo la nostra vertex shader allo shader program, idem per la fragment
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); // e linkiamole

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}
	glDeleteShader(vertexShader); //cancelliamo le shader dato che le abbiamo gia linkate allo shaderprogram
	glDeleteShader(fragmentShader);


	
	while (!glfwWindowShouldClose(window)) { //while loop che si ripete in continuazione durante l'esecuzione della finestra

		processInput(window);

		glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);


	glfwTerminate();
	return 0;
}