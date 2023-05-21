#include <stdio.h>
#include <cstdlib>

#include "koloL.h"
#include "loadOBJ.h"
#include "shaderprogram.h"

#define model "modele/koloduze.obj"

using namespace glm;

KoloL::KoloL(vec3 starter) {
    bool res = loadOBJ(model, this->vertices, this->uvs, this->normals, this->vertexCount);
    if(!res) {
        fprintf(stderr,"Blad - nie wczytano modelu - koloL");
        exit(1);
    } else fprintf(stderr,"koloduze - wczytano\n");
    position = starter;
    start = starter;
    przesunDoZera = vec3(0, 0, -1.06803);
    angleZ = 0.0f;
}

KoloL::~KoloL() {
    vertices.clear();
    uvs.clear();
    normals.clear();
}

void KoloL::drawKoloL(GLuint &tex, ShaderProgram *sp) {
    float *verts= &(this->vertices[0]);
    float *normals= &(this->normals[0]);
	float *texCoords= &(this->uvs[0]);
	unsigned int vertexCount= this->vertexCount;

    mat4 M=mat4(1.0f);
	M=rotate(M,-90 * PI / 180,vec3(1.0f,0.0f,0.0f));
	M=translate(M,position);

	M=translate(M,vec3(-start[0],-start[1],-start[2]));
    M=rotate(M,this-> angleZ * PI / 180,vec3(0.0f, 0.0f, 1.0f));
    M=translate(M,vec3(start[0],start[1],start[2]));

	M=translate(M, vec3(0, 0, -1.06803));
    M=rotate(M,this-> obrot,vec3(1.0f, 0.0f, 0.0f));
    M=translate(M,-vec3(0, 0, -1.06803));

    Model::drawSolid(tex, sp, verts, normals, texCoords, vertexCount, M);

}
