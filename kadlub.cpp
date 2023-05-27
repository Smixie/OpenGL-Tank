#include <stdio.h>
#include <cstdlib>

#include "kadlub.h"
#include "loadOBJ.h"
#include "shaderprogram.h"

#define model "modele/kadlub.obj"

using namespace glm;

Kadlub::Kadlub() {
	bool res = loadOBJ(model, this->vertices, this->uvs, this->normals, this->vertexCount);
	if (!res) {
		fprintf(stderr, "Blad - nie wczytano modelu - kadlub");
		exit(1);
	}
	else fprintf(stderr, "Kadlub - wczytano\n");
	pozycja = vec3(0.0f, 0.0f, -0.1f);
	angleZ = 0.0f;
}

Kadlub::~Kadlub() {
	vertices.clear();
	uvs.clear();
	normals.clear();
}

void Kadlub::drawKadlub(GLuint& tex, ShaderProgram* sp) {
	float* verts = &(this->vertices[0]);
	float* normals = &(this->normals[0]);
	float* texCoords = &(this->uvs[0]);
	unsigned int vertexCount = this->vertexCount;

	mat4 M = mat4(1.0f);
	M = rotate(M, -90 * PI / 180, vec3(1.0f, 0.0f, 0.0f));
	M = translate(M, pozycja);

	M = rotate(M, this->angleZ * PI / 180, vec3(0.0f, 0.0f, 1.0f));

	Model::drawSolid(tex, sp, verts, normals, texCoords, vertexCount, M);

}