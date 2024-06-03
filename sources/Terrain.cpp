//
// Created by Cafer SELLI on 1.06.2024.
//

#include "../headers/Terrain.h"

Terrain::Terrain(int width, int height){
    this->width = width;
    this->height = height;
    NUM_STRIPS = (height * resolution - 1);
    VERTICES_PER_STRIP = width * 2 * resolution;
    generateTerrain();
}

void Terrain::generateTerrainVertices(){
    glm::vec3 v0 = glm::vec3(-width / 2.0f, 0.0f, -height / 2.0f);
    glm::vec3 v1 = glm::vec3(width / 2.0f, 0.0f, -height / 2.0f);
    glm::vec3 v2 = glm::vec3(-width / 2.0f, 0.0f, height / 2.0f);
    glm::vec3 v3 = glm::vec3(width / 2.0f, 0.0f, height / 2.0f);

    glm::vec3 dir03 = glm::vec3(1, 0, 0) / resolution;
    glm::vec3 dir12 = glm::vec3(0, 0, 1) / resolution;

    // dir2 and dir3
    for (float i = 0; i < resolution + 1; i++)
    {
        // dir1
        for (float j = 0; j < resolution + 1; j++)
        {
            glm::vec3 acrossj = ((v1 + i * dir12) - (v0 + i * dir03)) / float(resolution);
            glm::vec3 crntVec = v0 + i * dir03 + j * acrossj;
            // Position
            vertices.push_back(crntVec.x);
            vertices.push_back(crntVec.y);
            vertices.push_back(crntVec.z);
            // Texture
            vertices.push_back(j / resolution);
            vertices.push_back(i / resolution);
        }
    }

    std::cout << "Terrain Vertices generated..." << std::endl;
}

void Terrain::generateTerrainIndices(){

    for (int row = 0; row < resolution; row++)
    {
        for (int col = 0; col < resolution; col++)
        {
            int index = row * (resolution + 1) + col;			// 3___2
            indices.push_back(index);					//     |
            indices.push_back(index + 1);				//     |
            indices.push_back(index + (resolution + 1) + 1);	//  ___|
            indices.push_back(index + (resolution + 1));		// 0   1
        }
    }

    std::cout << "Terrain Indices generated..." << std::endl;
}

void Terrain::generateTerrain() {
    generateTerrainVertices();
    generateTerrainIndices();

//    float freq = 1, amp = 1;
//    float yScale = 16.0f, yShift = 16.0f;  // apply a scale+shift to the height data
//    for(unsigned int i = 0; i < height * resolution; i++)
//    {
//        for(unsigned int j = 0; j < width * resolution; j++)
//        {
//            // retrieve texel for (i,j) tex coord
//            // raw height at coordinate
////            freq = 1;
////            amp = 1;
////            float y = 0;
////            for(int k = 0; k < 8; k++) {
////                y += perlin_noise(i / resolution * freq / GRID_SIZE, j / resolution * freq / GRID_SIZE) * amp;
////                freq *= 2;
////                amp *= 0.5;
////            }
//
//            //TODO: Update height according to the perlin noise
//            float y = 1;
//
//            if(y < -1) y = -1;
//            if(y > 1) y = 1;
//
//            y = y * yScale + yShift; // apply scale and shift
//
//            // vertex
//            vertices.push_back( -height/2.0f + i / resolution );        // v.x
//            vertices.push_back(y); // v.y
//            vertices.push_back( -width/2.0f + j / resolution );        // v.z
//        }
//    }

    // calculate normals

    // index generation
//    for(unsigned int i = 0; i < height*resolution - 1; i++)       // for each row a.k.a. each strip
//    {
//        for(unsigned int j = 0; j < width*resolution; j++)      // for each column
//        {
//            for(unsigned int k = 0; k < 2; k++)      // for each side of the strip
//            {
//                indices.push_back(j + width * resolution * (i + k));
//            }
//        }
//    }

//    for(unsigned int i = 0; i < height*resolution - 1; i++)       // for each row a.k.a. each strip
//    {
//        for (unsigned int j = 0; j < width * resolution; j++)      // for each column
//        {
//            glm::vec3 norm;
//            int dx = 1, dy = 1;
//            if(i == height*resolution - 1) dy = - 1;
//            if(j == width*resolution - 1) dx = -1;
//
//            // calculate normal for the current vertex
//            glm::vec3 v0(vertices[(i * width * resolution + j) * 3],
//                         vertices[(i * width * resolution + j) * 3 + 1],
//                         vertices[(i * width * resolution + j) * 3 + 2]);
//
//            glm::vec3 v1(vertices[((i + dy) * width * resolution + j) * 3],
//                            vertices[((i + dy) * width * resolution + j) * 3 + 1],
//                            vertices[((i + dy) * width * resolution + j) * 3 + 2]);
//
//            glm::vec3 v2(vertices[(i * width * resolution + j + dx) * 3],
//                            vertices[(i * width * resolution + j + dx) * 3 + 1],
//                            vertices[(i * width * resolution + j + dx) * 3 + 2]);
//
//            norm = glm::normalize(glm::cross(v1 - v0, v2 - v0));
//
//            normals.push_back(norm.x);
//            normals.push_back(norm.y);
//            normals.push_back(norm.z);
//        }
//    }

    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &EBO);

    glNamedBufferData(VBO, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
    glNamedBufferData(EBO, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

    glEnableVertexArrayAttrib(VAO, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);

    glEnableVertexArrayAttrib(VAO, 1);
    glVertexArrayAttribBinding(VAO, 1, 0);
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(float));
    glVertexArrayElementBuffer(VAO, EBO);

    glCreateTextures(GL_TEXTURE_2D, 1, &screen);
    glTextureParameteri(screen, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(screen, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(screen, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(screen, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(screen, 1, GL_RGBA32F, width, height);
    glBindImageTexture(0, screen, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

float Terrain::getHeightAt(const glm::vec2& pos) const{
    if(pos.x < -width/2 || pos.x > width/2 || pos.y < -height/2 || pos.y > height/2)
        return 0;
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
        glDrawElements(GL_PATCHES,   // primitive type
                       VERTICES_PER_STRIP, // number of indices to render
                       GL_UNSIGNED_INT,     // index data type
                       (void*)(sizeof(unsigned int)
                               * VERTICES_PER_STRIP
                               * strip)); // offset to starting index
    }
}