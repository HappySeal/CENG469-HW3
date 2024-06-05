#version 330 core
#define SEGMENTS 24
#define PI 3.14159265359

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

const float WIDTH = -0.05;
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

mat4 rotateX(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, c, -s, 0.0,
        0.0, s, c, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 rotateY(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat4(
        c, 0.0, s, 0.0,
        0.0, 1.0, 0.0, 0.0,
        -s, 0.0, c, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}
mat4 rotateZ(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat4(
        c, -s, 0.0, 0.0,
        s, c, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 rotateAxisAngle(vec3 axis, float angle) {
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(
        oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0,
        oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0,
        oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
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
        (gs_in[index].world_pos.x + uTime *  uWindSpeed *  uWindSpeed) / WINDMAP_SIZE,
        (gs_in[index].world_pos.z + uTime *  uWindSpeed *  uWindSpeed) / WINDMAP_SIZE
    );

    vec3 wind = texture(uWindMapX1, uv).xyz;
    vec4 windDirection = vec4(wind.x, 0.0, wind.z , 0.0f) * uWindSpeed;
    vec4 windOrtho = vec4(-wind.z, 0.0, wind.x, 0.0f);

    vec4 p0 = gs_in[index].world_pos;
    float rotaion = length(wind) * uHeight * uWindSpeed;

    vec4 p4= gs_in[index].world_pos + rotateAxisAngle(windDirection.xyz,  radians(30.0 * rotaion)) * (vec4(0,uHeight,0,1));
    vec4 p1 = gs_in[index].world_pos + rotateAxisAngle(windDirection.xyz,  radians(15.0 * rotaion)) * (vec4(0,uHeight * 0.3,0,1));
    vec4 p3 = gs_in[index].world_pos + rotateAxisAngle(windDirection.xyz, radians(20.0 * rotaion)) * (vec4(0,uHeight * 0.6,0,1));

    for (int i = 0;  i < SEGMENTS; i++) {
        float t = float(i) / float(SEGMENTS - 1);
        float height = uHeight * (t);
        float width = WIDTH * sqrt(1 - t);

        float lightness = height / uHeight * 0.5 + 0.25;


        gl_Position = camMatrix * (
            vec4(bezier(p0, p1, p3, p4, t).xyz,1.0) + rotateZ(radians(180.0 * sin(gs_in[index].world_pos.x + gs_in[index].world_pos.y))) * vec4(width, 0.0, 0.0, 0.0));
        color = hslToRgb(uHue, 0.6, lightness) ;
        EmitVertex();

        gl_Position = camMatrix * (
            vec4(bezier(p0, p1, p3, p4, t).xyz,1.0) + rotateZ(radians(180.0 * sin(gs_in[index].world_pos.x+ gs_in[index].world_pos.y))) * vec4(-width, 0.0, 0.0, 0.0));
        color = hslToRgb(uHue, 0.6, lightness) ;
        EmitVertex();
    }
    EndPrimitive();

    //Middle
}

void GenerateMiddle(int index){

    vec2 uv = vec2(
        ((gs_in[index + 1].world_pos.x + gs_in[index + 2].world_pos.x)*0.5 + uTime *  uWindSpeed) / WINDMAP_SIZE,
        ((gs_in[index + 1].world_pos.z + gs_in[index + 2].world_pos.z)*0.5 + uTime *  uWindSpeed) / WINDMAP_SIZE
    );

    vec3 wind = texture(uWindMapX1, uv).xyz;
    vec4 windDirection = vec4(wind.x, 0.0, wind.z , 0.0f) * uWindSpeed;
    vec4 windOrtho = vec4(-wind.z, 0.0, wind.x, 0.0f);

    vec4 p0 = (gs_in[index + 1].world_pos + gs_in[index + 2].world_pos) * 0.5;
    float rotaion = length(wind) * uHeight * uWindSpeed;

    vec4 p4= p0 + rotateAxisAngle(windDirection.xyz,  radians(30.0 * rotaion)) * (vec4(0,uHeight,0,1));
    vec4 p1 = p0 + rotateAxisAngle(windDirection.xyz,  radians(15.0 * rotaion)) * (vec4(0,uHeight * 0.3,0,1));
    vec4 p3 = p0 + rotateAxisAngle(windDirection.xyz, radians(20.0 * rotaion)) * (vec4(0,uHeight * 0.6,0,1));

    for (int i = 0;  i < SEGMENTS; i++) {
        float t = float(i) / float(SEGMENTS - 1);
        float height = uHeight * (t);
        float width = WIDTH * sqrt(1 - t);

        float lightness = height / uHeight * 0.5 + 0.25;


        gl_Position = camMatrix * (
        vec4(bezier(p0, p1, p3, p4, t).xyz,1.0) + rotateZ(sin(p0.x + p0.y)) * vec4(width, 0.0, 0.0, 0.0));
        color = hslToRgb(uHue, 0.6, lightness) ;
        EmitVertex();

        gl_Position = camMatrix * (
        vec4(bezier(p0, p1, p3, p4, t).xyz,1.0) + rotateZ(sin(p0.x + p0.y)) * vec4(-width, 0.0, 0.0, 0.0));
        color = hslToRgb(uHue, 0.6, lightness) ;
        EmitVertex();
    }
    EndPrimitive();
}

void main()
{
    GenerateLine(0);// first vertex normal
    GenerateMiddle(0);
}
