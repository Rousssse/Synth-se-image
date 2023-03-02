#version 300 es

precision mediump float;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

out vec4 fFragColor;

uniform sampler2D texture3D;

void main()
{
    fFragColor = texture(texture3D, vTexCoords);
}


