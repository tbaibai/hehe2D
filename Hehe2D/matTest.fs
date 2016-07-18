precision mediump float;
uniform mat4 u_m;  
void main() 
{         
  mat4 m1 = mat4(
   	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0
  );
  
  mat4 m2 = mat4(
   	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 0.0
  );
  
  mat4 m = m1*m2;
  gl_FragColor = m * vec4(1.0, 1.0, 1.0, 0.0);
}      