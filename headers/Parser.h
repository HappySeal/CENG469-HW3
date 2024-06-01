//
// Created by Cafer SELLI on 7.05.2024.
//

#ifndef SAMPLEGL_PARSER_H
#define SAMPLEGL_PARSER_H

#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>
#include <vector>
#include "Mesh.h"
#include "Vertex.h"
#include "Normal.h"


using namespace std;

class Parser
{
public:
    static bool ParseObj(const string& fileName, Mesh* mesh)
    {
        std::vector<Vertex> vertices;
        std::vector<Normal> normals;
        fstream myfile;

        // Open the input
        myfile.open(fileName.c_str(), std::ios::in);

        if (myfile.is_open())
        {
            string curLine;

            while (getline(myfile, curLine))
            {
                stringstream str(curLine);
                GLfloat c1, c2, c3;
                // GLuint index[9];
                string tmp;

                if (curLine.length() >= 2)
                {
                    if (curLine[0] == 'v')
                    {
                        if (curLine[1] == 'n') // normal
                        {
                            str >> tmp; // consume "vn"
                            str >> c1 >> c2 >> c3;
                            normals.push_back(Normal(c1, c2, c3));
                        }
                        else // vertex
                        {
                            str >> tmp; // consume "v"
                            str >> c1 >> c2 >> c3;
                            vertices.push_back(Vertex(c1, c2, c3));
                        }
                    }
                    else if (curLine[0] == 'f') // face
                    {
                        str >> tmp; // consume "f"
                        char c;
                        int vIndex[4],  nIndex[4];
                        str >> vIndex[0]; str >> c >> c; // consume "//"
                        str >> nIndex[0];
                        str >> vIndex[1]; str >> c >> c; // consume "//"
                        str >> nIndex[1];
                        str >> vIndex[2]; str >> c >> c; // consume "//"
                        str >> nIndex[2];
                        str >> vIndex[3]; str >> c >> c; // consume "//"
                        str >> nIndex[3];


                        if(vIndex[3] > vertices.size() || 1 > vIndex[3] || nIndex[3] > normals.size() || 1 > nIndex[3] )
                        {
                            mesh->renderingMode = 3;
                        }
                        else
                        {
                            mesh->renderingMode = 4;
                        }
                        for (int c = 0; c <  mesh->renderingMode; ++c)
                        {
                            vIndex[c] -= 1;
                            nIndex[c] -= 1;

                        }
                        if( mesh->renderingMode == 3)
                        {
                            for (int c = 0; c < 3; ++c)
                            {
                                mesh->vertices.push_back(vertices[vIndex[c]]);
                                mesh->normals.push_back(normals[nIndex[c]]);
                            }
                        }
                        else
                        {
                            mesh->vertices.push_back(vertices[vIndex[0]]);
                            mesh->normals.push_back(normals[nIndex[0]]);
                            mesh->vertices.push_back(vertices[vIndex[1]]);
                            mesh->normals.push_back(normals[nIndex[1]]);
                            mesh->vertices.push_back(vertices[vIndex[2]]);
                            mesh->normals.push_back(normals[nIndex[2]]);

                            mesh->vertices.push_back(vertices[vIndex[2]]);
                            mesh->normals.push_back(normals[nIndex[2]]);
                            mesh->vertices.push_back(vertices[vIndex[3]]);
                            mesh->normals.push_back(normals[nIndex[3]]);
                            mesh->vertices.push_back(vertices[vIndex[0]]);
                            mesh->normals.push_back(normals[nIndex[0]]);
                            //3 2
                            //0 1
                        }

                    }
                    else
                    {
                        cout << "Ignoring unidentified line in obj file: " << curLine << endl;
                    }
                }

                //data += curLine;
                if (!myfile.eof())
                {
                    //data += "\n";
                }
            }
            myfile.close();
        }
        else
        {
            return false;
        }

        //mesh->calculateNormals();
        assert( mesh->vertices.size() ==  mesh->normals.size());

        return true;
    }

    static bool ReadDataFromFile(const std::string& fileName, std::string& data)
    {
        fstream myfile;
        // Open the input
        myfile.open(fileName.c_str(), std::ios::in);

        if (myfile.is_open())
        {
            string curLine;

            while (getline(myfile, curLine))
            {
                data += curLine;
                if (!myfile.eof())
                {
                    data += "\n";
                }
            }

            myfile.close();
        }
        else
        {
            return false;
        }

        return true;
    }
};

#endif //SAMPLEGL_PARSER_H
