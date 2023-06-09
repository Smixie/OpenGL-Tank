#include <stdio.h>
#include <cstdlib>

#include "koloDuze.h"
#include "loadOBJ.h"
#include "shaderprogram.h"

#define model "modele/koloduze.obj"

using namespace glm;

KoloDuze::KoloDuze(vec3 starter) {
    bool res = loadOBJ(model, this->vertices, this->uvs, this->normals, this->vertexCount);
    if(!res) {
        fprintf(stderr,"Blad - nie wczytano modelu - koloL");
        exit(1);
    } else fprintf(stderr,"koloduze - wczytano\n");
    pozycja = starter + vec3(0.0f,0.0f,-1.06f);
    start = starter;
    angleZ = 0.0f;
}

KoloDuze::~KoloDuze() {
    vertices.clear();
    uvs.clear();
    normals.clear();
}

void KoloDuze::drawKoloDuze(GLuint &tex, ShaderProgram *sp) {
    float *verts= &(this->vertices[0]);
    float *normals= &(this->normals[0]);
	float *texCoords= &(this->uvs[0]);
	unsigned int vertexCount= this->vertexCount;

    mat4 M=mat4(1.0f);
	M=rotate(M,-90 * PI / 180,vec3(1.0f,0.0f,0.0f));
	M=translate(M,pozycja);

	M=translate(M,-start);
    M=rotate(M,this-> angleZ * PI / 180,vec3(0.0f, 0.0f, 1.0f));
    M=translate(M,start);

    M=rotate(M,this->speed,vec3(1.0f, 0.0f, 0.0f));

    Model::drawSolid(tex, sp, verts, normals, texCoords, vertexCount, M);

}
