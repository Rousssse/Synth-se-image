#version 300 es

precision mediump float;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

out vec4 fFragColor;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;

void main()
{
    vec4 textureE = texture(uTexture0, vTexCoords);
    vec4 textureC = texture(uTexture1, vTexCoords);
    vec4 resultColor = textureE + textureC;
    fFragColor = resultColor;
}


