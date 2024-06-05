#version 330 core

out vec4 FragColor;
in vec3 Normal;

void main() {
    // diffuse + ambient + specular
    vec3 color = vec3(60, 30, 0) / 255.0;

//     ambient
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * color;

//     diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(vec3(1.0,10,0));
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);

        vec3 result = (ambient + diffuse) * color;
        FragColor = vec4(result, 1.0);

}
