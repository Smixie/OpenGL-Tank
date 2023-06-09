#pragma once
#ifndef KADLUB_H
#define KADLUB_H

#include "model.h"
#include "shaderprogram.h"

class Kadlub : public Model {
public:
    Kadlub();
    ~Kadlub();
    void drawKadlub(GLuint& tex, ShaderProgram* sp);
};


#endif // KADLUB_H