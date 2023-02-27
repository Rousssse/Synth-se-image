#version 300 es
precision mediump float;

in vec2 vFragTexture;
in vec2 vFragPosition;
in vec3 vFragColor;

out vec3 fFragColor;

uniform vec3 uColor;
uniform sampler2D uTexture;


void main() {

  fFragColor = texture(uTexture, vFragTexture).xyz;
};