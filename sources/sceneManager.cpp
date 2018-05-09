#include "sceneManager.hpp"



sceneManager::sceneManager()
{
	programID = LoadShaders("shaders/StandardShading.vertexshader", "shaders/StandardShading.fragmentshader");
	glUseProgram(programID);
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	MatrixID = glGetUniformLocation(programID, "MVP");

}

void sceneManager::addMesh(const char* object) {
	mesh newMesh = mesh(object);
	meshs.push_back(newMesh);
}
void sceneManager::addModel(const char* Texture, glm::vec3 position, GLuint meshID) {
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


void sceneManager::drawScenario(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix,
	GLuint ViewMatrixID, GLFWwindow* g_pWindow) 
{
	glUseProgram(programID);
	for (unsigned short i = 0; i < models.size();i++) {
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * models.at(i).getModelMatrix();
		//Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(models.at(i).getModelMatrixID, 1, GL_FALSE, &models.at(i).getModelMatrix()[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		lightPos = glm::vec3(4, 4, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, models.at(i).gTexture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

	}
}