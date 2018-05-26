#include "model.hpp"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#define STEPS 50

model::model(const char* Texture, GLuint programID, vec3 position, GLuint meshID)
{
	// Load the texture
	this->Texture = loadDDS(Texture);
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	this->meshID = meshID;

	// Flag de modificacao
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
	// transforma a matriz passo por vez
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

void model::rotateOP(vec3 rotation, float angle, double time) {
	int steps = (int)(time* STEPS);

	mat4 aux = rotate(mat4(1), (float)(angle/steps), rotation);

	flag = true;

	for (unsigned short i = 0; i < steps; i++)
		transformations.push_back(aux);

}

void model::shearOP(vec3 shear, double time) {
	int steps = (int)(STEPS * time);
	shear = shear / vec3(steps, steps, steps);
	mat4 aux(1);
	aux[0][1] = shear.x;
	aux[0][2] = shear.y;
	aux[0][3] = shear.z;
	
	flag = true;

	for (unsigned short i = 0; i < steps; i++)
		transformations.push_back(aux);
}

void model::shear2(vec3 shear, double time, char axis) {
	int steps = (int)(STEPS * time);
	shear = shear / vec3(steps, steps, steps);
	mat4 aux(1);
	printf("%c", axis);
	if (axis == 'x')
		aux = shearX3D(aux, shear.y, shear.z);
	else if (axis == 'y')
		aux = shearY3D(aux, shear.x, shear.z);
	else if (axis == 'z')
		aux = shearZ3D(aux, shear.x, shear.y);

	flag = true;

	for (unsigned short i = 0; i < steps; i++)
		transformations.push_back(aux);
}


void model::rotateAroundPoint(vec3 point,vec3 rotation, float angle, double time) {
	int steps = (int)STEPS*time;

	// Pegar a posicao inicial do modelo... Vec3 de Mat4
	vec3 currentPosition;
	currentPosition.x = ModelMatrix[3][0];
	currentPosition.x = ModelMatrix[3][1];
	currentPosition.x = ModelMatrix[3][2];
	
	flag = true;

	// Calcular distancia
	vec3 auxPoint = point - currentPosition;

	// glm::rotate( angle_in_degrees, myRotationAxis );
	mat4 aux = translate(auxPoint) * rotate(angle / steps, rotation) * translate(-auxPoint);
	for (unsigned short i = 0; i < steps; i++)
		transformations.push_back(aux);
}

void model::rotateAroundAxis(vec3 rot, float angle, double time) {
	int steps = (int)STEPS*time;

	// Pegar a posicao inicial do modelo... Vec3 de Mat4
	vec3 currentPosition;
	currentPosition.x = ModelMatrix[3][0];
	currentPosition.x = ModelMatrix[3][1];
	currentPosition.x = ModelMatrix[3][2];

	flag = true;

	mat4 aux = rotate(angle / steps, rot);

	for (unsigned short i = 0; i < steps; i++)
		transformations.push_back(aux);

}

void model::translateScale(vec3 translat, vec3 scaleSize, vec3 rotation, float angle, double time) {
	int steps = (int)(time* STEPS);

	// Matriz identidade com vec3; dividido em etapas 
	mat4 aux = translate(mat4(1), translat / vec3(steps, steps, steps));

	double stepAux = (double)1 / steps;
	aux = scale(aux, vec3(pow(scaleSize.x, stepAux), pow(scaleSize.y, stepAux), pow(scaleSize.z, stepAux)));

	aux = rotate(aux, (float)(angle / steps), rotation);

	// flag para fazer transformacao
	flag = true;

	for (unsigned short i = 0; i < steps; i++)
		transformations.push_back(aux);

}

void model::back(vec3 back, double time) {
	back.x = -1;
	back.y = 1;
	back.z = -1;
	flag = true;
	mat4 aux = scale(back);
	transformations.push_back(aux);
}