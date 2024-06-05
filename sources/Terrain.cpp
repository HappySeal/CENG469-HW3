//
// Created by Cafer SELLI on 1.06.2024.
//

#include "../headers/Terrain.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../headers/stb_image_write.h"

#include "string"

Terrain::Terrain(int width, int height){
    this->width = width;
    this->height = height;
    NUM_STRIPS = (height * resolution - 1);
    VERTICES_PER_STRIP = width * 2 * resolution;
    generateTerrain();
    generateWindMap();
}

void Terrain::generateTerrain() {
    float freq = 1, amp = 1;
    float yScale = 16.0f, yShift = -4.0f;  // apply a scale+shift to the height data
    for(unsigned int i = 0; i < height * resolution; i++)
    {
        for(unsigned int j = 0; j < width * resolution; j++)
        {
            // retrieve texel for (i,j) tex coord
            // raw height at coordinate
            freq = 1;
            amp = 1;
            float y = 0;
            for(int k = 0; k < 8; k++) {
                y += perlin_noise(i / resolution * freq / GRID_SIZE, j / resolution * freq / GRID_SIZE) * amp;
                freq *= 2;
                amp *= 0.5;
            }

            if(y < -1) y = -1;
            if(y > 1) y = 1;

            y = y * yScale + yShift; // apply scale and shift

            // vertex
            vertices.push_back( -height/2.0f + i / resolution );        // v.x
            vertices.push_back(y); // v.y
            vertices.push_back( -width/2.0f + j / resolution );        // v.z
        }
    }

    // calculate normals

    // index generation
    for(unsigned int i = 0; i < height*resolution - 1; i++)       // for each row a.k.a. each strip
    {
        for(unsigned int j = 0; j < width*resolution; j++)      // for each column
        {
            for(unsigned int k = 0; k < 2; k++)      // for each side of the strip
            {
                indices.push_back(j + width * resolution * (i + k));
            }
        }
    }

    for(unsigned int i = 0; i < height*resolution - 1; i++)       // for each row a.k.a. each strip
    {
        for (unsigned int j = 0; j < width * resolution; j++)      // for each column
        {
            glm::vec3 norm;
            int dx = 1, dy = 1;
            if(i == height*resolution - 1) dy = - 1;
            if(j == width*resolution - 1) dx = -1;

            // calculate normal for the current vertex
            glm::vec3 v0(vertices[(i * width * resolution + j) * 3],
                         vertices[(i * width * resolution + j) * 3 + 1],
                         vertices[(i * width * resolution + j) * 3 + 2]);

            glm::vec3 v1(vertices[((i + dy) * width * resolution + j) * 3],
                            vertices[((i + dy) * width * resolution + j) * 3 + 1],
                            vertices[((i + dy) * width * resolution + j) * 3 + 2]);

            glm::vec3 v2(vertices[(i * width * resolution + j + dx) * 3],
                            vertices[(i * width * resolution + j + dx) * 3 + 1],
                            vertices[(i * width * resolution + j + dx) * 3 + 2]);

            norm = glm::normalize(glm::cross(v2 - v0,v1 - v0));

            normals.push_back(norm.x);
            normals.push_back(norm.y);
            normals.push_back(norm.z);
        }
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),       // size of vertices buffer
                 &vertices[0],                          // pointer to first element
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
//
    glGenBuffers(1, &nVBO);
    glBindBuffer(GL_ARRAY_BUFFER, nVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 normals.size() * sizeof(float),       // size of vertices buffer
                 &normals[0],                          // pointer to first element
                 GL_STATIC_DRAW);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int), // size of indices buffer
                 &indices[0],                           // pointer to first element
                 GL_STATIC_DRAW);
}

void Terrain::generateWindMap(){
    float freq = 1;
    float offset = 0.5;

    unsigned int* windmap_ptr = &windMapX1;


    for (int k = 0; k < 3; ++k) {
        float* windMap;
        windMap = new float[int(windMapResolution * windMapResolution * 3)];


        for (int i = 0; i < windMapResolution; ++i) {
            for (int j = 0; j < windMapResolution; ++j) {
                float windX = perlin_noise(i * freq / windMapResolution, j  * freq / windMapResolution);
                float windZ = perlin_noise(i * freq / windMapResolution + offset, j  * freq / windMapResolution + offset);
//                windMap.push_back(windX);
//                windMap.push_back(windZ);
//                windMap.push_back(0.0f);
                windMap[int(i * windMapResolution + j) * 3] = windX;
                windMap[int(i * windMapResolution + j) * 3 + 1] = windZ;
                windMap[int(i * windMapResolution + j) * 3 + 2] = 0;
            }
        }

        freq *= 2;

        glGenTextures(1, windmap_ptr);
        glBindTexture(GL_TEXTURE_2D, *windmap_ptr);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, int(windMapResolution), int(windMapResolution), 0, GL_RGB, GL_FLOAT, windMap); // note how we specify the texture's data value to be float

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        switch (k) {
            case 0:
                offset = 0.25;
                windmap_ptr = &windMapX2;
                break;
            case 1:
                offset = 0.125;
                windmap_ptr = &windMapX4;
                break;
            default:
                break;
        }

//        std::string filename = "windMapX" + std::to_string(1 << (k)) + ".png";
//        stbi_write_png(filename.c_str(), windMapResolution, windMapResolution, 3, windMapX1Chr, 0);
    }

}

float Terrain::getHeightAt(const glm::vec2& pos) const{
    if(pos.x < -width/2 || pos.x > width/2 || pos.y < -height/2 || pos.y > height/2)
        return -1.0f;
    int i = (int)((pos.x + height/2) * resolution);
    int j = (int)((pos.y + width/2) * resolution);

    float y00 = vertices[(i * width * resolution + j)*3 + 1];
    float y01 = vertices[(i * width* resolution + j + 1)*3 + 1];
    float y10 = vertices[((i + 1) * width* resolution + j)*3 + 1];
    float y11 = vertices[((i + 1) * width * resolution+ j + 1)*3 + 1];

    float x = ((pos.x + height/2) * resolution - i);
    float y = ((pos.y + width/2) * resolution - j);

    float y0 = y00 * x + (1 - x) * y01;
    float y1 = y10 * x + (1 - x) * y11;

//    std::cout << x << " " << y << " = " << interpolate(y0, y1, x) << std::endl;

//    return interpolate(y0, y1, y);
    return y0 * y + y1 * ( 1 - y );
}

void Terrain::draw() {
    glBindVertexArray(VAO);
    // render the mesh triangle strip by triangle strip - each row at a time
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for(unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
    {
        glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
                       VERTICES_PER_STRIP, // number of indices to render
                       GL_UNSIGNED_INT,     // index data type
                       (void*)(sizeof(unsigned int)
                               * VERTICES_PER_STRIP
                               * strip)); // offset to starting index
    }
}