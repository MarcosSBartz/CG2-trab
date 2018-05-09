#pragma once

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <shader.hpp>
#include <model.hpp>
#include <mesh.hpp>


class sceneManager
{
	glm::vec3 lightPos;
	GLuint programID;
	GLuint LightID;
	GLuint MatrixID;
	std::vector<mesh> meshs;
	std::vector<model> models;
	
public:
	sceneManager();
	GLuint getProgramID();
	GLuint getLightID();
	GLuint getMatrixID();
	void addModel(const char* Texture, glm::vec3 position, GLuint meshID);
	void addMesh(const char* object);

	void drawScenario(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix,
		GLuint ViewMatrixID, GLFWwindow* g_pWindow);
};

