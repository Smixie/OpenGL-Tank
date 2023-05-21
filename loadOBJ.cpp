#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <limits>
#include <string>
#include "loadOBJ.h"

bool loadOBJ(
    const char* path,
    std::vector < float >& out_vertices,
    std::vector < float >& out_uvs,
    std::vector < float >& out_normals,
    int& vertexCount
) {
    // Tymczasowe zmienne to przechowywania informacji
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< float > temp_vertices;
    std::vector< float > temp_uvs;
    std::vector< float > temp_normals;

    //Otwarcie pliku
    FILE* file;
    fopen_s(&file,path, "r");
    if (file == NULL) {
        fprintf(stderr, "Can't read file!!! %s\n", path);
        return false;
    }

    //Wczytywanie z pliku do koñca
    while (1) {
        char lineHeader[512];
        int res = fscanf_s(file, "%s", lineHeader);
        if (res == EOF)
            break;
        if (strcmp(lineHeader, "v") == 0) {
            float x;
            float y;
            float z;
            fscanf_s(file, "%f %f %f\n", &x, &y, &z);
            temp_vertices.push_back(x);
            temp_vertices.push_back(-z);
            temp_vertices.push_back(y);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            float uvx;
            float uvy;
            fscanf_s(file, "%f %f\n", &uvx, &uvy);
            temp_uvs.push_back(uvx);
            temp_uvs.push_back(1 - uvy);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            float nx;
            float ny;
            float nz;
            fscanf_s(file, "%f %f %f\n", &nx, &ny, &nz);
            temp_normals.push_back(nx);
            temp_normals.push_back(-nz);
            temp_normals.push_back(ny);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9) {
                fprintf(stderr, "Wrong file format! Try with another options! \n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }

        for (unsigned int i = 0; i < vertexIndices.size(); i++) {
            unsigned int vertexIndex = vertexIndices[i];
            float vertex = temp_vertices[vertexIndex - 1];
            out_vertices.push_back(vertex);
        }

        for (unsigned int j = 0; j < uvIndices.size(); j++) {
            unsigned int uvIndex = uvIndices[j];
            float uv = temp_uvs[uvIndex - 1];
            out_uvs.push_back(uv);
        }

        for (unsigned int k = 0; k < normalIndices.size(); k++) {
            unsigned int normalIndex = normalIndices[k];
            float normal = temp_normals[normalIndex - 1];
            out_normals.push_back(normal);
        }
        return true;
    };
};