#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in uint inColor;

layout(location = 0) out vec4 fragColor;

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0);
    vec4 color;
    color.a = (float((inColor       ) & 0xFFu))/255.0;
    color.b = (float((inColor >> 8u ) & 0xFFu))/255.0;
    color.g = (float((inColor >> 16u) & 0xFFu))/255.0;
    color.r = (float((inColor >> 24u) & 0xFFu))/255.0;
    fragColor = vec4(color.xyz, 1.0);
}
