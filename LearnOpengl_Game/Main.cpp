#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "Shader.h"
#include "Dep/include/stb_image.h"

enum Color {
	RED,
	GREEN,
	BLUE,
};

void screen_size_changed_callback(GLFWwindow* _window, int width, int height);
void process_input(GLFWwindow* _window);
void clear_screen();
const char* read_shader(const char* filename);

int screenW = 1280;
int screenH = 720;


float vertices[] = {
	 -0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top left 
	 0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top right   
	 0.5f,  -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
	 -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f // bottom right

};

unsigned int rect_indices[] = {  // note that we start from 0!
	0, 1, 2,  // first Triangle
	0, 3, 2   // second Triangle
};


unsigned int vbo;
unsigned int vao;
unsigned int ebo;
unsigned int texture1;



int main() {

#pragma region Init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(screenW, screenH, "Learn OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Window, failed to open~!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	// load glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// resize the window
	glViewport(0, 0, screenW, screenH);
	glfwSetFramebufferSizeCallback(window, screen_size_changed_callback);
#pragma endregion

#pragma region Shaders
	Shader def_shader("Shaders/Default.vert", "Shaders/Default.frag");
#pragma endregion

#pragma region VBO/VAO

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	// EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), &rect_indices, GL_STATIC_DRAW);

	// for the vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// for the color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// for the texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// texture
	glGenTextures(1, &texture1);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	//std::cout << "Width: " << width << std::endl << "Height: " << height << std::endl;
	if (data) {

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateTextureMipmap(texture);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else {

		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	def_shader.use();
	def_shader.setInt("texture1", 0);

	//glBindTexture(GL_TEXTURE_2D, 0);

	///unsigned int tex0Uni = glGetUniformLocation(def_shader.id, "texture_");
	//if (!tex0Uni) {
		//std::cout << "Texture uniform failed to be located" << std::endl;
	//}

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(0);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#pragma endregion

#pragma region Loop


	float offset_x = 0;
	float offset_y = 0;
	float prev_delta = 0;
	bool paused = false;
	//float offset_z = 0;
	float delta;

	while (!glfwWindowShouldClose(window)) {

		// update
		process_input(window);

		// render
		// clear
		glClearColor(0.5f, 0.4f, 0.6f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw

		delta = glfwGetTime();

		//glActiveTexture(GL_TEXTURE0);
		
		
		
		//glUniform1i(tex0Uni, 0);
		//def_shader.setBool("color_needed", false);
		// first triangle
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		def_shader.use();
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
#pragma endregion

#pragma region Cleanup

	glDeleteVertexArrays(1, &vao);
	//glDeleteVertexArrays(1, &vao[1]);
	glDeleteBuffers(1, &vbo);
	//glDeleteBuffers(1, &vbo[1]);
	glDeleteBuffers(1, &ebo);
	//glDeleteProgram(blue_shader_program);
	//glDeleteProgram(green_shader_program);

	glfwTerminate();
	return 0;
#pragma endregion


}


void screen_size_changed_callback(GLFWwindow* _window, int width, int height) {
	screenW = width;
	screenH = height;
	glViewport(0, 0, screenW, screenH);
}

void process_input(GLFWwindow* _window) {
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, true);
}

void clear_screen() {
}

const char* read_shader(const char* filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return nullptr;
	}
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	char* buffer = new char[size + 1]; // +1 for null-terminator
	if (!file.read(buffer, size)) {
		std::cerr << "Failed to read file: " << filename << std::endl;
		delete[] buffer;
		return nullptr;
	}
	buffer[size] = '\0'; // Null-terminate for convenience
	return buffer;
}