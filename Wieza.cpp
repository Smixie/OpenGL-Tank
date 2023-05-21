#include <stdio.h>
#include <cstdlib>

#include "wieza.h"
#include "loadOBJ.h"
#include "shaderprogram.h"

#define model "modele/wizalol.obj"

using namespace glm;

Wieza::Wieza() {
    bool res = loadOBJ(model, this->vertices, this->uvs, this->normals, this->vertexCount);
    if(!res) {
        fprintf(stderr,"Blad - nie wczytano modelu - wieza");
        exit(1);
    } else fprintf(stderr,"Wieza - wczytano\n");
    position = vec3(0.0f,-0.05f,-0.55f);
    przesunDoZera = vec3(-0.00106f,-0.98469f+0.05f,0.67054f+0.42f);
    obrot = 0.0f;
    angleZ = 0.0f;
}

Wieza::~Wieza() {
    vertices.clear();
    uvs.clear();
    normals.clear();
}

void Wieza::drawWieza(GLuint &tex, ShaderProgram *sp) {
    float *verts= &(this->vertices[0]);
    float *normals= &(this->normals[0]);
	float *texCoords= &(this->uvs[0]);

    mat4 M=glm::mat4(1.0f);
	M=rotate(M,-90 * PI / 180,vec3(1.0f,0.0f,0.0f));
	M=translate(M,position);

	M=rotate(M,this-> angleZ * PI / 180,vec3(0.0f, 0.0f, 1.0f));

	M=translate(M,przesunDoZera);
    M=rotate(M,this-> obrot,vec3(0.0f, 0.0f, 1.0f));
    M=translate(M,-przesunDoZera);

    Model::drawSolid(tex, sp, verts, normals, texCoords, vertexCount, M);

}
