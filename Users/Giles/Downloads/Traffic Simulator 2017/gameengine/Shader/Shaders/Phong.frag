#version 450

uniform vec4 light_diff;
uniform vec4 light_amb;
uniform vec4 light_spec;
uniform vec3 light_dir;
uniform float mat_shininess;

uniform sampler2D diffuse;

in vec2 vert_texCoord;
in vec3 vert_normal;
in vec3 vert_EyePos;

out vec4 ex_Color;

void main(void)
{
	//vec4 colour = vec4(ex_Color,1.0);

	vec4 colour = vec4(texture(diffuse, vert_texCoord).rgb, 1.0);

	vec3 L = normalize(light_dir);
	vec3 E = normalize(-vert_EyePos);
	vec3 R = normalize(-reflect(L,vert_normal));

	//ambient term
	vec4 Iamb = light_amb;

	//diffuse term
	vec4 Idiff = light_diff * max(dot(vert_normal,L),0.0);
	Idiff = clamp(Idiff, 0.0, 1.0);

	//specular term
	vec4 Ispec = light_spec * pow( max( dot(R,E) ,0.0) ,0.3 * mat_shininess);
	Ispec = clamp(Ispec, 0.0, 1.0);

	ex_Color = colour + Iamb + Idiff + Ispec;
}