#version 430

uniform mat4 transform;

in vec3 in_position;
in vec2 in_texCoord;
in vec3 in_normal;

out vec2 vert_texCoord;
out vec3 vert_normal;

void main()
{
	gl_Position = transform*vec4(in_position, 1.0);
	vert_texCoord = in_texCoord;
	vert_normal = (transform*vec4(in_normal, 0.0)).xyz;
}