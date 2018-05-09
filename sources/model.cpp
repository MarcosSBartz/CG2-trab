#include "model.hpp"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

model::model(const char* Texture, GLuint programID, vec3 position, GLuint meshID)
{
	// Load the texture
	this->Texture = loadDDS(Texture);
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	this->meshID = meshID;

	// Posicao inicial da matriz
	ModelMatrix = translate(position);

}

mat4 model::getModelMatrix() {
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

void model::clear() {
	glDeleteTextures(1, &Texture);
}

