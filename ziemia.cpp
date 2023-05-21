#include <stdio.h>
#include <cstdlib>

#include "ziemia.h"
#include "loadOBJ.h"
#include "shaderprogram.h"

#define model "modele/ziemia.obj"

using namespace glm;

Ziemia::Ziemia() {
    bool res = loadOBJ(model, this->vertices, this->uvs, this->normals, this->vertexCount);
    if(!res) {
        fprintf(stderr,"Blad - nie wczytano modelu - ziemia");
        exit(1);
    } else fprintf(stderr,"Ziemia - wczytano\n");
    position = vec3(0.0f,0.0f,-1.75f);
}

Ziemia::~Ziemia() {
    vertices.clear();
    uvs.clear();
    normals.clear();
}

void Ziemia::drawZiemia(GLuint &tex, ShaderProgram *sp) {
    float *verts= &(this->vertices[0]);
    float *normals= &(this->normals[0]);
	float *texCoords= &(this->uvs[0]);
	unsigned int vertexCount= this->vertexCount;

    mat4 M=glm::mat4(1.0f);
	M=rotate(M,-90 * PI / 180,glm::vec3(1.0f,0.0f,0.0f));
	M=translate(M,position);

    Model::drawSolid(tex, sp, verts, normals, texCoords, vertexCount, M);
}
