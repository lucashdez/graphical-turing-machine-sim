#version 450

layout(push_constant) uniform Push {
    mat4 u_mvp;
    vec4 u_color;
};

layout(location = 0) out vec4 out_color;

void main() {
    out_color = u_color;
}