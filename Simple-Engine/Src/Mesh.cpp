#include "pch.h"
#include "Mesh.h"

const std::vector<SimpleEngine::Vertex>& SimpleEngine::Mesh::Getvertices() const
{
	return mVertices;
}

const std::vector<uint32_t>& SimpleEngine::Mesh::Getindices() const
{
	return mIndices;
}
