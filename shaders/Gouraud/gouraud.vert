#version 330 core

layout (location = 0) in vec3 vInPos;
layout (location = 1) in vec3 vInNorm;

out vec3 fInNorm;

// layout (binding = 0) 
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;


void main() {
    fInNorm = vInNorm;
    gl_Position = vec4(vInPos, 0.0) * modelMat * viewMat * projectionMat;
}