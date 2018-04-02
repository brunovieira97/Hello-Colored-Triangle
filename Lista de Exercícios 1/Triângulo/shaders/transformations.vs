#version 410
in vec2 position;
in vec4 color;
out vec4 color0;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    color0 = color;
}