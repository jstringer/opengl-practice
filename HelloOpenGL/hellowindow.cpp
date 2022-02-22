#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


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

//Vertex Shader
const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

// Fragment shader
const char * fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

const char * altFragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.9f, 0.1f, 1.0f);\n"
	"}\0";

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

	// ------ Shader compilation and linking --------

	//Vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
	}

	// ------ Fragment shader ---------
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int altFragmentShader;
	altFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(altFragmentShader, 1, &altFragmentShaderSource, NULL);
	glCompileShader(altFragmentShader);

	//Create shader program, link shaders together, tell openGL to use this program
	unsigned int firstShaderProgram;
	firstShaderProgram = glCreateProgram();
	glAttachShader(firstShaderProgram, vertexShader);
	glAttachShader(firstShaderProgram, fragmentShader);
	glLinkProgram(firstShaderProgram);


	//Second program
	unsigned int secondShaderProgram;
	secondShaderProgram = glCreateProgram();
	glAttachShader(secondShaderProgram, vertexShader);
	glAttachShader(secondShaderProgram, altFragmentShader);
	glLinkProgram(secondShaderProgram);

	//Free shader memory
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(altFragmentShader);

	// ---------- Vertex data config  ----------------

	// A float array representing the coordinates for the three vertices of a triangle
	float first_triangle[] = {
		0.5f, 0.5f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};

	float second_triangle[] = {	
		-0.5f, 0.5f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};

	//Create vertex buffer object (for holding vertices), and vertex array object (for holding many vertex buffer objects)	
	unsigned int vbos[2], vaos[2];
	glGenVertexArrays(2, vaos);
	glGenBuffers(2, vbos);		//Generate the vertex buffer

	glBindVertexArray(vaos[0]);		//Bind the vertex array so we can add buffers to it
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);		//Bind the buffer so we can add vertices to it
	glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle), first_triangle, GL_STATIC_DRAW);		//Add the 3 vertices to the buffer	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	//Tell OpenGL how to interpret vertex attributes in the currently bound buffer
	glEnableVertexAttribArray(0);	//Enable the one and only attribute pointer for vaos[0]

	glBindVertexArray(vaos[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(second_triangle), second_triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	//Enable the one and only attribute pointer for vaos[1]

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
	
		glUseProgram(firstShaderProgram);
		glBindVertexArray(vaos[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(secondShaderProgram);
		glBindVertexArray(vaos[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();	//Check if any events have occurred that need to be handled during this frame
		glfwSwapBuffers(window);	//Swap the front (visible) buffers with the back buffers that have been updated during this frame
	}

	//Clean up OpenGL resources
	glDeleteVertexArrays(2, vaos);
	glDeleteBuffers(2, vbos);
	glDeleteProgram(firstShaderProgram);
	glDeleteProgram(secondShaderProgram);

	//Clean up GLFW resources and terminate
	glfwTerminate();
	return 0;
}