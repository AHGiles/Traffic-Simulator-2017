#pragma once

#include "../../lib/Assimp/include/assimp/Importer.hpp" // C++ importer interface
#include "../../lib/Assimp/include/assimp/scene.h"		// Output data structure
#include "../../lib/Assimp/include/assimp/postprocess.h"// Post processing fla
#include <vector>

#include "../../Shader/Shader.h"

#include "VertexBuffer.h"
#include "AssimpTexture.h"

  

class AssimpModel
{
public:
	bool LoadModelFromFile(char* sFilePath);
	//bool LoadModelFromFileBumpMap(char* sFilePath, char* sColorMap, char* sNormalMap);

	static void FinalizeVBO();
	static void BindModelsVAO();

	void RenderModel();
	//void RenderModelBumpMap(Shader* spProgram);

	AssimpModel();
	bool bLoaded;
private:
	
	static VertexBuffer vboModelData;
	static /*UINT*/ GLuint uiVAO;
	static vector<AssimpTexture> tTextures;

	std::vector<int> iMeshStartIndices;
	std::vector<int> iMeshSizes;
	std::vector<int> iMaterialIndices;
	int iNumMaterials;
};
