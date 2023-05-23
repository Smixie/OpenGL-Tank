#pragma once
#ifndef POCISK_H
#define POCISK_H

#include "model.h"
#include "shaderprogram.h"

class Pocisk : public Model {
public:
    Pocisk(vec3 shootpos, float obr) ;
    ~Pocisk();
    float speed;
    int life;
    vec3 shootzero;
    void drawPocisk(GLuint& tex, ShaderProgram* sp);
    void Update(float depression);
};

#endif // POCISK_H