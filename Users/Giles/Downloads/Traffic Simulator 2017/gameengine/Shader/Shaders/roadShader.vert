#version 430

uniform mat4 transform;
uniform mat4 model;
uniform mat3 normal;

in vec3 in_position;
in vec2 in_texCoord;
in vec3 in_normal;


out vec4 vertPos;
out vec3 cPos;
out vec2 vert_texCoord;
out vec3 vert_normal;
out vec3 vert_EyePos;

void main()
{
	//Set Pos
	gl_Position = transform*vec4(in_position, 1.0);

	//Grab exact pos for traffic calcs
	vertPos =  model*vec4(in_position, 1.0);
	cPos = vec3(model[3][0],model[3][1],model[3][2]);

	//Grab shiz for lighting and texturing
	vert_texCoord = in_texCoord;
	vert_normal = normalize(normal * in_normal);
	vert_EyePos = vec3((transform * vec4(in_position, 1.0) ) );
}