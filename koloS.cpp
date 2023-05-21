#include <stdio.h>
#include <cstdlib>

#include "koloS.h"
#include "loadOBJ.h"
#include "shaderprogram.h"

#define model "modele/kolomale.obj"

using namespace glm;

KoloS::KoloS(vec3 starter) {
    bool res = loadOBJ(model, this->vertices, this->uvs, this->normals, this->vertexCount);
    if(!res) {
        fprintf(stderr,"Blad - nie wczytano modelu - koloS");
        exit(1);
    } else fprintf(stderr,"KoloS - wczytano\n");
    position = starter;
    start = starter;
    przesunDoZera = vec3(0.0f,-3.29f,-0.91359f);
    angleZ = 0.0f;
}

KoloS::~KoloS() {
    vertices.clear();
    uvs.clear();
    normals.clear();
}

void KoloS::drawKoloS(GLuint &tex, ShaderProgram *sp) {
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

    M=translate(M,przesunDoZera);
    M=rotate(M,this-> obrot,vec3(1.0f, 0.0f, 0.0f));
    M=translate(M,-przesunDoZera);

    Model::drawSolid(tex, sp, verts, normals, texCoords, vertexCount, M);

}
