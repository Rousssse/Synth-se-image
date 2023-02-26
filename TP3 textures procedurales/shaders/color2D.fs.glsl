#version 330 core

in vec3 vFragColor;
in vec2 vFragPosition;


out vec3 fFragColor;

void main() {
  float alpha = 1.5;
  float quentin = 25.0;
  float Dist = distance(vFragPosition,vec2(0,0));
  float a = alpha*exp(-quentin*(Dist*Dist));
  //fFragColor = a*vFragColor;
  vec2 P = vFragPosition;
  fFragColor = smoothstep(0.4, 0.5, max(abs(fract(8.0 * P.x - 0.5 * mod(floor(8.0 * P.y), 2.0)) - 0.5), abs(fract(8.0 * P.y) - 0.5)))*vFragColor;
  //length(fract(5.0 * P)) ( c'est bo)
  //length(abs(fract(5.0 * P) * 2.0 - 1.0)) (bof)
  //mod(floor(10.0 * P.x) + floor(10.0 * P.y), 2.0) (beurk)
  //smoothstep(0.3, 0.32, length(fract(5.0 * P) - 0.5)) (petits ronds choupi)
  //smoothstep(0.4, 0.5, max(abs(fract(8.0 * P.x - 0.5 * mod(floor(8.0 * P.y), 2.0)) - 0.5), abs(fract(8.0 * P.y) - 0.5))) (tema la pyramide)

};