#include <stdio.h>
#include <cstdlib>
#include <random>

#include "pocisk.h"
#include "loadOBJ.h"
#include "shaderprogram.h"


#define model "modele/pocisk.obj"

using namespace glm;

Pocisk::Pocisk(vec3 shootpos, float obr) {
    bool res = loadOBJ(model, this->vertices, this->uvs, this->normals, this->vertexCount);
    if (!res) {
        fprintf(stderr, "Blad - nie wczytano modelu - pocisk");
        exit(1);
    }
    else fprintf(stderr, "Pocisk - wczytano\n");
	pozycja = shootpos + vec3(0.0f, -1.0f, -0.2f);// +vec3(sin(this->obrot), -cos(this->obrot), 0.0f); // Set the initial position of the bullet 
	//printf("%f %f %f", position[0], position[1], position[2]);
    scale = vec3(0.2f, 0.2f, 0.2f); // Set the initial scale of the bullet
	obrot = obr;
	life = 150;
	speed = 0.2f;
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
	M = glm::translate(M, vec3(pozycja[0], -pozycja[2], -pozycja[1]));
	M = glm::scale(M, scale);

	M = rotate(M, this->obrot, vec3(0.0f, 1.0f, 0.0f));

	Model::drawSolid(tex, sp, verts, normals, texCoords, vertexCount, M);

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 3, GL_FLOAT, false, 0, psystem); // Przesy³anie danych cz¹stek
	glDrawArrays(GL_POINTS, 0, this->numberOfParticles); // Renderowanie punktów dla ka¿dej cz¹stki
	glDisableVertexAttribArray(sp->a("vertex"));
}

void Pocisk::Update(float depression,float delta) {
	this->pozycja += (speed * vec3( sin(this->obrot),-cos(this->obrot), sin(depression)));
	if (-pozycja[2] < 0) {
		this->life = -1;
	}
	else
	{
		this->life -= 1;
	}
	Pocisk::processSystem(this->psystem, this->gravity, this->numberOfParticles, delta);
}

void Pocisk::createParticle(Particle& p) { //Zainicjowanie cz¹stki
	std::random_device rd; // Seed for the random number engine
	std::mt19937 gen(rd()); // Mersenne Twister random number engine
	std::normal_distribution<float> dist(-1.0f, 1.0f); // Normal distribution with mean 0 and standard deviation 1

	float radius = 1.5f; // Radius of the sphere
	float theta = 2.0f * PI * dist(gen); // Random angle in range [0, 2*pi]
	float phi = acos(2.0f * dist(gen) - 1.0f); // Random angle in range [0, pi]

	float x = radius * sin(phi) * cos(theta);
	float y = radius * sin(phi) * sin(theta);
	float z = radius * cos(phi);

	p.position = vec3(x,y,z);
	p.pspeed = vec3(0.01f, 0.01f, 0.01f);
	p.ttl = 0.1;
}

void Pocisk::initializeSystem(Particle* psystem) {//Zainicjowanie ka¿dej cz¹stki
	for (int i = 0; i < this->numberOfParticles; i++){
		Pocisk::createParticle(psystem[i]);
	}
}

void Pocisk::processSystem(Particle* psystem, vec3 gravity, int n, float timestep) {
	for (int i = 0; i < n; i++) {
		psystem[i].position += psystem[i].pspeed * timestep; //przesuniêcie
		psystem[i].pspeed += gravity * timestep; //uwzglêdnienie grawitacji
		psystem[i].ttl -= timestep; //Skrócenie czasu ¿ycia cz¹stki
	}
}