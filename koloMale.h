#ifndef KOLOMALE_H
#define KOLOMALE_H

#include "model.h"
#include "shaderprogram.h"

class KoloMale: public Model {
public:
    KoloMale(vec3 starter);
    ~KoloMale();
    void drawKoloMale(GLuint &tex, ShaderProgram *sp);
    vec3 start;
};

#endif // KOLOMALE_H
