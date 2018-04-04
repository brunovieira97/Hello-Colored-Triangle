/* 
	Hello, Colored Triangle 
 
	Author: Bruno Peres Vieira - brunopv@edu.unisinos.br
	Initial release: 03/18/2018
 */

using namespace std;

/* Includes*/
#include <iostream>
#include <string>
#include <assert.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// Using Shader header from LearnOpenGL tutorials, so shaders are read from external files.
#include "Shader.h"		

/* Constant declarations */
// Vertex source
/*
const char* vert_source = ""
"#version 410\n"
"in vec2 position;\n"
"in vec4 color;\n"
"out vec4 color0;\n"
"void main() {\n"
"    gl_Position = vec4(position, 0.0, 1.0);\n"
"    color0 = color;\n"
"}\n";

// Fragment source
const char* frag_source = ""
"#version 410\n"
"out vec4 frag_color;\n"
"in vec4 color0;\n"
"void main() {\n"
"    frag_color = color0;\n"
"}\n";
*/

// Vertex positions array (left, right, top)
const float positions[] = {
	-0.5, -1.0,
	0.5, -1.0,
	0.0, 0.0,

	-0.5, 1.0,
	0.5, 1.0,
	0.0, 0.0
};

// Vertices RGBA colors array
const float colors[] = {
	1, 0, 0, 1,
	0, 1, 0, 1,
	0, 0, 1, 1,

	1, 0, 0, 1,
	0, 1, 0, 1,
	0, 0, 1, 1,
};

/* Main */
int main() {
	if (!glfwInit()) {
		std::cout << "GLFW init failed! \n";
	}

	// Set OpenGL target version to 4.1
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

	GLFWwindow *window = glfwCreateWindow(512, 512, "Hello, colored Triangle!", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW init failed! \n";
	}

	/* Triangle setup*/
	// Creates shader program, vertex and fragment shaders
	//unsigned int shader_program = glCreateProgram();

	Shader shader_program("../shaders/transformations.vs", "../shaders/transformations.frag");

	//unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	//unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);

	enum { POSITION_ATTRIB_LOC, COLOR_ATTRIB_LOC };

	// Applies shader's source code (constant)
	//glShaderSource(vs, 1, &vert_source, NULL);
	//glShaderSource(fs, 1, &frag_source, NULL);

	// Compiling both shaders
	//glCompileShader(vs);
	//glCompileShader(fs);

	// Attaches vertex shader and fragment shader to current shader program
	//glAttachShader(shader_program, vs);
	//glAttachShader(shader_program, fs);

	// Binds attribute pointers to the shader program
	//glBindAttribLocation(shader_program, POSITION_ATTRIB_LOC, "position");
	//glBindAttribLocation(shader_program, COLOR_ATTRIB_LOC, "color");

	// Links shader program
	//glLinkProgram(shader_program);
	//glValidateProgram(shader_program);

	// Shader already linked, can be destroyed
	//glDeleteShader(vs);
	//glDeleteShader(fs);

	/* Buffers */
	enum { POSITION, COLOR, NUM_BUFFERS };
	unsigned int vao, vbo[NUM_BUFFERS];

	// Generates and binds the VAO 
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Generates VBO
	glGenBuffers(NUM_BUFFERS, vbo);

	// Setting VBOs position properties
	glBindBuffer(GL_ARRAY_BUFFER, vbo[POSITION]);
	// Sets VBO data to be received from *positions* array
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_ATTRIB_LOC);
	glVertexAttribPointer(POSITION_ATTRIB_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	// Setting VBO color properties
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COLOR]);
	// Sets VBO data to be received from *colors* array
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(COLOR_ATTRIB_LOC);
	glVertexAttribPointer(COLOR_ATTRIB_LOC, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);

	// Clears window background
	glClearColor(0.1, 0.1, 0.1, 1.0);
	
	// Game Loop
	while (!glfwWindowShouldClose(window)) {
		glfwWaitEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		// Using shader program
		//glUseProgram(shader_program);
		shader_program.Use();
		// Binds VAO to current shader program
		glBindVertexArray(vao);
		// Draws VAO arrays
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
	}
}