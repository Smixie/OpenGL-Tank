#pragma once
#ifndef POCISK_H
#define POCISK_H

#include "model.h"
#include "shaderprogram.h"

struct Particle {
    vec3 position; //Po�o�enie cz�stki
    vec3 pspeed; //Pr�dko�� cz�stki
    float ttl; //Czas �ycia
};

class Pocisk : public Model {
public:
    const int numberOfParticles = 1000; //Liczba cz�stek
    Particle psystem[1000]; //Tablica cz�stek
    vec3 gravity = vec3(0, -0.1, 0); //Wektor grawitacji
    Pocisk(vec3 shootpos, float obr) ;
    ~Pocisk();
    float speed;
    int life;
    vec3 shootzero;
    void drawPocisk(GLuint& tex, ShaderProgram* sp);
    void Update(float depression,float delta);
    void createParticle(Particle& p);
    void processSystem(Particle* psystem, vec3 gravity, int n, float timestep);
    void initializeSystem(Particle* psystem);
};

#endif // POCISK_H