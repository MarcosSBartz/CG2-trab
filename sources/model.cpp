#include "model.hpp"



model::model(const char* Texture, GLuint programID, glm::vec3 position, GLuint meshID)
{
	// Load the texture
	this->Texture = loadDDS(Texture);
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	ModelMatrixID = glGetUniformLocation(programID, "M");

	// Posicao inicial da matriz
	ModelMatrix = glm::translate(glm::mat4(1.0), position);

}

glm::mat4 model::getModelMatrix() {
	return ModelMatrix;
}

GLuint model::getModelMatrixID() {
	return ModelMatrixID;
}

GLuint model::getTexture() {
	return Texture;
}
GLuint model::getTextureID() {
	return TextureID;
}
GLuint model::getMeshID() {
	return meshID;
}


