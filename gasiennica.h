#ifndef GASIENNICA_H
#define GASIENNICA_H

#include "model.h"
#include "shaderprogram.h"

class Gasiennica: public Model {
public:
    Gasiennica(vec3 starter);
    ~Gasiennica();
    void drawGasienica(GLuint &tex, ShaderProgram *sp);
    vec3 start;
};

#endif // GASIENNICA_H
