#version 330
layout (location = 0) in vec4 vInPos;
uniform mat4 model;


void main() {
    gl_Position = vec4(vInPos.x, vInPos.y, vInPos.z, 1.0) * model;
}