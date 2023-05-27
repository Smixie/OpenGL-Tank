#ifndef KOLODUZE_H
#define KOLODUZE_H

#include "model.h"
#include "shaderprogram.h"

class KoloDuze: public Model {
public:
    KoloDuze(vec3 starter);
    ~KoloDuze();
    void drawKoloDuze(GLuint &tex, ShaderProgram *sp);
    vec3 start;
};

#endif // KOLODUZE_H
