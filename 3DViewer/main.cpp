/*
* This program uses example code that has been modified from the original author.
* Most code is basad off the OpenGL startup guide + a Youtube tutorial example.
* There are many parts of the codebase from the original that has been modified.
*
* Hadi Rumjahn
* February 2023
*/

#include <filesystem>
namespace fs = std::filesystem;

#include "Model.h"
#include "Camera.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <iostream>

// Generally not a good idea to include the whole namespace. 
// But it makes it simpler in examples.
using namespace glm;

// Window Size
const unsigned int width = 720;
const unsigned int height = 720;

// Create Camera Object
Camera camera(glm::vec3(0.0f, 20.0f, 50.0f));
double lastX = width / 2.0f;
double lastY = height / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// Interactions
bool mouseLeftButtonDown = false;

void processInput(GLFWwindow* window)
{
	// Exit the program
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// Process WASD Movements of the camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
		printf("FORWARD PRESSED \n");
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		printf("BACKWARD PRESSED \n");
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
		printf("LEFT PRESSED \n");
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
		printf("RIGHT PRESSED \n");
	}
}

// Updates resizing of the window
void resizeWindowCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Update mouse button handler callbacks (Clicking/Pressing)
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mouseLeftButtonDown = true;
		glfwGetCursorPos(window, &lastX, &lastY);
	}

	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		mouseLeftButtonDown = false;
	}
}

// Update mouse handler callbacks (physical movement of the mouse)
void mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	double xpos = xposIn;
	double ypos = yposIn;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	// Only move camera when left clicking
	if (mouseLeftButtonDown)
	{
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

// Update scrolling callback on the physical mouse
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
	printf("SCROLLING MOUSE \n");
}

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "3D Model Viewer"
	GLFWwindow* window = glfwCreateWindow(width, height, "3D Model Viewer", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resizeWindowCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("7.4.camera.vs", "7.4.camera.fs");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	// Load in model
	Model pen("pen.obj", false);

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Set up the projection matrix
	glm::mat4 projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	// Set up the view matrix
	glm::mat4 view = lookAt(vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	// Log some messages
	printf("OpenGL version: %s\n", glGetString(GL_VERSION));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// GLFW Input Control
		processInput(window);

		// Render
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		shaderProgram.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		shaderProgram.setMat4("view", view);

		// Tell OpenGL a new frame is about to begin
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ImGUI window creation
		ImGui::Begin("3D Model Viewer");

		// Text that appears in the window
		ImGui::Text("Controls");

		// Slider that appears in the window
		ImGui::Text("Scroll Wheel: Zoom");
		ImGui::Text("Left Click to activate camera movement on a free axis");
		ImGui::Text("Then use WASD to fly around object shown");
		ImGui::Text("W: Pan Up");
		ImGui::Text("A: Pan Left");
		ImGui::Text("S: Pan Down");
		ImGui::Text("D: Pan Right");

		//ImGui::ShowDemoWindow();

		// Ends the window
		ImGui::End();

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		shaderProgram.setMat4("model", model);

		pen.Draw(shaderProgram);

		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Take care of all GLFW events
		glfwPollEvents();

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
	}

	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Delete window before ending the program
	glfwDestroyWindow(window);

	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;
}