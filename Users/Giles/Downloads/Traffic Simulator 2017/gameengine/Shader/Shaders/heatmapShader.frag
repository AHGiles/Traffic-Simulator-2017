#version 430

//Texture
uniform sampler2D diffuse;

uniform float heatmap_stat;

in vec3 vert_normal;
in vec2 vert_texCoord;

out vec3 out_col;

vec3 heatmapMixing(vec3 texCol){

	vec3 heatmapCol = texCol;

	if(heatmap_stat >= 0.5){
		heatmapCol = mix(heatmapCol, vec3(1,0,0), 0.5);
	}
	else{			//if not > 0.5
		if(heatmap_stat >= 0.2){
			heatmapCol = mix(heatmapCol, vec3(1,0.65,0), 0.5);
		}
		else{		//if not > 0.5 or 0.2
			if(heatmap_stat >= 0.05){
				heatmapCol = mix(heatmapCol, vec3(1,1,0.2), 0.5);
			}
			else{	//default
				heatmapCol = mix(heatmapCol, vec3(0,0.8,0), 0.5);
			}
		}
	}
	return heatmapCol;
}

vec3 heatmapMixing2(vec3 texCol){

	vec3 heatmapCol = texCol;
	vec3 mixCol;
	if(heatmap_stat > 0.6){
		float mixVal = (heatmap_stat - 0.6)*2.5;
		mixCol = mix(vec3(1,0.6,0),vec3(1,0,0), mixVal);
	}else{
		if(heatmap_stat > 0.3){
			float mixVal = (heatmap_stat - 0.3)*(10/3);
			mixCol = mix(vec3(1,1,0),vec3(1,0.6,0), mixVal);
		}
		else{
			float mixVal = heatmap_stat*(10/3);
			mixCol = mix(vec3(0,1,0),vec3(1,1,0), mixVal);
		}
	}
	//mixCol = mix(vec3(0,1,0),vec3(1,0,0), heatmap_stat);
	heatmapCol = mix(heatmapCol, mixCol, 0.5);
	
	return heatmapCol;
}

void main()
{
	vec3 texCol = texture(diffuse, vert_texCoord).rgb;

	if(heatmap_stat == -1){
		out_col = texCol;
	}else{
		out_col = heatmapMixing2(texCol);
	}
	
}