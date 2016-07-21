attribute vec4 a_position;
uniform mat4 u_m;  
void main()               
{
   gl_Position = u_m*a_position;
}