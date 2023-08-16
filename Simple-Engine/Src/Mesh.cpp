#include "pch.h"
#include "Mesh.h"

const std::vector<SimpleEngine::VertexData>& SimpleEngine::Mesh::GetVertecis() const
{
	return mVertecis;
}

const std::vector<uint32_t>& SimpleEngine::Mesh::GetIndecis() const
{
	return mIndecis;
}
