#ifndef GASIENNICA_H
#define GASIENNICA_H

#include "model.h"
#include "shaderprogram.h"

class Gasienica: public Model {
public:
    Gasienica(vec3 starter);
    ~Gasienica();
    void drawGasienica(GLuint &tex, ShaderProgram *sp);
    vec3 start;
};

#endif // GASIENNICA_H
