#version 330
layout (location = 0) in vec4 vInPos;
uniform mat4 model;
uniform mat4 projection;

//Mv order

void main() {
    gl_Position = projection * model * vec4(vInPos.xyz, 1.0);
    //gl_Position = model * vec4(vInPos.x, vInPos.y, vInPos.z, 1.0);
}