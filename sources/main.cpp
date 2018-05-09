// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* g_pWindow;
unsigned int g_nWidth = 1024, g_nHeight = 768;

// Include AntTweakBar
#include <AntTweakBar.h>
TwBar *g_pToolBar;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <shader.hpp>
#include <texture.hpp>
#include <controls.hpp>
#include <objloader.hpp>
#include <vboindexer.hpp>
#include <glerror.hpp>
#include<mesh.hpp>
#include<model.hpp>
#include<sceneManager.hpp>

void WindowSizeCallBack(GLFWwindow *pWindow, int nWidth, int nHeight) {

	g_nWidth = nWidth;
	g_nHeight = nHeight;
	glViewport(0, 0, g_nWidth, g_nHeight);
	TwWindowSize(g_nWidth, g_nHeight);
}

int main(void)
{
	int nUseMouse = 0;

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	g_pWindow = glfwCreateWindow(g_nWidth, g_nHeight, "CG UFPel", NULL, NULL);
	if (g_pWindow == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(g_pWindow);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	check_gl_error();//OpenGL error from GLEW

	// Initialize the GUI
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(g_nWidth, g_nHeight);

	// Set GLFW event callbacks. I removed glfwSetWindowSizeCallback for conciseness
	glfwSetMouseButtonCallback(g_pWindow, (GLFWmousebuttonfun)TwEventMouseButtonGLFW); // - Directly redirect GLFW mouse button events to AntTweakBar
	glfwSetCursorPosCallback(g_pWindow, (GLFWcursorposfun)TwEventMousePosGLFW);          // - Directly redirect GLFW mouse position events to AntTweakBar
	glfwSetScrollCallback(g_pWindow, (GLFWscrollfun)TwEventMouseWheelGLFW);    // - Directly redirect GLFW mouse wheel events to AntTweakBar
	glfwSetKeyCallback(g_pWindow, (GLFWkeyfun)TwEventKeyGLFW);                         // - Directly redirect GLFW key events to AntTweakBar
	glfwSetCharCallback(g_pWindow, (GLFWcharfun)TwEventCharGLFW);                      // - Directly redirect GLFW char events to AntTweakBar
	glfwSetWindowSizeCallback(g_pWindow, WindowSizeCallBack);


	// CRIACAO DA TOOLBAR
	g_pToolBar = TwNewBar("CG UFPel ToolBar");
	/*typedef enum { SUZANNE, GOOSE, CUBE } MESH_TYPE;
	// cria uma variavel para a mesh atual selecionada
	MESH_TYPE currentMesh = SUZANNE;
	TwEnumVal meshs[] = { { SUZANNE, "Suzanne" },{ GOOSE, "Goose" },{ CUBE, "Cube" } };
	TwType meshType = TwDefineEnum("Malha", meshs, 3);

	typedef enum { UVMAP_T, GOOSE_T } TEXTURE_TYPE;
	TEXTURE_TYPE currentTexture = UVMAP_T;
	TwEnumVal textures[] = { { UVMAP_T, "uvmap" },{ GOOSE_T, "goose" } };
	TwType textureType = TwDefineEnum("Textura", textures, 2);

	//Para adicionar um novo modelo
	glm::vec3 posicao_novomodelo(0);
	TwAddSeparator(g_pToolBar, "Modelo", NULL);
	TwAddButton(g_pToolBar, "Parametros Novo Modelo", NULL, NULL, "");
	TwAddVarRW(g_pToolBar, "Modelo: ", meshType, &currentMesh, NULL);
	TwAddVarRW(g_pToolBar, "Textura: ", textureType, &currentTexture, NULL);
	TwAddVarRW(g_pToolBar, "Posicao:", TW_TYPE_DIR3F, &posicao_novomodelo, NULL);
	*/
	// FIM TOOLBAR
	
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(g_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(g_pWindow, g_nWidth / 2, g_nHeight / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	sceneManager manager = sceneManager();
	GLuint ViewMatrixID = glGetUniformLocation(manager.getProgramID(), "V");

	glUseProgram(manager.getProgramID());
	
	manager.addMesh("mesh/suzanne.obj");
	manager.addMesh("mesh/goose.obj");

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames    = 0;
	vec3 pos(1);
	manager.addModel("mesh/uvmap.DDS", pos, 0);

	do{
        check_gl_error();

        //use the control key to free the mouse
		if (glfwGetKey(g_pWindow, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS)
			nUseMouse = 0;
		else
			nUseMouse = 1;

		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1sec ago
			// printf and reset
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames  = 0;
			lastTime += 1.0;
		}

		// Carrega os buffers para desenhar
		//goose.getBuffers();
		//suzan.getBuffers();
		
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(manager.getProgramID());

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs(nUseMouse, g_nWidth, g_nHeight);
		mat4 ProjectionMatrix = getProjectionMatrix();
		mat4 ViewMatrix       = getViewMatrix();
		//glm::mat4 ModelMatrix      = glm::mat4(1.0);
		//glm::mat4 MVP              = ProjectionMatrix * ViewMatrix * ModelMatrix;

		if ((manager.getModels()).size() > 0) {
			manager.drawScenario(ProjectionMatrix, ViewMatrix, ViewMatrixID, g_pWindow);
		}
		else {
			// Clear the screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Use our shader
			glUseProgram(manager.getProgramID());
			// Draw tweak bars
			TwDraw();
			// Swap buffers
			glfwSwapBuffers(g_pWindow);
			glfwPollEvents();
		}

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(g_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(g_pWindow) == 0);

	// Cleanup VBO and shader
	manager.clear();
	glDeleteProgram(manager.getProgramID());

	// Terminate AntTweakBar and GLFW
	TwTerminate();
	glfwTerminate();

	return 0;
}
