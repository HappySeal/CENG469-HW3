#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 camMatrix;
uniform mat4 model;

out vec3 currentPos;
out vec3 Normal;


void main(){
        currentPos = vec3(model * vec4(aPos,1.0f));
        Normal = mat3(transpose(inverse(model))) * aNormal;

        gl_Position = camMatrix * vec4(currentPos, 1.0);

}