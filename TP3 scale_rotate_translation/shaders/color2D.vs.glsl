#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragColor;

mat3 translate(float tx ,float ty){
return mat3(vec3(1,0,0),vec3(0,1,0),vec3(tx,ty,1));
};
mat3 scale(float sx, float sy){
return mat3(vec3(sx,0,0),vec3(0,sy,0),vec3(0,0,1));
};
mat3 rotate(float a){
  return mat3(vec3(cos(a),sin(a),0),vec3(-1*sin(a),cos(a),0), vec3(0,0,1));
};


void main() {
  vFragColor = aVertexColor;
  
  gl_Position = vec4((translate(0.5,0)* rotate(45) * scale(0.5,0.5)  * vec3(aVertexPosition,1)).xy, 0, 1);
  

  // pour scale 3* pour translation + vec2(x,y) pour grossir ou reduire * vec2(x,y)
  //avec matrice
  // gl_Position = vec4((translate(0.5,0.5) * vec3(aVertexPosition,1)).xy, 0, 1);
  //scale 
  //gl_Position = vec4((scale(2,0.5) * vec3(aVertexPosition,1)).xy, 0, 1);
};