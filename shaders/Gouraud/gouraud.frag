#version 330 core

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};
struct PointLight {
    vec3 location;
    vec3 color;
    float intensity;
    float radius;
};

layout (location=0) out vec4 outColor;

in vec3 fInNorm;

uniform DirectionalLight direcLights[10];
uniform PointLight pntLights[50];
uniform int numDirecLights;
uniform int numPntLights;

void main() {
    outColor = vec4(0.0, 0.0, 0.0, 0.0);
    int actNumDirecLights = (numDirecLights>10) ? 10 : numDirecLights;
    for(int i = 0; i<actNumDirecLights; i++) {
        float lightDirec = dot(fInNorm, direcLights[i].direction);
        outColor += (dot(fInNorm, direcLights[i].direction)*direcLights[i].intensity)*vec4(direcLights[i].color, 0.0);
    }
    outColor = normalize(outColor);
}