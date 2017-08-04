#version 430

uniform sampler2D diffuse;

uniform int is_selected;

in vec3 vert_normal;
in vec2 vert_col;

out vec4 out_col;

void main()
{
	if(is_selected == 2){
		out_col = vec4(mix(texture(diffuse, vert_col), vec4(1,1,0.2,0.5f), 0.6f));
	}
	else{ 
		if(is_selected == 1){
			out_col = vec4(mix(texture(diffuse, vert_col), vec4(0.2,0.6,0.7,0.5f), 0.6f));
		}
		else{
			out_col = texture(diffuse, vert_col);
		}
	}
}