#include "SceneManager.h"

//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;

SceneManager::SceneManager() {
}

SceneManager::~SceneManager() {
}

void SceneManager::initialize(GLuint w, GLuint h) {
	width = w;
	height = h;

	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();
}

void SceneManager::initializeGraphics() {
	glfwInit();

	// Window
	window = glfwCreateWindow(width, height, "Hello Transform", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	glfwSetWindowSizeCallback(window, resize);

	// This means GLEW will know to use a modern approach for retrieving function pointers/extensions
	glewExperimental = GL_TRUE;

	glewInit();

	// Build and compile our shader program
	addShader("Shaders/transformations.vs", "Shaders/transformations.frag");

	// Scene description can come from external config files
	setupScene();

	// Enters camera setup on first run
	resized = true;
}

void SceneManager::addShader(string vFilename, string fFilename) {
	shader = new Shader(vFilename.c_str(), fFilename.c_str());
}


void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::resize(GLFWwindow * window, int w, int h) {
	width = w;
	height = h;
	resized = true;

	// Defines new viewport dimensions
	glViewport(0, 0, width, height);
}


void SceneManager::do_movement() {
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void SceneManager::render() {
	// Clear the colorbuffer
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render scene
	shader->Use();

	// Create transformations 
	//model = glm::mat4();
	//model = glm::rotate(model, (GLfloat)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

	// Get their uniform location
	GLint modelLoc = glGetUniformLocation(shader->Program, "model");

	// Pass them to the shaders
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	if (resized) {
		setupCamera2D();
		resized = false;
	}

	GLint colorLoc = glGetUniformLocation(shader->Program, "inputColor");
	assert(colorLoc > -1);
	glUseProgram(shader->Program);
	//glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);

	// Draw container
	/*glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);*/

	//model = glm::mat4();
	//model = glm::rotate(model, (GLfloat)glfwGetTime(), glm::vec3(0.0f, 0.0f, 0.1f));

	float colorX = -1.0f;
	float colorY = -1.0f;
	float colorZ = 1.0f;
	
	float x = 1.0;
	float y = 0.0;

	for (size_t i = 0; i < 4; i++) {
		y = 0.0;
		for (size_t j = 0; j < 4; j++) {
			if (j == 1 && i == 0) {
				colorX += 0.5f;
				colorY += 0.3f;
				colorZ -= 0.5f;
			}

			if (j == 3 && i == 1) {
				colorX -= 0.5f;
				colorY -= 0.3f;
				colorZ += 0.5f;
			}

			model = glm::mat4();
			model = glm::translate(model, glm::vec3(x, y, 0.0));

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glUniform4f(colorLoc, colorX, colorY, colorZ, 1.0);

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			colorX += 0.1;
			colorZ -= 0.1;
			y -= 1.0;
		}
		colorY += 0.1;
		x += 1.0;
	}
}

void SceneManager::run() {
	// Game Loop
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		do_movement();

		render();

		glfwSwapBuffers(window);
	}
}

void SceneManager::finish() {
	glfwTerminate();
}


void SceneManager::setupScene() {
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           
		-4.5f, 4.5f, 0.0f,   1.0f, 0.0f, 0.0f,    // Top Right
		-4.5f, 3.5f, 0.0f,   0.0f, 1.0f, 0.0f,    // Bottom Right
		-3.5f, 3.5f, 0.0f,   0.0f, 0.0f, 1.0f,   // Bottom Left
		-3.5f, 4.5f, 0.0f,   1.0f, 1.0f, 0.0f,   // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	GLuint VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);*/

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
}

void SceneManager::setupCamera2D() {
	// Aspect ratio correction
	float ratio;
	float xMin = -5.0, xMax = 5.0, yMin = -5.0, yMax = 5.0, zNear = -5.0, zFar = 5.0;
	if (width >= height) {
		ratio = width / (float)height;
		projection = glm::ortho(xMin*ratio, xMax*ratio, yMin, yMax, zNear, zFar);
	} else {
		ratio = height / (float)width;
		projection = glm::ortho(xMin, xMax, yMin*ratio, yMax*ratio, zNear, zFar);
	}

	// Get their uniform location
	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
