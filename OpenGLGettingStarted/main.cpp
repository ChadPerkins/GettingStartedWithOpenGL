#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

int main()
{
	glfwInit();
	// Set the major and minor version of OpenGL to be used
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW to use the core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the window object
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return - 1;
	}
	glfwMakeContextCurrent(window);
	// Tell GLFW to use our resize function on every window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Make sure GLAD is initialized before we can call any OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Build and compile the shader program:
	// -------------------------------------
	// Create a vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Attach the shader source code to the shader and compile the shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Create a fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Attach the shader source code to the shader and compile the shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check for shader compile errors
	int success;
	char infoLog[512];

	// Vertex shader compilation test
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment shader compilation test
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create a Shader Program object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// Attach the compiled shaders to the shader program object and link them
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check to make sure the shader linking was successful
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	
	// Delete the shader objects after linking to the shader program object since we dont need them anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Set up vertex data, buffers and configure vertex attributes:
	// ------------------------------------------------------------
	// Vertecies to render a triangle:
	/*float verticies[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};*/

	// Vertecies to render a rectangle:
	float verticies[] = {
		 0.5f,  0.5f, 0.0f, // Top right
		 0.5f, -0.5f, 0.0f, // Bottom right
		-0.5f, -0.5f, 0.0f, // Bottom left
		-0.5f,  0.5f, 0.0f, // Top left
	};

	unsigned int indicies[] = {
		0, 1, 3, // First triangle
		1, 2, 3  // Second triangle
	};

	// Create an Element Buffer Object
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// Bind and set Element Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	// Generate a Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// Generate a buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first,
	glBindVertexArray(VAO);

	// Then bind and set vertex buffer(s),
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	// And then configure vertex attributes(s)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// uncomment this call to draw in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Input
		// .....
		// Check to see if the user closes GLFW
		processInput(window);

		// Rendering commands
		// .....
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw our first triangle
		glUseProgram(shaderProgram);
		/*glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);*/

		// Draw a rectangle
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Check and call events and swap the buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// Free up all resources alocated by glfw
	glfwTerminate();
	return 0;
}

// A finction to resize the viewport to match the window when resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// A function that closes GLFW if the user presses the escape key
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}