#version 330
layout (location = 0) in vec4 vInPos;
uniform mat4 model;
uniform mat4 projection;

void main() {
    gl_Position = vec4(vInPos.xyz, 1.0) * model * projection;
    //gl_Position = model * vec4(vInPos.x, vInPos.y, vInPos.z, 1.0);
}