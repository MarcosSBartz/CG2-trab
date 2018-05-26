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
#include "glm/ext.hpp"

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

	// Adicionar Malhas
	typedef enum { SUZANNE, GOOSE, CUBE} MESH_TYPE;
	MESH_TYPE currentMesh = SUZANNE;
	TwEnumVal meshs[] = { { SUZANNE, "Suzanne" },{ GOOSE, "Goose" },{ CUBE, "Cube" } };
	TwType meshType = TwDefineEnum("Mesh", meshs, 3);

	// Adicionar texturas
	typedef enum { UVMAP_T, GOOSE_T } TEXTURE_TYPE;
	TEXTURE_TYPE currentTexture = UVMAP_T;
	TwEnumVal textures[] = { { UVMAP_T, "uvmap" },{ GOOSE_T, "goose" } };
	TwType textureType = TwDefineEnum("Texture", textures, 2);

	// Shear mode
	typedef enum { X, Y, Z } AXIS_TYPE;
	AXIS_TYPE currentAxis = X;
	TwEnumVal axis[] = { { X, "X" },{ Y, "Y" },{ Z, "Z" } };
	TwType axisType = TwDefineEnum("Axis", axis, 3);
	
	// Criar novo modelo
	vec3 positionNewModel(1.0);
	TwAddButton(g_pToolBar, "New Model", NULL, NULL, "");
	TwAddVarRW(g_pToolBar, "Model: ", meshType, &currentMesh, NULL);
	TwAddVarRW(g_pToolBar, "Texture: ", textureType, &currentTexture, NULL);
	TwAddVarRW(g_pToolBar, "Position:", TW_TYPE_DIR3F, &positionNewModel, NULL);
	TwAddSeparator(g_pToolBar, "", NULL);
	
	// Translacao
	vec3 positionTranslation(1.0);
	double timeTranslation = 10;
	TwAddButton(g_pToolBar, "Translation", NULL, NULL, "");
	TwAddVarRW(g_pToolBar, "T Position:", TW_TYPE_DIR3F, &positionTranslation, NULL);
	TwAddVarRW(g_pToolBar, "T Time: ", TW_TYPE_DOUBLE, &timeTranslation, NULL);
	TwAddSeparator(g_pToolBar, "", NULL);
	
	// Escala
	vec3 positionScale(2,4,6);
	double timeScale = 5;
	TwAddButton(g_pToolBar, "Scale", NULL, NULL, "");
	TwAddVarRW(g_pToolBar, "S Position:", TW_TYPE_DIR3F, &positionScale, NULL);
	TwAddVarRW(g_pToolBar, "S Time: ", TW_TYPE_DOUBLE, &timeScale, NULL);
	TwAddSeparator(g_pToolBar, "", NULL);

	// Rotacao
	vec3 positionRotation(2, 4, 6);
	int32 angleRotation = 180;
	double timeRotation = 5;
	TwAddButton(g_pToolBar, "Rotation", NULL, NULL, "");
	TwAddVarRW(g_pToolBar, "R Position:", TW_TYPE_DIR3F, &positionRotation, NULL);
	TwAddVarRW(g_pToolBar, "R Angle: ", TW_TYPE_INT32, &angleRotation, NULL);
	TwAddVarRW(g_pToolBar, "R Time: ", TW_TYPE_DOUBLE, &timeRotation, NULL);
	//TwAddVarRW(g_pToolBar, "R Angle: ", TW_TYPE_DOUBLE, &angle, NULL);
	TwAddSeparator(g_pToolBar, "", NULL);

	// Shear
	vec3 positionShear(2, 2, 2);
	double timeShear = 5;
	TwAddButton(g_pToolBar, "Shear", NULL, NULL, "");
	TwAddVarRW(g_pToolBar, "Sh Position:", TW_TYPE_DIR3F, &positionShear, NULL);
	TwAddVarRW(g_pToolBar, "Sh Time: ", TW_TYPE_DOUBLE, &timeShear, NULL);
	TwAddVarRW(g_pToolBar, "Sh Axis: ", axisType, &currentAxis, NULL);
	//TwAddVarRW(g_pToolBar, "R Angle: ", TW_TYPE_DOUBLE, &angle, NULL);
	TwAddSeparator(g_pToolBar, "", NULL);

	// Rotacao ao redor de um ponto
	vec3 positionRAP(2, 2, 1);
	vec3 directionRAP(2, 1, 3);
	int32 angleRAP(720);
	double timeRAP = 5;
	TwAddButton(g_pToolBar, "Rotation Around a Point (RAP)", NULL, NULL, "");
	TwAddVarRW(g_pToolBar, "RAP Position:", TW_TYPE_DIR3F, &positionRAP, NULL);
	TwAddVarRW(g_pToolBar, "RAP Direction:", TW_TYPE_DIR3F, &directionRAP, NULL);
	TwAddVarRW(g_pToolBar, "RAP Angle: ", TW_TYPE_INT32, &angleRAP, NULL);
	TwAddVarRW(g_pToolBar, "RAP Time: ", TW_TYPE_DOUBLE, &timeRAP, NULL);
	TwAddSeparator(g_pToolBar, "", NULL);

	// Rotacao ao redor de um eixo
	vec3 positionRAA(4, 2, 2);
	int32 angleRAA(222);
	double timeRAA = 5;
	TwAddButton(g_pToolBar, "Rotation Around a Axis (RAA)", NULL, NULL, "");
	TwAddVarRW(g_pToolBar, "RAA Position:", TW_TYPE_DIR3F, &positionRAA, NULL);
	TwAddVarRW(g_pToolBar, "RAA Angle: ", TW_TYPE_INT32, &angleRAA, NULL);
	TwAddVarRW(g_pToolBar, "RAA Time: ", TW_TYPE_DOUBLE, &timeRAA, NULL);
	TwAddSeparator(g_pToolBar, "", NULL);

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
	manager.addMesh("mesh/cube.obj");

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames    = 0;
	vec3 pos(0);
	manager.addModel("mesh/uvmap.DDS", pos, 0);
	vec3 newPos(-1);

	
	/* TESTE TRANSLATE - OPENGL
	
	//Builds a translation 4 * 4 matrix created from a vector of 3 components.
	glm::mat4 myMatrix = glm::translate(glm::mat4(), glm::vec3(10.0f, 0.0f, 0.0f));
	glm::vec4 myVector(10.0f, 10.0f, 10.0f, 1.0f);
	glm::vec4 transformedVector = myMatrix * myVector; // guess the result

	std::cout << glm::to_string(myMatrix) << std::endl;

	std::cout << glm::to_string(transformedVector) << std::endl;

	*/

	vec3 n1(10, 10, 10);
	vec3 n2(5, 5, 5);
	vec3 r = n1 * n2;

	uint meshID;
	uint currentModelID = 0;
	char *texture;
	char auxAxis;

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

		// Criar novo modelo
		if (glfwGetKey(g_pWindow, GLFW_KEY_SPACE) == GLFW_PRESS && (currentTime > lastTime + 0.5)) {
			lastTime = glfwGetTime();
			// Parametros do novo modelo
			if (currentMesh == SUZANNE)
				meshID = 0;
			else if (currentMesh == GOOSE)
				meshID = 1;
			else if (currentMesh == CUBE)
				meshID = 2;
			if (currentTexture == UVMAP_T)
				texture = "mesh/uvmap.DDS";
			else
				texture = "mesh/goose.DDS";
			manager.addModel(texture, positionNewModel, meshID);
			currentModelID = manager.getModels()->size() -1;
		}

		// Fazer translacao
		if (glfwGetKey(g_pWindow, GLFW_KEY_T) == GLFW_PRESS && currentTime > (lastTime + 0.5)) {
			lastTime = glfwGetTime();
			manager.getModels()->at(currentModelID).translateOP(positionTranslation, timeTranslation);
		}

		// Fazer escala
		if (glfwGetKey(g_pWindow, GLFW_KEY_S) == GLFW_PRESS && currentTime > (lastTime + 0.5)) {
			lastTime = glfwGetTime();
			manager.getModels()->at(currentModelID).scaleOP(positionScale, timeScale);
		}

		// Fazer rotacao
		if (glfwGetKey(g_pWindow, GLFW_KEY_R) == GLFW_PRESS && currentTime > (lastTime + 0.5)) {
			lastTime = glfwGetTime();
			manager.getModels()->at(currentModelID).rotateOP(positionRotation, angleRotation, timeRotation);
		}

		// Fazer shear
		if (glfwGetKey(g_pWindow, GLFW_KEY_H) == GLFW_PRESS && currentTime > (lastTime + 0.5)) {
			lastTime = glfwGetTime();
			if (currentAxis == X)
				auxAxis = 'x';
			if (currentAxis == Y)
				auxAxis = 'y';
			if (currentAxis == Z)
				auxAxis = 'z';
			manager.getModels()->at(currentModelID).shear2(positionShear, timeShear, auxAxis);
		}
		
		// Fazer rotacao ao redor de um ponto
		if (glfwGetKey(g_pWindow, GLFW_KEY_A) == GLFW_PRESS && currentTime > (lastTime + 0.5)) {
			lastTime = glfwGetTime();
			manager.getModels()->at(currentModelID).rotateAroundPoint(positionRAP,directionRAP,angleRAP,timeRAP);
		}

		// Fazer rotacao ao redor de um Eixo
		if (glfwGetKey(g_pWindow, GLFW_KEY_E) == GLFW_PRESS && currentTime > (lastTime + 0.5)) {
			lastTime = glfwGetTime();
			manager.getModels()->at(currentModelID).rotateAroundAxis(positionRAA, angleRAA, timeRAA);
		}

		// Translacao + rotacao + escala
		if (glfwGetKey(g_pWindow, GLFW_KEY_C) == GLFW_PRESS && currentTime > (lastTime + 0.5)) {
			lastTime = glfwGetTime();
			manager.getModels()->at(currentModelID).translateScale(positionTranslation, positionScale, positionRotation, angleRotation, 5);
		}

		// back
		if (glfwGetKey(g_pWindow, GLFW_KEY_B) == GLFW_PRESS && currentTime > (lastTime + 0.5)) {
			lastTime = glfwGetTime();
			manager.getModels()->at(currentModelID).back(positionTranslation, 5);
		}


		// Executar as mudancas
		manager.execTransformations();

		// Desenhar
		if ((manager.getModels())->size() > 0) {
			manager.drawScenario(ProjectionMatrix, ViewMatrix, ViewMatrixID, g_pWindow);
		}
		// Tela sem modelos = tela limpa
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
