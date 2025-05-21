#version 460 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec2 tex;

out vec3 color;
out vec2 texCoords;

uniform vec3 offset;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    color = in_color;
    texCoords = tex;

}