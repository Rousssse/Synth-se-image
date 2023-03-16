#version 300 es

precision mediump float;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

out vec4 fFragColor;

uniform sampler2D uEarthTexture;
uniform sampler2D uCloudTexture;

void main()
{
    vec4 textureE = texture(uEarthTexture, vTexCoords);
    vec4 textureC = texture(uCloudTexture, vTexCoords);
    vec4 resultColor = textureE + textureC;
    fFragColor = resultColor;
}


