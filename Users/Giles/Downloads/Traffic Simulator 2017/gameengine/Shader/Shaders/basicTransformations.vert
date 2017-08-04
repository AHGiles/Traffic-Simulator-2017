#version 330

uniform mat4 transform;

in  vec3 in_position;	// Position coming in
in  vec3 in_col;		// colour coming in

out vec3 vert_col;		// colour leaving the vertex, this will be sent to the fragment shader

void main(void)
{
	gl_Position = transform * vec4(in_position, 1.0);
	
	vert_col = in_col;
}