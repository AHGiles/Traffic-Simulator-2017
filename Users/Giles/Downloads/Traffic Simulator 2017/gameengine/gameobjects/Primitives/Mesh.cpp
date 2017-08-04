#include "Mesh.h"

using namespace std;



Mesh::Mesh(const std::string file)
{
	//IndexedModel model = OBJModel(file).ToIndexedModel();

	//Init(model);

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile((file).c_str(),
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace);

	if (!scene)
	{
		std::cout << "Mesh load failed!: " << file << std::endl;
		assert(0 == 0);
	}

	const aiMesh* model = scene->mMeshes[0];

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	//std::vector<Vector3f> tangents;
	std::vector<unsigned int> indices;

	const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < model->mNumVertices; i++)
	{
		const aiVector3D pos = model->mVertices[i];
		const aiVector3D normal = model->mNormals[i];
		const aiVector3D texCoord = model->HasTextureCoords(0) ? model->mTextureCoords[0][i] : aiZeroVector;
		const aiVector3D tangent = model->mTangents[i];

		positions.push_back(glm::vec3(pos.x, pos.y, pos.z));
		texCoords.push_back(glm::vec2(texCoord.x, texCoord.y));
		normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
		//tangents.push_back(Vector3f(tangent.x, tangent.y, tangent.z));
	}

	for (unsigned int i = 0; i < model->mNumFaces; i++)
	{
		const aiFace& face = model->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	IndexedModel m(positions, texCoords, normals, indices);
	Init(m);
	//s_resourceMap.insert(std::pair<std::string, MeshData*>(fileName, m_meshData));

	
}



Mesh::Mesh(Vert* verts, unsigned int numVerts, unsigned int* indices, unsigned int numIndices)
{
	//drawCount = numVerts;
	//drawCount = numIndices;
	IndexedModel model;


	glm::vec3 min(500000, 500000, 500000), max(-500000, -500000, -500000);


	for (int i = 0;i < numVerts;i++)
	{
		model.positions.push_back(*(verts[i].getPos()));
		model.texCoords.push_back(*(verts[i].getTex()));
		model.normals.push_back(*(verts[i].getNormal()));

		if (model.positions[i].x < min.x)
			min.x = model.positions[i].x;
		if(model.positions[i].y < min.y)
			min.y = model.positions[i].y;
		if (model.positions[i].z < min.z)
			min.z = model.positions[i].z;
		if (model.positions[i].x > max.x)
			max.x = model.positions[i].x;
		if (model.positions[i].y > max.y)
			max.y = model.positions[i].y;
		if (model.positions[i].z > max.z)
			max.z = model.positions[i].z;
	}
	
	this->AABB = Box(min.x, min.y, min.z, max.x, max.y, max.z);

	for (int i = 0; i < numIndices; i++)
		model.indices.push_back(indices[i]);

	Init(model);
																							   

}

void Mesh::Init(const IndexedModel& model)
{
	//drawCount = numVerts;
	numIndices = model.indices.size();
	numVerts = model.positions.size();


	glGenVertexArrays(1, &VAO); // Create space for a vertex array 
	glBindVertexArray(VAO); // Bind our VAO
	glGenBuffers(NUM_BUFFERS, VAB); // Generate our buffers

	glBindBuffer(GL_ARRAY_BUFFER, VAB[POSITION_VB]); // Bind our buffer at the given point in the VB
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW); // Takes in data from RAM and puts it into the buffer on the GPU

	glEnableVertexAttribArray(0); // All data is a single attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Tells GPU what to do with each attribute, 3 pieces of data(vec3), and they are floats

	// Position data is now on the GPU

	glBindBuffer(GL_ARRAY_BUFFER, VAB[TEXTURE_VB]); // Bind our buffer at the given point in the VB
	glBufferData(GL_ARRAY_BUFFER, model.texCoords.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW); // Takes in data from RAM and puts it into the buffer on the GPU

	glEnableVertexAttribArray(1); // All data is a single attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); // Tells GPU what to do with each attribute, 2 pieces of data(vec2), and they are floats

	// Texture data is now on the GPU

	glBindBuffer(GL_ARRAY_BUFFER, VAB[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Normal data is now on GPU

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VAB[INDEX_VB]); // Bind our buffer at the given point in the VB, though this is an element array, not just an array buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW); // Takes in data from RAM and puts it into the buffer on the GPU



	// GPU can now draw the mesh
	glBindVertexArray(0); // Unbind VAO and cleanup


}



Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO); // Delete the VAO

}

void Mesh::Draw()
{
	glBindVertexArray(VAO); // Bind our VAO at the start

	//glDrawElementsBaseVertex(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0, 0);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_QUADS, 0, numVerts);

	openGLErrorCheck(__FILE__, __LINE__);

	//glDrawArrays(GL_TRIANGLES, 0, drawCount); // Tell GPU how to draw the mesh and how far to draw(drawCount)


	glBindVertexArray(0); // Unbind at the end
	
}





bool Mesh::AABBCollision(glm::vec3 point) // AABB on Point Collision	
{
	return (point.x >= this->AABB.minX && point.x <= this->AABB.maxX) &&
		(point.y >= this->AABB.minY && point.y <= this->AABB.maxY) &&
		(point.z >= this->AABB.minZ && point.z <= this->AABB.maxZ);
}
