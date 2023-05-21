#include <stdio.h>
#include <cstdlib>

#include "gasiennica.h"
#include "loadOBJ.h"
#include "shaderprogram.h"

#define model "modele/gasiennica.obj"

using namespace glm;

Gasiennica::Gasiennica(vec3 starter) {
	bool res = loadOBJ(model, this->vertices, this->uvs, this->normals, this->vertexCount);
	if (!res) {
		fprintf(stderr, "Blad - nie wczytano modelu - gasiennica");
		exit(1);
	}
	else fprintf(stderr, "Gasiennica - wczytano\n");
	position = starter;
	start = starter;
	angleZ = 0.0f;
}

Gasiennica::~Gasiennica() {
	vertices.clear();
	uvs.clear();
	normals.clear();
}

void Gasiennica::drawGasienica(GLuint& tex, ShaderProgram* sp) {
	float* verts = &(this->vertices[0]);
	float* normals = &(this->normals[0]);
	float* texCoords = &(this->uvs[0]);
	unsigned int vertexCount = this->vertexCount;

	mat4 M = glm::mat4(1.0f);
	M = rotate(M, -90 * PI / 180, vec3(1.0f, 0.0f, 0.0f)); // Obrót
	M = translate(M, position); // Ustawienie na dane pozycje

	M = translate(M, -start); // połączenie z czołgiem
	M = rotate(M, this->angleZ * PI / 180, vec3(0.0f, 0.0f, 1.0f));
	M = translate(M, start);

	Model::drawSolid(tex, sp, verts, normals, texCoords, vertexCount, M);
}
