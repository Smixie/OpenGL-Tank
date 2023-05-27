#include <stdio.h>
#include <cstdlib>

#include "lufa.h"
#include "loadOBJ.h"
#include "shaderprogram.h"

#define model "modele/lufav2.obj"

using namespace glm;

Lufa::Lufa() {
	bool res = loadOBJ(model, this->vertices, this->uvs, this->normals, this->vertexCount);
	if (!res) {
		fprintf(stderr, "Blad - nie wczytano modelu - lufa");
		exit(1);
	}
	else fprintf(stderr, "Lufa - wczytano\n");
	pozycja = vec3(0.0f, -0.05f, -0.42f);
	przesunDoZera = vec3(-0.00106f, -0.98469f + 0.05f, 0.67054f + 0.42f);
	obrot = 0.0f;
	podniesienie = 0.0f;
}

Lufa::~Lufa() {
	vertices.clear();
	uvs.clear();
	normals.clear();
}

void Lufa::drawLufa(GLuint& tex, ShaderProgram* sp) {
	float* verts = &(this->vertices[0]);
	float* normals = &(this->normals[0]);
	float* texCoords = &(this->uvs[0]);
	unsigned int vertexCount = this->vertexCount;

	mat4 M = mat4(1.0f);
	M = rotate(M, -90 * PI / 180, vec3(1.0f, 0.0f, 0.0f));
	M = translate(M, pozycja);

	M = rotate(M, this->angleZ * PI / 180, vec3(0.0f, 0.0f, 1.0f));

	M = translate(M, przesunDoZera);
	M = rotate(M, this->obrot, vec3(0.0f, 0.0f, 1.0f));
	M = translate(M, vec3(0, -1.156 - 0.16f, 0));
	M = rotate(M, this->podniesienie, vec3(1.0f, 0.0f, 0.0f));
	M = translate(M, vec3(0, 1.156 + 0.16f, 0));
	M = translate(M, -przesunDoZera);

	Model::drawSolid(tex, sp, verts, normals, texCoords, vertexCount, M);
}
