#version 450

uniform mat4 transform;
uniform mat3 normal;

in vec3 in_position;
in vec2 in_texCoord;
in vec3 in_normal;

out vec2 vert_texCoord;
out vec3 vert_normal;
out vec3 vert_EyePos;

void main(void)
{
	gl_Position = transform * vec4(in_position, 1.0);

	vert_normal = normalize(normal * in_normal);
	vert_EyePos = vec3((transform * vec4(in_position, 1.0) ) );

	vert_texCoord = in_texCoord;
}