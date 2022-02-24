#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"


//Callback to be invoked whenever the window resize event occurs
//resizes (replaces?) the framebuffer whenever the size of the viewport changes based on a resizing of the window.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//Processes key input on the window passed in
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	//Inialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create the window object with dimensions and metadata
	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	//Set the newly created window as the GL context on the current thread

	//Load up GLAD so we can call OpenGL methods
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Set the viewport size for the OpenGL rendering area
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	//Register the callback for framebuffer resize event

	Shader shader("vshader.vert", "fshader.frag");
																		
	// ---------- Vertex data config  ----------------


	// A float array representing the coordinates for the three vertices of a triangle
	float vertices[] = {
		//positions		   //colors	
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,	//bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	//bottom left
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f		//top

	};

	//Create vertex buffer object (for holding vertices), and vertex array object (for holding many vertex buffer objects)	
	unsigned int vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);		//Generate the vertex buffer

	glBindVertexArray(vao);		//Bind the vertex array so we can add buffers to it
	glBindBuffer(GL_ARRAY_BUFFER, vbo);		//Bind the buffer so we can add vertices to it
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		//Add the 3 vertices to the buffer	
	
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  6 * sizeof(float), (void*)0);	//Tell OpenGL how to interpret vertex attributes in the currently bound buffer
	glEnableVertexAttribArray(0);

	//color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Unbind both the buffer and the vertex array from the current context so we do not make changes to them accidentally
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//-------- Render loop -----------
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);	//Check if the escape key has been pressed every frame

		//All rendering commands would go here
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	//Sets the color with which to clear the screen. Everything in the color buffer will be reset to this color when necessary
		glClear(GL_COLOR_BUFFER_BIT);	//Clear the color buffer every single frame with the clear color specified above.

		//Draw here
		shader.use();
		shader.setFloat("offset", 0.3);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();	//Check if any events have occurred that need to be handled during this frame
		glfwSwapBuffers(window);	//Swap the front (visible) buffers with the back buffers that have been updated during this frame
	}

	//Clean up OpenGL resources
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	//Clean up GLFW resources and terminate
	glfwTerminate();
	return 0;
}