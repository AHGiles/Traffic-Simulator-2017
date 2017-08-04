#version 330

in  vec3 vert_col;		//colour arriving from the vertex


out vec4 out_col;		//colour for the pixel

void main(void)
{
	out_col = vec4(vert_col,1.0);
}