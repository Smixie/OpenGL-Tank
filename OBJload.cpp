﻿#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <limits>
#include <string>
#include "loadOBJ.h"



bool loadOBJ(const char* path,
    std::vector < float >& out_vertices,
    std::vector < float >& out_uvs,
    std::vector < float >& out_normals,
    int& vertexCount
) {
    printf("\nWczytywanie pliku OBJ -> %s...\n", path);
    vertexCount = 0;
    std::vector < unsigned int > vertexIndices, uvIndices, normalIndices; // indeksy
    std::vector < float > temp_vertices; // wierzchołki tymczasowe
    std::vector < float > temp_uvs; // koordy tekstur
    std::vector < float > temp_normals; // normalne

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Z³y format pliku!\n");
        getchar();
        return false;
    }
    while (1) {
        char lineHeader[512];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
        if (strcmp(lineHeader, "v") == 0) {
            float x,y,z;
            fscanf_s(file, "%f %f %f\n", &x, &y, &z);
            temp_vertices.push_back(x);
            temp_vertices.push_back(-z);
            temp_vertices.push_back(y);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            float uvx, uvy;
            fscanf_s(file, "%f %f\n", &uvx, &uvy);
            temp_uvs.push_back(uvx);
            temp_uvs.push_back(1 - uvy);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            float nx, nz, ny;
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
                printf("Cos poszlo nie tak, sprobuj wyeksportowac uzywajac innych opcji\n"); //blad
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]); // zapisujemy do kontenera (typ unsigned int) indeksy kolejnych wierszy 'f'
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
        else {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    fclose(file);

    std::vector<float> temp_vect_normals(temp_vertices.size(), 0.0f);
    vertexCount = vertexIndices.size();
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = 3 * (vertexIndices[i] - 1);
        float vertex = temp_vertices[vertexIndex];
        out_vertices.push_back(vertex);
        vertex = temp_vertices[vertexIndex + 1];
        out_vertices.push_back(vertex);
        vertex = temp_vertices[vertexIndex + 2];
        out_vertices.push_back(vertex);
        out_vertices.push_back(1.0f);
        unsigned int normalIndex = 3 * (normalIndices[i] - 1);
        temp_vect_normals[vertexIndex] += temp_normals[normalIndex];
        temp_vect_normals[vertexIndex + 1] += temp_normals[normalIndex + 1];
        temp_vect_normals[vertexIndex + 2] += temp_normals[normalIndex + 2];
    }
    for (unsigned int i = 0; i < temp_vertices.size();) {
        glm::vec3 temp = glm::vec3(temp_vect_normals[i],
            temp_vect_normals[i + 1],
            temp_vect_normals[i + 2]);
        glm::vec3 temp2 = glm::normalize(temp);
        temp_vect_normals[i] = temp2.x;
        temp_vect_normals[i + 1] = temp2.y;
        temp_vect_normals[i + 2] = temp2.z;
        i += 3;
    }
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = 3 * (vertexIndices[i] - 1);
        float normal = temp_vect_normals[vertexIndex];
        out_normals.push_back(normal);
        normal = temp_vect_normals[vertexIndex + 1];
        out_normals.push_back(normal);
        normal = temp_vect_normals[vertexIndex + 2];
        out_normals.push_back(normal);
        out_normals.push_back(0.0f);
    }
    for (unsigned int i = 0; i < uvIndices.size(); i++) {
        unsigned int uvIndex = 2 * (uvIndices[i] - 1);
        float uv = temp_uvs[uvIndex];
        out_uvs.push_back(uv);
        uv = temp_uvs[uvIndex + 1];
        out_uvs.push_back(uv);
    }
    return true;
}