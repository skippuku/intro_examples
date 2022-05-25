#version 330 core

layout(location = 0) in vec2 vert;

uniform vec2 screen_dimensions;

void
main() {
    vec2 scale = screen_dimensions * vec2(0.5, -0.5);
    gl_Position = vec4((vert / scale) + vec2(-1, 1), 0.0, 1.0);
}
