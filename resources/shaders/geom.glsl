#version 330 core
#define SEGMENTS 24

layout (triangles) in;
layout (triangle_strip, max_vertices = 48) out;

in VS_OUT {
    vec4 normal;
    vec4 pos;
    vec4 world_pos;
    vec4 world_normal;
} gs_in[];

out vec3 color;

uniform mat4 camMatrix;
uniform mat4 modelMatrix;

uniform float uTime;
uniform float uHue;

uniform sampler2D uWindMapX1;
uniform sampler2D uWindMapX2;
uniform sampler2D uWindMapX4;


uniform float uHeight = .6;
uniform float uWindSpeed = 3.0;

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

vec4 rotateAroundAxis(vec4 v, vec4 axis, float angle) {
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return vec4(
        oc * axis.x * axis.x + c,
        oc * axis.x * axis.y - axis.z * s,
        oc * axis.z * axis.x + axis.y * s,
        0.0
    ) * v.x +
    vec4(
        oc * axis.x * axis.y + axis.z * s,
        oc * axis.y * axis.y + c,
        oc * axis.y * axis.z - axis.x * s,
        0.0
    ) * v.y +
    vec4(
        oc * axis.z * axis.x - axis.y * s,
        oc * axis.y * axis.z + axis.x * s,
        oc * axis.z * axis.z + c,
        0.0
    ) * v.z;
}

vec4 bezier(vec4 p0, vec4 p1, vec4 p2, vec4 p3, float t) {
    float t2 = t * t;
    float t3 = t2 * t;
    float mt = 1.0 - t;
    float mt2 = mt * mt;
    float mt3 = mt2 * mt;

    vec4 pos = p0 * mt3 + p1 * 3.0 * mt2 * t + p2 * 3.0 * mt * t2 + p3 * t3;
    return pos;
}

void GenerateLine(int index)
{
    vec2 uv = vec2(
        (gs_in[index].world_pos.x + uTime *  uWindSpeed) / WINDMAP_SIZE,
        (gs_in[index].world_pos.z + uTime *  uWindSpeed) / WINDMAP_SIZE
    );

    vec3 wind = texture(uWindMapX1, uv).xyz;
    vec4 windDirection = vec4(wind.x, 0.0, wind.z , 0.0f) * uWindSpeed;
    vec4 windOrtho = vec4(-wind.z, 0.0, wind.x, 0.0f);

    vec4 p0 = gs_in[index].world_pos;
    vec4 p4= gs_in[index].world_pos + gs_in[index].normal * uHeight + windDirection * uHeight;
    vec4 p1 = gs_in[index].world_pos + gs_in[index].normal * uHeight * .5;
    vec4 p3 = gs_in[index].world_pos + gs_in[index].normal * uHeight + windDirection * uHeight * 0.5;

    for (int i = 0;  i < SEGMENTS; i++) {
        float t = float(i) / float(SEGMENTS - 1);
        float height = uHeight * (t);
        float width = WIDTH * (1 - t);

        float lightness = height / uHeight * 0.5 + 0.25;


        gl_Position = camMatrix * (
            bezier(p0, p1, p3, p4, t)) + vec4(width, 0.0, 0.0, 0.0);
        color = hslToRgb(uHue, 0.6, lightness) ;
        EmitVertex();

        gl_Position = camMatrix * (
            bezier(p0, p1, p3, p4, t)) + vec4(-width, 0.0, 0.0, 0.0);
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
