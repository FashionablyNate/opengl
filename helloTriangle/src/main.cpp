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

	/* RENDER LOOP START */
	while (!glfwWindowShouldClose(window)) {
		// guess what we do here :) (tell the window to do something on user input)
		processInput(window);

		/* RENDERING COMMANDS START */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // fill color buffer with greenish blue
		glClear(GL_COLOR_BUFFER_BIT); // clear color buffer
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
