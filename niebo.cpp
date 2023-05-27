#include <stdio.h>
#include <cstdlib>

#include "niebo.h"
#include "loadOBJ.h"
#include "shaderprogram.h"

#define model "modele/niebo.obj"

using namespace glm;

Niebo::Niebo() {
    bool res = loadOBJ(model, this->vertices, this->uvs, this->normals, this->vertexCount);
    if(!res) {
        fprintf(stderr,"Blad - nie wczytano modelu - niebo");
        exit(1);
    } else fprintf(stderr,"Niebo - wczytano\n");
    pozycja = vec3(0.0f,0.0f,0.0f);
    scale = vec3(40.0f,40.0f,40.0f);
    angleX = 0.0f;
}

Niebo::~Niebo() {
    vertices.clear();
    uvs.clear();
    normals.clear();
}

void Niebo::drawNiebo(GLuint &tex, ShaderProgram *sp) {
    float *verts= &(this->vertices[0]);
    float *normals= &(this->normals[0]);
	float *texCoords= &(this->uvs[0]);
	unsigned int vertexCount= this->vertexCount;

    glm::mat4 M=glm::mat4(1.0f);
	M=glm::translate(M,vec3(pozycja[0], pozycja[2], -pozycja[1]));
	M=glm::scale(M,scale);
	M=rotate(M,90 * PI / 180,vec3(1.0,0.0,0.0));
	M=rotate(M,angleX,vec3(0.0,0.0,1.0)); // Kierunek obrotu chmur

    Model::drawSolid(tex, sp, verts, normals, texCoords, vertexCount, M);
}
