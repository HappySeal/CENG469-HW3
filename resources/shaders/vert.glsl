#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;


out VS_OUT {
    vec4 normal;
    vec4 pos;
    vec4 world_pos;
    vec4 world_normal;
} vs_out;

uniform mat4 camMatrix;
uniform mat4 model;

void main(){
    vec4 res =  camMatrix * model * vec4(aPos, 1.0);
    vs_out.normal = vec4(aPos + aNormal,1.0);
    vs_out.pos = res;
    vs_out.world_pos = model * vec4(aPos, 1.0);
    vs_out.world_normal = transpose(inverse(camMatrix * model)) * vec4(aNormal,1.0);
    gl_Position = res;
}