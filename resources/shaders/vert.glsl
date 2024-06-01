#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;


out VS_OUT {
    vec3 normal;
    vec4 pos;
} vs_out;

uniform mat4 camMatrix;
uniform mat4 model;

void main(){
    vec4 res = camMatrix * model * vec4(aPos, 1.0);
    vs_out.normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.pos = res;
    gl_Position = res;
}