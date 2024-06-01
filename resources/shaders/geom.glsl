#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 normal;
    vec4 pos;
} gs_in[];

out vec3 color;

const float HEIGHT = .6;
const float WIDTH = 0.02;

void GenerateLine(int index)
{
    gl_Position = gs_in[index].pos + vec4(1.0,0.0,0.0, 0.0) * WIDTH;
    color = vec3(0.0, 0.2, 0.0);
    EmitVertex();
    gl_Position = (gs_in[index].pos + vec4(-1.0,0.0,0.0, 0.0) * WIDTH);
    color = vec3(0.0, 0.2, 0.0);
    EmitVertex();
    gl_Position = (gs_in[(index)].pos + vec4(0.0,1.0,0.0, 0.0) * HEIGHT);
    color = vec3(0.0, 0.6, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
}