#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include "shader_s.h"

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
	
	// build and compile shader program
	Shader ourShader("src/shader.vs", "src/shader.fs");
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	/* VERTEX & ELEMENT BUFFER OBJECT */
	// vertex data that defines 3d coordinates of three points
	float vertices[] = {
		 // Position        // Color
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // top
	};
	/*
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	*/

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/* RENDER LOOP START */
	while (!glfwWindowShouldClose(window)) {
		// guess what we do here :) (tell the window to do something on user input)
		processInput(window);

		/* RENDERING COMMANDS START */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // fill color buffer with greenish blue
		glClear(GL_COLOR_BUFFER_BIT); // clear color buffer

		ourShader.use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);
		/* RENDERING COMMANDS END */

		// swaps the color buffer, and shows it as output to the screen
		glfwSwapBuffers(window);
		// checks for events being triggered, and calls appropriate callback function
		glfwPollEvents();
	}
	/* RENDER LOOP END */

	glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

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
