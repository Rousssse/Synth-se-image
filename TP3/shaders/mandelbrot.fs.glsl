#version 300 es
precision mediump float;

in vec3 vFragColor;
in vec2 vFragPosition;

out vec3 fFragColor;

vec2 complexSqr(vec2 z){
   return vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y);
}


void main() {

  vec2 z = vec2(vFragPosition.x,vFragPosition.y);
  int Nmax = 200;


  for(int i=0; i<Nmax ; i++){
    z = complexSqr(z) +  z;

    if(length(z)> 2.0){
     //fFragColor = vec3(float(i)/float(Nmax));
     fFragColor = vec3(1.0,1.0,1.0);
      break;
    }
    else {
      fFragColor = vec3(0.0,0.0,0.0);
    }
  
  }
};