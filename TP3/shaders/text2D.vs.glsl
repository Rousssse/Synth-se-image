#version 300 es

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec2 vFragTexture;
out vec2 vFragPosition;

uniform mat3 uModelMatrix;


void main() {
  //vFragColor = vec3(1.0,0.0,0.0);
  vec2 modif = (uModelMatrix*vec3(aVertexPosition,1)).xy;
  vFragPosition = aVertexPosition;
  vFragTexture = aVertexTexture;
  gl_Position = vec4(modif, 0, 1);

}

