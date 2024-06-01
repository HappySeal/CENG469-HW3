#version 330 core


uniform mat4 skyboxMatrix;
uniform samplerCube skybox;
uniform vec3 cameraPos;
uniform float exposure;


in vec3 currentPos; // Position of the fragment
in vec3 Normal; // Normal at the fragment

out vec4 FragColor;


vec3 tonemap(vec3 hdrColor) {
    float lum = hdrColor.r * 0.2126 + hdrColor.g * 0.7152 + hdrColor.b * 0.0722;
    float lumScaled = lum * exposure; // exposure is a uniform variable
    float lumTM = lumScaled / (lumScaled + 1);
    vec3 tmColor = clamp((lumTM / lum) * hdrColor, vec3(0, 0, 0), vec3(1, 1, 1));
    float p = 1. / 2.2;
    return pow(tmColor, vec3(p, p, p));
}

// glass shader
void main() {
    float eta = 1.0f / 1.52f;
    vec3 d = normalize(currentPos - cameraPos);
    vec3 n = normalize(Normal);

    float cosTheta = dot(-d, n);
    float cosPhI = 1 - eta * eta * (1 - cosTheta * cosTheta);
    float cosPhT = 0;
    if(cosPhI < 0.0) {
        cosPhT = 0;
    }else{
        cosPhT = sqrt(cosPhI);
    }

    vec3 w = (d + n * cosTheta)*eta - n * cosPhT;
    vec3 refracted = vec3(inverse(skyboxMatrix) * vec4(normalize(w), 0.0));

    FragColor = vec4(tonemap(texture(skybox, refracted).rgb).rgb, 1.0);
}