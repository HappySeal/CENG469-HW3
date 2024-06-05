#version 330 core
#define SEGMENTS 6

layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;

in VS_OUT {
    vec4 normal;
    vec4 pos;
    vec4 world_pos;
} gs_in[];

out vec3 color;

uniform float uTime;
uniform float uHue;

uniform sampler2D uWindMapX1;
uniform sampler2D uWindMapX2;
uniform sampler2D uWindMapX4;


uniform float uHeight = .6;
const float WIDTH = -0.02;
const float WINDMAP_SIZE = 16.0;


float hueToRgb(float p, float q, float t) {
    if (t < 0.0) t += 1.0;
    if (t > 1.0) t -= 1.0;
    if (t < 1.0/6.0) return p + (q - p) * 6.0 * t;
    if (t < 1.0/2.0) return q;
    if (t < 2.0/3.0) return p + (q - p) * (2.0/3.0 - t) * 6.0;
    return p;
}

vec3 hslToRgb(float h, float s, float l) {
    float r, g, b;

    if (s == 0.0) {
        r = g = b = l; // achromatic
    } else {
        float q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
        float p = 2.0 * l - q;
        r = hueToRgb(p, q, h + 1.0/3.0);
        g = hueToRgb(p, q, h);
        b = hueToRgb(p, q, h - 1.0/3.0);
    }

    return vec3(r, g, b);
}

void GenerateLine(int index)
{
    for (int i = 0;  i < SEGMENTS; i++) {
        float t = float(i) / float(SEGMENTS - 1);
        float height = uHeight * (t);
        float width = WIDTH * (1 - t);

        vec2 uv = vec2(
            (gs_in[index].world_pos.x + uTime ) / WINDMAP_SIZE,
            (gs_in[index].world_pos.z + uTime ) / WINDMAP_SIZE
        );

        vec3 skew = texture(uWindMapX1, uv).xyz * t * 2 + texture(uWindMapX2, uv * 2).xyz * t + texture(uWindMapX4, uv * 4).xyz * t * .5;
        float lightness = height / uHeight * 0.5 + 0.25;


        gl_Position = gs_in[index].pos + vec4(width + skew.x,0.0,0.0, 0.0)  + gs_in[index].normal * height;
        color = hslToRgb(uHue, 0.6, lightness) ;
        EmitVertex();

        gl_Position = gs_in[index].pos + vec4(-width + skew.x ,0.0,0.0, 0.0) + gs_in[index].normal * height;
        color = hslToRgb(uHue, 0.6, lightness) ;
        EmitVertex();
    }
//
//    gl_Position = gs_in[index].pos + vec4(1.0,0.0,0.0, 0.0) * WIDTH;
//
//    EmitVertex();
//    gl_Position = (gs_in[index].pos + vec4(-1.0,0.0,0.0, 0.0) * WIDTH);
//
//    color = vec3(0.0, 0.2, 0.0);
//    EmitVertex();
//
//    gl_Position = (gs_in[(index)].pos + gs_in[index].normal * HEIGHT);
//    color = vec3(0.0, 0.6, 0.0);
//    EmitVertex();

    EndPrimitive();
}

void main()
{
    GenerateLine(0);// first vertex normal
}
