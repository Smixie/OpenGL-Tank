#pragma once

#include <iostream>
#include <vector>

#include "shaderprogram.h"
#include "Vertex.h"

class Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	void initVertexData(Vertex* vertexArray,const unsigned&){}

public:
	Mesh() {

	}

	~Mesh() {

	}
	void update();
	void render(ShaderProgram* shader)
};
