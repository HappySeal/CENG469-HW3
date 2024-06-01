#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 controlPoints[];

uniform float time;
uniform vec3 windDirection;
uniform float windSpeed;
uniform float grassHeight;

float perlinNoise(vec3 position) {
    // Simple Perlin noise function
    return fract(sin(dot(position.xyz, vec3(12.9898, 78.233, 45.164))) * 43758.5453);
}

void main() {
    // Calculate Bezier control points
    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = p0 + vec3(0.0, grassHeight * 0.5, 0.0);
    vec3 p2 = p0 + vec3(0.0, grassHeight, 0.0);

    // Apply wind effect
    float windEffect = windSpeed * perlinNoise(p0.xyz + windDirection * time);
    p1.x += windEffect;
    p2.x += windEffect * 1.5;

    // Emit vertices
    gl_Position = vec4(p0, 1.0);
    EmitVertex();
    gl_Position = vec4(p1, 1.0);
    EmitVertex();
    gl_Position = vec4(p2, 1.0);
    EmitVertex();
    EndPrimitive();
}
