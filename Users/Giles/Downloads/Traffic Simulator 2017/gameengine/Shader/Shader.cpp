#include "Shader.h"
#include <fstream>
#include <iostream>


//Lighting Constants
GLfloat Light_Ambient_And_Diffuse[4] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat Light_Specular[4] = { 1.0,1.0,1.0,1.0 };
GLfloat Light_Dir[] = { 0.0f, -0.8f, 0.5f };

Shader::Shader(const string& fileName)
{
	main_program = glCreateProgram(); // Create new program
	main_shaders[0] = CreateShader(LoadShader(fileName + ".vert"), GL_VERTEX_SHADER); // Create a new vertex shader
	main_shaders[1] = CreateShader(LoadShader(fileName + ".frag"), GL_FRAGMENT_SHADER); // Create a new fragment shader

	for (int i = 0;i < NUM_SHADERS;i++)
		glAttachShader(main_program, main_shaders[i]); // Attach shaders to our program

	glBindAttribLocation(main_program, 0, "in_position"); // Bind the vertex shader position attribute
	glBindAttribLocation(main_program, 1, "in_texCoord"); // Bind the shader texture position attribute
	glBindAttribLocation(main_program, 2, "in_normal"); // Bind the shader normal position attribute

	bool failed = false;

	glLinkProgram(main_program); // Link the program
	failed = CheckShaderError(main_program, GL_LINK_STATUS, true, "Error: Shader failed to link with program."); // Check for shader link errors

	glValidateProgram(main_program); // Validate the program
	failed = CheckShaderError(main_program, GL_VALIDATE_STATUS, true, "Error: Validation of program failed."); // Check for shader validation errors

	if (!failed)
		cout << "Shader: "<< fileName << " Loaded Succesfully" << endl;

	main_uniforms[TRANSFORM_U] = glGetUniformLocation(main_program, "transform"); // Figure out what we should apply our uniform transform to (transform that is applied to all vertices)

	main_uniforms[MODEL] = glGetUniformLocation(main_program, "model");

	main_uniforms[NORMAL_U] = glGetUniformLocation(main_program, "normal");

	main_uniforms[DIFFUSE_MAP] = glGetUniformLocation(main_program, "diffuse_map");

	main_uniforms[LIGHT_DIFFUSE] = glGetUniformLocation(main_program, "light_diff");

	main_uniforms[LIGHT_AMB] = glGetUniformLocation(main_program, "light_amb");

	main_uniforms[LIGHT_SPEC] = glGetUniformLocation(main_program, "light_spec");

	main_uniforms[LIGHT_DIR] = glGetUniformLocation(main_program, "light_dir"); 

	main_uniforms[MAT_SHIN] = glGetUniformLocation(main_program, "mat_shininess");

	main_uniforms[SELECTED] = glGetUniformLocation(main_program, "is_selected");

	main_uniforms[ACTIVE_LIGHT] = glGetUniformLocation(main_program, "active_light");

	main_uniforms[HEATMAP_STAT] = glGetUniformLocation(main_program, "heatmap_stat");

	openGLErrorCheck(__FILE__, __LINE__);
}

Shader::~Shader()
{
	for (int i = 0;i < NUM_SHADERS;i++) // Unbind and delete shaders
	{
		glDetachShader(main_program, main_shaders[i]);
		glDeleteShader(main_shaders[i]);
	}
	glDeleteProgram(main_program); // Remove program
}

void Shader::Bind()
{
	glUseProgram(main_program); // Bind the shader to the program
	openGLErrorCheck(__FILE__, __LINE__);
}

void Shader::Update(Transform transform, Camera camera)
{
	glm::mat4 modelViewMatrix = camera.getProjectionMatrix() * camera.getView() * transform.getModel();
	glUniformMatrix4fv(main_uniforms[TRANSFORM_U], 1, GL_FALSE, &modelViewMatrix[0][0]);

	glm::mat3 normalMatix = transform.getNormal();
	glUniformMatrix4fv(main_uniforms[NORMAL_U], 1, GL_FALSE, &normalMatix[0][0]);

	openGLErrorCheck(__FILE__, __LINE__);
}

void Shader::UpdateRoad(Transform transform, Camera camera, int isSelected, int activeLight, GLfloat mat_shininess)
{
	glm::mat4 modelMatrix = /*camera.getView() */ transform.getModel();
	glUniformMatrix4fv(main_uniforms[MODEL], 1, GL_FALSE, &modelMatrix[0][0]);

	glm::mat4 projModelView = camera.getProjectionMatrix() * camera.getView() * modelMatrix;
	glUniformMatrix4fv(main_uniforms[TRANSFORM_U], 1, GL_FALSE, &projModelView[0][0]);

	glm::mat3 normalMatix = transform.getNormal();
	glUniformMatrix4fv(main_uniforms[NORMAL_U], 1, GL_FALSE, &normalMatix[0][0]);

	glUniform1i(main_uniforms[SELECTED], isSelected);

	glUniform1i(main_uniforms[ACTIVE_LIGHT], activeLight);

	glUniformMatrix4fv(main_uniforms[LIGHT_DIFFUSE], 1, GL_FALSE, Light_Ambient_And_Diffuse);
	glUniformMatrix4fv(main_uniforms[LIGHT_AMB], 1, GL_FALSE, Light_Ambient_And_Diffuse);
	glUniformMatrix4fv(main_uniforms[LIGHT_SPEC], 1, GL_FALSE, Light_Specular);
	glUniformMatrix4fv(main_uniforms[LIGHT_DIR], 1, GL_FALSE, Light_Dir);
	glUniform1f(main_uniforms[MAT_SHIN], mat_shininess);

	//openGLErrorCheck(__FILE__, __LINE__);
}

void Shader::UpdateRoadHeatMap(Transform transform, Camera camera, float heatMapStat){

	glm::mat4 modelMatrix = /*camera.getView() */ transform.getModel();
	glUniformMatrix4fv(main_uniforms[MODEL], 1, GL_FALSE, &modelMatrix[0][0]);

	glm::mat4 projModelView = camera.getProjectionMatrix() * camera.getView() * modelMatrix;
	glUniformMatrix4fv(main_uniforms[TRANSFORM_U], 1, GL_FALSE, &projModelView[0][0]);

	glm::mat3 normalMatix = transform.getNormal();
	glUniformMatrix4fv(main_uniforms[NORMAL_U], 1, GL_FALSE, &normalMatix[0][0]);


	glUniform1f(main_uniforms[HEATMAP_STAT], heatMapStat);

	openGLErrorCheck(__FILE__, __LINE__);

}

void Shader::UpdateCubeMap(Camera camera){

	glm::mat4 projView = camera.getProjectionMatrix() * camera.getView();
	glUniformMatrix4fv(main_uniforms[TRANSFORM_U], 1, GL_FALSE, &projView[0][0]);

	openGLErrorCheck(__FILE__, __LINE__);
}


void Shader::UpdateGUI(int isSelected)
{

	glUniform1i(main_uniforms[SELECTED], isSelected);
	openGLErrorCheck(__FILE__, __LINE__);
}


GLuint Shader::CreateShader(const string& input, GLenum type)
{
	GLuint shader = glCreateShader(type);
	openGLErrorCheck(__FILE__, __LINE__);

	if (shader == 0)
		cerr << "Error: Failed to create shader." << endl;

	const GLchar* shaderStringList[1];
	GLint shaderStringListLength[1];

	shaderStringList[0] = input.c_str();
	shaderStringListLength[0] = input.length();

	glShaderSource(shader, 1, shaderStringList, shaderStringListLength);
	glCompileShader(shader); // Build shader

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader failed to compile."); // Check for shader compilation errors

	return shader;
}

string Shader::LoadShader(const string& fileName)
{
	ifstream file;
	file.open((fileName).c_str());

	string output, line;
	
	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
		cerr << "Unable to load shader: " << fileName << endl;
	return output;
}

bool Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		cerr << errorMessage << ": '" << error << "'" << endl;

		return true;

	}
	return false;
}





