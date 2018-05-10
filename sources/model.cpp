#include "model.hpp"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#define STEPS 50

model::model(const char* Texture, GLuint programID, vec3 position, GLuint meshID)
{
	// Load the texture
	this->Texture = loadDDS(Texture);
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	this->meshID = meshID;
	flag = 0;

	// Posicao inicial da matriz
	ModelMatrix = translate(position);

}

void model::setModelMatrix(mat4 newMatrix) {
	ModelMatrix = newMatrix;
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

int model::getFlag() {
	return flag;
}

void model::clear() {
	glDeleteTextures(1, &Texture);
}


void model::translateOP(vec3 translation, double time) {
	int steps = (int)(STEPS * time);

	// Matriz identidade com vec3; dividido em etapas 
	mat4 aux = translate(mat4(1), translation / vec3(steps, steps, steps));

	// flag para fazer transformacao
	flag = true;

	// Adiciona passo por passo na matriz que realizara as transformacoes
	for (unsigned short i = 0; i < steps; i++)
		transformations.push_back(aux);
}

void model::transformationMatrix() {
	// transforma um passo da matriz
	setModelMatrix(getModelMatrix() * transformations.front());
	transformations.erase(transformations.begin());
	if (transformations.empty()) {
		flag = false;
	}
}






void model::scaleOP(vec3 scaleSize, double time) {
	int steps = (int)(STEPS * time);

	double stepAux = (double)1 / steps;
	// Encolhendo
	//mat4 aux = scale(mat4(1), vec3(scaleSize.x / steps, scaleSize.y / steps, scaleSize.z / steps));

	// x elevado na stepAux, vai escalar por partes
	mat4 aux = scale(mat4(1), vec3(pow(scaleSize.x, stepAux), pow(scaleSize.y, stepAux), pow(scaleSize.z, stepAux)));


	flag = true;

	for(unsigned short i = 0;i<steps;i++)
		transformations.push_back(aux);
	
}
