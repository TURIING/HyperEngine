#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec2 vTexCoord;

layout(location = 0) out vec2 texCoord;

void main() {
    gl_Position = vec4(vPosition, 0.0, 1.0);
    texCoord = vTexCoord;
}