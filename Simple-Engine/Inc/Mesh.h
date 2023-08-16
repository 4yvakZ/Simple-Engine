#pragma once
#include "ShaderStructs.h"

namespace SimpleEngine {
	struct Mesh
	{

		Mesh() = default;
		friend class AssetManager;

		const std::vector<SimpleEngine::VertexData>& GetVertecis() const;

		const std::vector<uint32_t>& GetIndecis() const;
	private:
		std::vector<SimpleEngine::VertexData> mVertecis;
		std::vector<uint32_t> mIndecis;
	};
}

