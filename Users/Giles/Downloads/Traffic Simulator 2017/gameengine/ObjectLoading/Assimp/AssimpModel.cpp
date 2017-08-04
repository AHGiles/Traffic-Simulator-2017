#include "AssimpModel.h"

VertexBuffer AssimpModel::vboModelData;
GLuint AssimpModel::uiVAO;
vector<AssimpTexture> AssimpModel::tTextures;

/*-----------------------------------------------

Name:	GetDirectoryPath

Params:	sFilePath - guess ^^

Result: Returns directory name only from filepath.

/*---------------------------------------------*/
string GetDirectoryPath(string sFilePath)
{
	// Get directory path
	string sDirectory = "";
	//RFOR(i, ESZ(sFilePath) - 1)if (sFilePath[i] == '\\' || sFilePath[i] == '/')
	for(int i = sFilePath.size() -1; i >= 0; i--){
		if (sFilePath[i] == '\\' || sFilePath[i] == '/') {
			sDirectory = sFilePath.substr(0, i + 1);
			break;
		}
	}
	return sDirectory;
}

AssimpModel::AssimpModel()
{
	bLoaded = false;
}

/*-----------------------------------------------

Name:	LoadModelFromFile

Params:	sFilePath - guess ^^

Result: Loads model using Assimp library.

/*---------------------------------------------*/
bool AssimpModel::LoadModelFromFile(char* sFilePath)
{
	if (vboModelData.GetBufferID() == 0){
		vboModelData.CreateVBO();
		tTextures.reserve(50);
	}
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(sFilePath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene)
	{
		cerr << "Error Loading Model: " << sFilePath << endl;
		return false;
	}

	const int iVertexTotalSize = sizeof(aiVector3D) * 2 + sizeof(aiVector2D);

	int iTotalVertices = 0;

	//FOR(i, scene->mNumMeshes)
	for (int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		int iMeshFaces = mesh->mNumFaces;
		iMaterialIndices.push_back(mesh->mMaterialIndex);
		int iSizeBefore = vboModelData.GetCurrentSize();
		iMeshStartIndices.push_back(iSizeBefore / iVertexTotalSize);
		//FOR(j, iMeshFaces)
		for(int j =0; j < iMeshFaces; j++){
			const aiFace& face = mesh->mFaces[j];
			//FOR(k, 3)
			for(int k=0; k < 3; k++){
				aiVector3D pos = mesh->mVertices[face.mIndices[k]];
				aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
				aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
				vboModelData.AddData(&pos, sizeof(aiVector3D));
				vboModelData.AddData(&uv, sizeof(aiVector2D));
				vboModelData.AddData(&normal, sizeof(aiVector3D));
			}
		}
		int iMeshVertices = mesh->mNumVertices;
		iTotalVertices += iMeshVertices;
		iMeshSizes.push_back((vboModelData.GetCurrentSize() - iSizeBefore) / iVertexTotalSize);
	}
	iNumMaterials = scene->mNumMaterials;

	vector<int> materialRemap(iNumMaterials);

	//FOR(i, iNumMaterials)
	for(int i = 0; i < iNumMaterials; i++){
		const aiMaterial* material = scene->mMaterials[i];
		int a = 5;
		int texIndex = 0;
		aiString path;  // filename

		if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS)
		{
			string sDir = GetDirectoryPath(sFilePath);
			string sTextureName = path.data;
			string sFullPath = sDir + sTextureName;
			int iTexFound = -1;
			//FOR(j, ESZ(tTextures))if (sFullPath == tTextures[j].GetPath())
			for (int j = 0; j < tTextures.size(); j++) {
				if (sFullPath == tTextures[j].GetPath()) {
					iTexFound = j;
					break;
				}
			}
			if (iTexFound != -1)
				materialRemap[i] = iTexFound;
			else
			{
				AssimpTexture tNew;
				if (tNew.LoadTexture2D(sFullPath, true)) {
					materialRemap[i] = tTextures.size();//ESZ(tTextures);
					tTextures.push_back(tNew);
					cout << "Loaded Texture: " << sFullPath << endl;
				}
				else {
					cout << "Failed to Load Texture: " << sFullPath << endl;
				}
			}
		}
	}

	//FOR(i, ESZ(iMeshSizes))
	for(int i = 0; i < iMeshSizes.size(); i++){
		int iOldIndex = iMaterialIndices[i];
		iMaterialIndices[i] = materialRemap[iOldIndex];
	}
	cout << "Loaded Model: " << sFilePath << endl;
	return bLoaded = true;
}

/*-----------------------------------------------

Name:	FinalizeVBO

Params: none

Result: Uploads all loaded model data in one global
models' VBO.

/*---------------------------------------------*/
void AssimpModel::FinalizeVBO()
{
	glGenVertexArrays(1, &uiVAO);
	glBindVertexArray(uiVAO);
	vboModelData.BindVBO();
	vboModelData.UploadDataToGPU(GL_STATIC_DRAW);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(aiVector3D) + sizeof(aiVector2D), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(aiVector3D) + sizeof(aiVector2D), (void*)sizeof(aiVector3D));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(aiVector3D) + sizeof(aiVector2D), (void*)(sizeof(aiVector3D) + sizeof(aiVector2D)));
}

/*-----------------------------------------------

Name:	BindModelsVAO

Params: none

Result: Binds VAO of models with their VBO.

/*---------------------------------------------*/
void AssimpModel::BindModelsVAO()
{
	glBindVertexArray(uiVAO);
}

/*-----------------------------------------------

Name:	RenderModel

Params: none

Result: Guess what it does ^^.

/*---------------------------------------------*/
void AssimpModel::RenderModel()
{
	if (!bLoaded)return;
	int iNumMeshes = iMeshSizes.size();//ESZ(iMeshSizes);
	//FOR(i, iNumMeshes)
	for(int i = 0; i < iNumMeshes; i++){
		int iMatIndex = iMaterialIndices[i];
		tTextures[iMatIndex].BindTexture();
		glDrawArrays(GL_TRIANGLES, iMeshStartIndices[i], iMeshSizes[i]);
	}
}