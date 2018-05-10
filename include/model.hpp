#pragma once

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <texture.hpp>

class model
{
private:
	GLuint Texture;
	GLuint meshID;
	GLuint TextureID;
	GLuint ModelMatrixID;
	glm::mat4 ModelMatrix;
	bool flag;

	std::vector<mat4> transformations;


public:
	// constructor
	model(const char* Texture, GLuint programID, glm::vec3 position, GLuint meshID);

	// getters
	glm::mat4 getModelMatrix();
	GLuint getModelMatrixID();
	GLuint getTexture();
	GLuint getTextureID();
	GLuint getMeshID();
	int getFlag();
	void setFlag(int newFlag);

	// setters
	void setModelMatrix(mat4 newMatrix);

	// misc
	void clear();
	void transformationMatrix();

	// transformations
	void translateOP(vec3 translation, double time);
	void scaleOP(vec3 scaleSize, double time);
};
