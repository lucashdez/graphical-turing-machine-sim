#version 450

layout(push_constant) uniform Push {
    mat4 u_mvp; // model * view * projection
    vec4 u_color;
};

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 a_uv;

layout(location = 0) out vec2 v_uv;


vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

void main() {
    gl_Position = u_mvp * vec4(position, 0.0, 1.0);
    v_uv = a_uv;
}