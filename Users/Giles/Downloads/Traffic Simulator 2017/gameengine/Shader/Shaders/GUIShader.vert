#version 430

in vec3 in_position;
in vec2 in_col;
in vec3 in_normal;

out vec2 vert_col;
out vec3 vert_normal;

void main()
{
	gl_Position = vec4(in_position, 1.0);
	vert_col = in_col;
	vert_normal = in_normal;
}