#include <stdio.h>
#include <cstdlib>

#include "pocisk.h"
#include "loadOBJ.h"
#include "shaderprogram.h"

#define model "modele/bullet.obj"

using namespace glm;

Pocisk::Pocisk(vec3 shootpos, float obr) {
    bool res = loadOBJ("modele/bull.obj", this->vertices, this->uvs, this->normals, this->vertexCount);
    if (!res) {
        fprintf(stderr, "Blad - nie wczytano modelu - pocisk");
        exit(1);
    }
    else fprintf(stderr, "Pocisk - wczytano\n");
    position = shootpos + vec3(0.0f, 0.0f, -0.2f); // Set the initial position of the bullet 
	//printf("%f %f %f", position[0], position[1], position[2]);
    scale = vec3(0.2f, 0.2f, 0.2f); // Set the initial scale of the bullet
	obrot = obr;
	life = 150;
	speed = 0.1f;
}

Pocisk::~Pocisk() {
	vertices.clear();
	uvs.clear();
	normals.clear();
}

void Pocisk::drawPocisk(GLuint& tex, ShaderProgram* sp){
	float* verts = &(this->vertices[0]);
	float* normals = &(this->normals[0]);
	float* texCoords = &(this->uvs[0]);
	unsigned int vertexCount = this->vertexCount;

	mat4 M = mat4(1.0f);
	M = glm::translate(M, vec3(position[0], -position[2], -position[1]));
	M = glm::scale(M, scale);

	M = rotate(M, this->obrot, vec3(0.0f, 1.0f, 0.0f));


	Model::drawSolid(tex, sp, verts, normals, texCoords, vertexCount, M);
}

void Pocisk::Update(vec3 posLufa) {
	//printf(" %f %f %f ", posLufa[0], posLufa[1], posLufa[2]);
	this->position += 0.2f * vec3( sin(this->obrot),-cos(this->obrot), 0.0f);
	//printf("%f %f %f %f\n", this->obrot, -cos(radiany), -cos(this->obrot), -sin(this->obrot));
	this->life -= 1;
}