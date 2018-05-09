#include "model.hpp"



model::model(const char* Texture, glm::vec3 position, GLuint meshID)
{
	// Load the texture
	this->Texture = loadDDS(Texture);

}


void model::clear() {
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
}
