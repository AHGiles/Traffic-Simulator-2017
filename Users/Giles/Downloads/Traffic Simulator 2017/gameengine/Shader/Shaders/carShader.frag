#version 430

uniform sampler2D diffuse;

uniform int is_selected;

in vec3 vert_normal;
in vec2 vert_texCoord;

out vec3 out_col;

void main()
{
	if(is_selected == 1){
		out_col = mix(texture(diffuse, vert_texCoord).rgb, vec3(1,1,0.2), 0.8f);
	}else{
		if(is_selected == 2){
			out_col = mix(texture(diffuse, vert_texCoord).rgb, vec3(0,1,0), 0.4f);
		}else{
			out_col = texture(diffuse, vert_texCoord).rgb ;
		}
	}

}