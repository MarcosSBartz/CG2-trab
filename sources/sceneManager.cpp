#include "sceneManager.hpp"

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

sceneManager::sceneManager()
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	programID = LoadShaders("shaders/StandardShading.vertexshader", "shaders/StandardShading.fragmentshader");
	glUseProgram(programID);
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	MatrixID = glGetUniformLocation(programID, "MVP");

}

void sceneManager::addMesh(const char* object) {
	mesh newMesh = mesh(object);
	meshs.push_back(newMesh);
}
void sceneManager::addModel(const char* Texture, vec3 position, GLuint meshID) {
	model newModel = model(Texture, programID, position, meshID);
	models.push_back(newModel);
}

GLuint sceneManager::getProgramID() {
	return programID;
}

GLuint sceneManager::getLightID() {
	return LightID;
}

GLuint sceneManager::getMatrixID() {
	return MatrixID;
}


void sceneManager::drawScenario(mat4 ProjectionMatrix, mat4 ViewMatrix,
	GLuint ViewMatrixID, GLFWwindow* g_pWindow) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programID);

	for (auto it = models.begin(); it != models.end();it++) {
		mat4 MVP = ProjectionMatrix * ViewMatrix * (*it).getModelMatrix();
		//Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv((*it).getModelMatrixID(), 1, GL_FALSE, &((*it).getModelMatrix()[0][0]));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		vec3 lightPos = vec3(4, 4, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, (*it).getTexture());
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i((*it).getTextureID(), 0);


		// Carrega a malha
		meshs.at((*it).getMeshID()).getBuffers();
		meshs.at((*it).getMeshID()).drawMesh();

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		// Draw tweak bars
		TwDraw();

	}
	// Swap buffers
	glfwSwapBuffers(g_pWindow);
	glfwPollEvents();
}

void sceneManager::clear() {
	for (unsigned i = 0; i < models.size(); i++)
	{
		models.at(i).clear();
	}
	for (unsigned i = 0; i < meshs.size(); i++)
	{
		meshs.at(i).clear();
	}
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
}

std::vector<model>* sceneManager::getModels() {
	return &models;
}
/*
void sceneManager::execTransformations() {
	for (auto it = models.begin(); it != models.end(); it++) {
		if ((*it).getFlag() == false) {
			printf("Chama");
		}
		if ((*it).getFlag()) {
			printf("nemMeVIU");
			(*it).transformationMatrix();
		}
	}
}*/

void sceneManager::execTransformations() {
	for (int i = 0; i < models.size(); i++) {
		if (models.at(i).getFlag()) {
			models.at(i).transformationMatrix();
		}
	}
}