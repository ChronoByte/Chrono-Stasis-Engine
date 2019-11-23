#pragma once

#include "csGlobals.h"
#include "csComponent.h"
#include "Resource.h"
#include "MathGeoLib/include/Math/float3.h"
class aiMesh;

template<typename T>
struct MeshInfo
{
	uint id = 0u;
	uint capacity = 0u;
	T* buffer = nullptr;
};

class ResourceMesh : public Resource
{
public:

	ResourceMesh(UID id, Resource::Type type);
	virtual ~ResourceMesh();

	void LoadMeshVertices(aiMesh* mesh);
	void LoadMeshIndices(aiMesh* mesh);
	void LoadMeshNormals(aiMesh* mesh);
	void LoadMeshFaceNormals();
	void LoadMeshVertexNormals();
	void LoadMeshColors(aiMesh* mesh, int index);
	void LoadMeshTextureCoords(aiMesh* mesh, int index);
	bool LoadMeshBuffers();

	const uint GetIndices() const;
	const uint GetVertices() const;
	const uint GetTextureCoords() const;
	const uint GetNormals() const;
	const uint GetTriangles() const;
	const uint GetFaceNormals() const;
	const uint GetVertexNormals() const;

	const MeshInfo<uint> GetMeshIndex() const;
	const MeshInfo<float> GetMeshVertex() const;
	const MeshInfo<float> GetMeshTextureCoords() const;
	const MeshInfo<float> GetMeshNormals() const;

	bool LoadResourceMesh();
	bool UnloadMeshBuffers();
	virtual bool LoadInMemory() override;
	virtual bool UnloadOutMemory() override;

public:
	MeshInfo<uint> index;
	MeshInfo<float> vertex;
	MeshInfo<float> colors;
	MeshInfo<float> normals;
	MeshInfo<float> faceNormals;
	MeshInfo<float> vertexNormals;
	MeshInfo<float> textureCoords;

	bool drawVertexNormals = false;
	bool drawFaceNormals = false;


};