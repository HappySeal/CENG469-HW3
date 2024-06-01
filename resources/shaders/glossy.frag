#version 330 core

struct Light {
    vec3 pos;
    vec3 color;
};

uniform Light lights[127]; // Array of lights
uniform int numLights; // Number of active lights
uniform vec3 cameraPos; // Camera position for specular calculation
uniform float exposure;
uniform mat4 skyboxMatrix;// Exposure value for tonemapping
uniform bool enableSpecular;
uniform samplerCube skybox;

in vec3 currentPos; // Position of the fragment
in vec3 Normal; // Normal at the fragment

out vec4 FragColor;


vec3 tonemap(vec3 hdrColor, float diffuser) {
    float lum = hdrColor.r * 0.2126 + hdrColor.g * 0.7152 + hdrColor.b * 0.0722;
    float lumScaled = lum * exposure;
    lumScaled /= diffuser;// exposure is a uniform variable
    float lumTM = lumScaled / (lumScaled + 1);
    vec3 tmColor = clamp((lumTM / lum) * hdrColor, vec3(0, 0, 0), vec3(1, 1, 1));
    float p = 1. / 2.2;
    return pow(tmColor, vec3(p, p, p));
}


void main() {
    //    vec3 kd = vec3(0.5, 0.5, 0.5); // Diffuse component set to medium gray
    //    vec3 ks = vec3(0.75, 0.75, 0.75); // Specular component set to lighter gray

    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    vec3 norm = normalize(Normal);

    vec3 I = normalize(currentPos - cameraPos);
    vec3 R = reflect(I, norm);

    vec3 transformedR = vec3(inverse(skyboxMatrix) * vec4(R, 0.0));

    vec3 envColor = texture(skybox, transformedR).rgb;



    for(int i = 0; i < numLights; i++) {
        // Diffuse shading
        vec3 lightDir = vec3(skyboxMatrix * vec4(normalize(lights[i].pos),1.0f));

        float diff = max(dot(Normal, lightDir), 0.0);
        diffuse += diff * tonemap(lights[i].color, 128000 * numLights);

        // Specular shading
        if(!enableSpecular) continue;

        vec3 viewDir = normalize(cameraPos - currentPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
        specular += spec * tonemap(lights[i].color, 128000.0f * numLights);
    }

    vec3 result = (diffuse + specular + envColor * 0.1);
    FragColor = vec4(result, 1.0);
}