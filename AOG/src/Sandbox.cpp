#include "Shader.h"
#include "Texture.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>

float mixValue = .2f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mixValue += 0.001f;
		if (mixValue >= 1.0f) mixValue = 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mixValue -= 0.001f;
		if (mixValue <= 0.0f) mixValue = 0.0f;
	}
}

void shaderCompilationCheck(unsigned int& id)
{
	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(id, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void programLinkageCheck(unsigned int& id)
{
	int success;
	char infoLog[512];
	glad_glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

int main()
{
	// Initialise GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile means we ll get access to smaller subset of OpenGL features
	// without backwards-compatible features we no longer need
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make the above window the main context of the current thread
	glfwMakeContextCurrent(window);

	// Glad manages function pointer to opengl so we want to initialise that first
	// We pass GLAD the function to load the address of the OpenGL function pointers (OS-specific)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		return -1;
	}

	// Before we can start rendering, set viewport
	glViewport(0, 0, 800, 600);

	// Callbaccak to resize window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// VAO - Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// Bind the vertex array object
	glBindVertexArray(VAO);

	// Hello-Triangle ----------------------------------------------------------
	float vertices[] = {    
		 // positions		colors				texture
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  1.0f,  1.0f, // top right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 0.2f, 0.4f, 0.1f,  0.0f,  1.0f, // top left  
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// Generate vertex buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// Bind newly generate buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Communicate to the gpu how you want the data to be displayed

	// position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture Attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Index Buffer
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Shaders
	Shader ourShader("./assets/shaders/vertexShader.glsl", "./assets/shaders/fragmentShader.glsl");
	ourShader.use();

	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	// Textures
	Texture texture1("./assets/textures/container.jpg");
	Texture texture2("./assets/textures/awesomeface.png");
	
	// Model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// View matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	// Projection matrix
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	ourShader.setMat4f("model", model);
	ourShader.setMat4f("view", view);
	ourShader.setMat4f("projection", projection);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		/*Input commands*/
		processInput(window);

		/* Rendering */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use Shader
		texture1.Bind(GL_TEXTURE0);
		texture2.Bind(GL_TEXTURE1);

		// Set the mix value
		ourShader.setFloat("mixValue", mixValue);

		ourShader.use();
		glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 3); -- GETS REPLACED BY DRAW ELEMENTS
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/* Check and call events and swap buffers */
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}