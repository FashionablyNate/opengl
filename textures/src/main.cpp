#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "../include/glad.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include "../include/shader_s.h"
#include "../include/stb_image.h"

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
	
	/* VERTEX & ELEMENT BUFFER OBJECT */
	// vertex data that defines 3d coordinates of three points
	float vertices[] = {
		 // Position         // Color           // texture coords
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // top left
	};

	// define indices of triangles that make up the rectangle
	float indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// create variables to store unique buffer IDs
	// generate buffer IDs
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
 	glEnableVertexAttribArray(2);
	
	// texture generation
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// specifies how textures should handle coordinates outside their range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// specifies how texture filtering should work
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int width, height, nrChannels;
	unsigned char * data = stbi_load("img/container.jpg", &width, &height, &nrChannels, 0);

	if (data) {
		// arg1, specifies texture target, in this case the currently bound 2D texure
		// arg2, specifies mipmap level we want to target
		// arg3, tells opengl in what format we want to store the texture
		// arg4 and arg5, set the width and height of the texture
		// arg6, must always be set to 0 (legacy stuff so I'm told)
		// arg7 and arg8, specify the format and datatype of the source image
		// ar9, actual image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// specifies how textures should handle coordinates outside their range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// specifies how texture filtering should work
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	data = stbi_load("img/awesomeface.png", &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}

	// free memory
	stbi_image_free(data);
	
	ourShader.use();
	ourShader.setInt("texture1", 1);
	ourShader.setInt("texture2", 1);

	/* RENDER LOOP START */
	while (!glfwWindowShouldClose(window)) {
		// guess what we do here :) (tell the window to do something on user input)
		processInput(window);

		/* RENDERING COMMANDS START */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // fill color buffer with greenish blue
		glClear(GL_COLOR_BUFFER_BIT); // clear color buffer

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
	glDeleteBuffers(1, &EBO);

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
