#version 430

//Texture
uniform sampler2D diffuse;

//Direction of green light if any 
uniform int active_light;	//-1 = none
							//0 = north
							//1 = east etc.
uniform int is_selected;

uniform vec4 light_diff;
uniform vec4 light_amb;
uniform vec4 light_spec;
uniform vec3 light_dir;
uniform float mat_shininess;

in vec4 vertPos;	//pos of current vertex
in vec3 cPos;		//pos of center of cell

in vec2 vert_texCoord;
in vec3 vert_normal;
in vec3 vert_EyePos;

out vec3 out_col;

vec3 northGreen(vec3 texCol, vec4 vertPos, vec3 cPos){
	
	vec3 trafficCol = texCol;

	//"North" part of cell, so mix with green
	if(vertPos.z > (cPos.z + 15) && vertPos.x < (cPos.x + 15) &&  vertPos.x > (cPos.x - 15)){
		trafficCol = mix(texCol, vec3(0,1,0),0.3);
	}
	//"East" part of cell so mix with red
	if(vertPos.x < (cPos.x - 15) && vertPos.z < (cPos.z + 15) &&  vertPos.z > (cPos.z - 15)){
		trafficCol = mix(texCol, vec3(1,0,0),0.3);
	}
	//"South" part of cell so mix with red
	if(vertPos.z < (cPos.z - 15) && vertPos.x < (cPos.x + 15) &&  vertPos.x > (cPos.x - 15)){
		trafficCol = mix(texCol, vec3(1,0,0),0.3);
	}
	//"West" part of cell so mix with red
	if(vertPos.x > (cPos.x + 15) && vertPos.z < (cPos.z + 15) &&  vertPos.z > (cPos.z - 15)){
		trafficCol = mix(texCol, vec3(1,0,0),0.3);
	}


	return trafficCol;
}

vec3 eastGreen(vec3 texCol, vec4 vertPos, vec3 cPos){
	
	vec3 trafficCol = texCol;

	//"North" part of cell, so mix with red
	if(vertPos.z > (cPos.z + 15) && vertPos.x < (cPos.x + 15) &&  vertPos.x > (cPos.x - 15)){
		trafficCol = mix(texCol, vec3(1,0,0),0.3);
	}
	//"East" part of cell so mix with green
	if(vertPos.x < (cPos.x - 15) && vertPos.z < (cPos.z + 15) &&  vertPos.z > (cPos.z - 15)){
		trafficCol = mix(texCol, vec3(0,1,0),0.3);
	}
	//"South" part of cell so mix with red
	if(vertPos.z < (cPos.z - 15) && vertPos.x < (cPos.x + 15) &&  vertPos.x > (cPos.x - 15)){
		trafficCol = mix(texCol, vec3(1,0,0),0.3);
	}
	//"West" part of cell so mix with red
	if(vertPos.x > (cPos.x + 15) && vertPos.z < (cPos.z + 15) &&  vertPos.z > (cPos.z - 15)){
		trafficCol = mix(texCol, vec3(1,0,0),0.3);
	}


	return trafficCol;
}

vec3 southGreen(vec3 texCol, vec4 vertPos, vec3 cPos){
	
	vec3 trafficCol = texCol;

	//"North" part of cell, so mix with red
	if(vertPos.z > (cPos.z + 15) && vertPos.x < (cPos.x + 15) &&  vertPos.x > (cPos.x - 15)){
		trafficCol = mix(texCol, vec3(1,0,0),0.3);
	}
	//"East" part of cell so mix with red
	if(vertPos.x < (cPos.x - 15) && vertPos.z < (cPos.z + 15) &&  vertPos.z > (cPos.z - 15)){
		trafficCol = mix(texCol, vec3(1,0,0),0.3);
	}
	//"South" part of cell so mix with green
	if(vertPos.z < (cPos.z - 15) && vertPos.x < (cPos.x + 15) &&  vertPos.x > (cPos.x - 15)){
		trafficCol = mix(texCol, vec3(0,1,0),0.3);
	}
	//"West" part of cell so mix with red
	if(vertPos.x > (cPos.x + 15) && vertPos.z < (cPos.z + 15) &&  vertPos.z > (cPos.z - 15)){
		trafficCol = mix(texCol, vec3(1,0,0),0.3);
	}


	return trafficCol;
}

vec3 westGreen(vec3 texCol, vec4 vertPos, vec3 cPos){
	
	vec3 trafficCol = texCol;

	//"North" part of cell, so mix with red
	if(vertPos.z > (cPos.z + 15) && vertPos.x < (cPos.x + 15) &&  vertPos.x > (cPos.x - 15)){
		trafficCol = mix(texCol, vec3(1,0,0),0.3);
	}
	//"East" part of cell so mix with red
	if(vertPos.x < (cPos.x - 15) && vertPos.z < (cPos.z + 15) &&  vertPos.z > (cPos.z - 15)){
		trafficCol = mix(texCol, vec3(1,0,0),0.3);
	}
	//"South" part of cell so mix with red
	if(vertPos.z < (cPos.z - 15) && vertPos.x < (cPos.x + 15) &&  vertPos.x > (cPos.x - 15)){
		trafficCol = mix(texCol, vec3(1,0,0),0.3);
	}
	//"West" part of cell so mix with green
	if(vertPos.x > (cPos.x + 15) && vertPos.z < (cPos.z + 15) &&  vertPos.z > (cPos.z - 15)){
		trafficCol = mix(texCol, vec3(0,1,0),0.3);
	}

	return trafficCol;
}

void main()
{
	vec4 colour = vec4(texture(diffuse, vert_texCoord).rgb, 1.0);
	//vec4 colour = vec4(1.0, 0.0, 0.0, 1.0);

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

	vec3 texCol = vec3(colour + Iamb + Idiff + Ispec);

	vec3 juncCol;

	//Check which direction is green if any
	switch(active_light){
		case 0:	//North is green
			juncCol = northGreen(texCol, vertPos, cPos);
			break;
		case 1:	//East is green
			juncCol = eastGreen(texCol, vertPos, cPos);
			break;
		case 2:	//South is green
			juncCol = southGreen(texCol, vertPos, cPos);
			break;
		case 3:	//West is green
			juncCol = westGreen(texCol, vertPos, cPos);
			break;
		default:	//Not a junction
			juncCol = texCol;//no change in color
			break;
			
	}

	//Check to see if cell is highlighted or selected
	if(is_selected == 1){
		out_col = mix(juncCol, vec3(1,1,0.2), 0.8f);
	}else{
		if(is_selected == 2){
			out_col = mix(juncCol, vec3(1,0.55,0), 0.4f);
		}else{
			if(is_selected == 3){
				out_col = mix(juncCol, vec3(1,0,0), 0.6f);
			}else{
				out_col = juncCol;
			}
		}
	}

	//out_col = juncCol;
}