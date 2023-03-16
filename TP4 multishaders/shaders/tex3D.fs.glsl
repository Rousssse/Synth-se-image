#version 300 es

precision mediump float;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

out vec4 fFragColor;

uniform sampler2D uTexture;

void main()
{
    fFragColor = texture(uTexture, vTexCoords);
}


