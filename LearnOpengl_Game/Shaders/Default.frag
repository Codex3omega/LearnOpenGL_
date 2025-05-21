#version 460 core
out vec4 FragColor;
in vec3 color;
in vec2 textCoords;

uniform sampler2D texture1;

void main()
{
    //FragColor = vec4(color, 1.0);
    FragColor = texture(texture1, textCoords);
}
