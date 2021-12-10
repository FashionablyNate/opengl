const char * vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main() {\n"
	" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char * fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// callback function for adjusting the viewport when window size is adjusted
void framebufferSizeCallback(GLFWwindow * window, int width, int height);

// function for handling all input
void processInput(GLFWwindow * window);

int main() {

	// initialize GLFW
	glfwInit();
	
	// arg 1 of glfwWindowHint selects option, and arg 2 sets value
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // required for Mac OS X

	// create 800x600 pixel window with header "LearnOpenGL", if statement error checks.
	GLFWwindow * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}

	// make context of window main context of current thread
	glfwMakeContextCurrent(window);

	// initialze glad, and error check
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
 
	// first two args set location of lower left corner.
	// we're telling opengl window size, we can set it lower than window object size
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// call framebufferSizeCallback everytime window is resized
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	/* VERTEX & ELEMENT BUFFER OBJECT */
	// vertex data that defines 3d coordinates of three points
	float vertices[] = {
		 0.5f,  0.5f, 0.0f, // top right
		 0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5,   0.5f, 0.0f  // top left
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// create variable to store unique buffer ID
	unsigned int VBO;
	// generate buffer ID for our Vertex Buffer Object (VBO)
	glGenBuffers(1, &VBO);
	// bind the new buffer to GL_ARRAY_BUFFER which corresponds to the type of VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// from now on buffer calls made on GL_ARRAY_BUFFER configure VBO because it's
	// the currently bound buffer.
	// Now we can copy our vertex data into the buffer's memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// GL_STATIC_DRAW specifies that we're going to write the data once,
	// and it'll be used many times.
	
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* VERTEX SHADER */
	// variable to store ID of shader object
	unsigned int vertexShader;
	// generate and assign a vertex shader object ID to our vertexShader variable
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// here we attach the source code defined at the top of code to our vertex shader
	// object.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// now we compile our shader
	glCompileShader(vertexShader);
	// now we check if compilation was successful
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/* FRAGMENT SHADER */
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/* SHADER PROGRAM */
	// create Shader Program object and store it's ID in shaderProgram variable
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// attach compiled shaders to program and link them
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// now we test to see if linking was successful
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// here we tell opengl how to interpret data we've placed in memory
	// arg1 specifies location of vertex attribute,
	// arg2 specifies the size of the vertex attribute,
	// arg3 specifies the data type and in GLSL vec * contains floats,
	// arg4 will normalize the data if set to true,
	// arg5 defines the stride, how far to the next set of position data,
	// arg6 is the offset of where the position data begins in the buffer.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/* RENDER LOOP START */
	while (!glfwWindowShouldClose(window)) {
		// guess what we do here :) (tell the window to do something on user input)
		processInput(window);

		/* RENDERING COMMANDS START */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // fill color buffer with greenish blue
		glClear(GL_COLOR_BUFFER_BIT); // clear color buffer

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		/* RENDERING COMMANDS END */

		// swaps the color buffer, and shows it as output to the screen
		glfwSwapBuffers(window);
		// checks for events being triggered, and calls appropriate callback function
		glfwPollEvents();
	}
	/* RENDER LOOP END */

	glfwTerminate(); // free resources
	return 0;
}

void framebufferSizeCallback(GLFWwindow * window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window) {
	// if the user presses escape, close the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}	
