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

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include <iostream>

// Generally not a good idea to include the whole namespace. 
// But it makes it simpler in examples.
using namespace glm;

// Window Size
const unsigned int width = 800;
const unsigned int height = 800;

// Create Camera Object
Camera camera;

double lastX = width / 2.0f;
double lastY = height / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// Interactions
bool mouseLeftButtonDown = false;
bool mouseRightButtonDown = false;
bool isDragging = false;

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
		isDragging = true;
		mouseLeftButtonDown = true;
		glfwGetCursorPos(window, &lastX, &lastY);
		printf("Left Button Down Pressed");
	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		mouseRightButtonDown = true;
		glfwGetCursorPos(window, &lastX, &lastY);
		printf("Right Button Down Pressed");
	}

	else if (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		isDragging = false;
		mouseLeftButtonDown = false;
		mouseRightButtonDown = false;
	}
}

// Update mouse handler callbacks (physical movement of the mouse)
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	std::cout << xoffset << " : " << yoffset << std::endl;

	lastX = xpos;
	lastY = ypos;

	if (isDragging && mouseLeftButtonDown)
	{
		camera.Orbit(xoffset, yoffset);
	} 

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
}

// Update scrolling callback on the physical mouse
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.Zoom(yoffset);
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
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetScrollCallback(window, scrollCallback);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	stbi_set_flip_vertically_on_load(true);

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Load the shaders
	Shader shaderProgram("vert.glsl", "frag.glsl");
	shaderProgram.use();

	// Load in model
	Model pen("models/pen.obj", true);

	// Build model matrix
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 model_translate_vec = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 model_scale = glm::vec3(1.0, 1.0, 1.0);
	float model_rotate_angle = glm::radians(0.0);

	glm::vec3 model_rotate_axis = glm::vec3(1.0, 0.0, 0.0);

	model = glm::translate(model, model_translate_vec);
	model = glm::scale(model, model_scale);
	model = glm::rotate(model, model_rotate_angle, model_rotate_axis);

	// Send model matrix to vertex shader as it remains constant
	shaderProgram.use();
	shaderProgram.setMat4("model", model);

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
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set the view and projection matrices
		shaderProgram.setMat4("view", camera.GetViewMatrix());
		shaderProgram.setMat4("projection", camera.GetProjectionMatrix());

		shaderProgram.use();

		pen.Draw(shaderProgram);

		// Tell OpenGL a new frame is about to begin
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ImGUI window creation
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("Import...");
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();

		ImGui::SetNextWindowSizeConstraints(ImVec2(width, 100), ImVec2(FLT_MAX, 100));
		ImGui::SetNextWindowPos(ImVec2(0, 18));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // set text color to black
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // set window background color to transparent
		ImGui::Begin("Instructions", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus);
		
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(30, 30)); // Set equal padding on left and top
		
		ImGui::SetWindowFontScale(1.2f);
		ImGui::Text("Controls");
		ImGui::PopStyleVar();

		ImGui::Indent(); // Add bullet points
		ImGui::BulletText("Scroll to Zoom");
		ImGui::BulletText("Left Click to spin model");
		ImGui::Unindent();

		ImGui::End();
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(2);

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