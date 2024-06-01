#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 currentPos;

uniform vec4 color;
uniform vec3 cameraPos;

uniform mat4 skyboxMatrix;
uniform samplerCube skybox;

uniform float exposure;

vec3 tonemap(vec3 hdrColor) {
   float lum = hdrColor.r * 0.2126 + hdrColor.g * 0.7152 + hdrColor.b * 0.0722;
   float lumScaled = lum * exposure; // exposure is a uniform variable
   float lumTM = lumScaled / (lumScaled + 1);
   vec3 tmColor = clamp((lumTM / lum) * hdrColor, vec3(0, 0, 0), vec3(1, 1, 1));
   float p = 1. / 2.2;
   return pow(tmColor, vec3(p, p, p));
}


void main(){

   vec3 mnormal = normalize(Normal);

   vec3 I = normalize(currentPos - cameraPos);
   vec3 R = reflect(I, mnormal);

   vec3 transformedR = vec3(inverse(skyboxMatrix) * vec4(R, 0.0));

   vec3 envColor = texture(skybox, transformedR).rgb;

   FragColor = vec4(tonemap(envColor), 1.0);



}