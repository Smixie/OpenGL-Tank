#ifndef NIEBO_H
#define NIEBO_H

#include "model.h"
#include "shaderprogram.h"

class Niebo: public Model {
public:
    Niebo();
    ~Niebo();
    void drawNiebo(GLuint &tex, ShaderProgram *sp);
};


#endif // NIEBO_H
