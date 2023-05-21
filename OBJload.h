#ifndef LOADOBJ_H
#define LOADOBJ_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <vector>


bool loadOBJ(const char* path,
    std::vector < glm::vec3 >& out_vertices,
    std::vector < glm::vec2 >& out_uvs,
    std::vector < glm::vec3 >& out_normals,
    int& vertexCount
);

#endif