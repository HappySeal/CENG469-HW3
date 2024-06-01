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

    vec3 result = (diffuse + specular);
    FragColor = vec4(result, 1.0);
}
//
//#version 330 core
//out vec4 FragColor;
//in vec3 currentPos;
//
//uniform samplerCube environmentMap;
//
//const float PI = 3.14159265359;
//
//void main()
//{
//    // The world vector acts as the normal of a tangent surface
//    // from the origin, aligned to WorldPos. Given this normal, calculate all
//    // incoming radiance of the environment. The result of this radiance
//    // is the radiance of light coming from -Normal direction, which is what
//    // we use in the PBR shader to sample irradiance.
//    vec3 N = normalize(currentPos);
//
//    vec3 irradiance = vec3(0.0);
//
//    // tangent space calculation from origin point
//    vec3 up    = vec3(0.0, 1.0, 0.0);
//    vec3 right = normalize(cross(up, N));
//    up         = normalize(cross(N, right));
//
//    float sampleDelta = 0.025;
//    float nrSamples = 0.0;
//    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
//    {
//        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
//        {
//            // spherical to cartesian (in tangent space)
//            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
//            // tangent space to world
//            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;
//
//            irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
//            nrSamples++;
//        }
//    }
//    irradiance = PI * irradiance * (1.0 / float(nrSamples));
//
//    FragColor = vec4(irradiance, 1.0);
//}