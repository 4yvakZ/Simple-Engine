#pragma once
#include "ShaderStructs.h"

namespace SimpleEngine {
	struct Mesh
	{

		Mesh() = default;
		friend class AssetManager;

		const std::vector<SimpleEngine::Vertex>& Getvertices() const;

		const std::vector<uint32_t>& Getindices() const;
	private:
		std::vector<SimpleEngine::Vertex> mVertices;
		std::vector<uint32_t> mIndices;
	};
}

