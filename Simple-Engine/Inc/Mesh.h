#pragma once
#include "ShaderStructs.h"

namespace SimpleEngine {
	struct Mesh
	{

		Mesh() = default;
		friend class AssetManager;

		const auto& getVertecis() const
		{
			return mVertecis;
		}

		const auto& getIndecis() const
		{
			return mIndecis;
		}
	private:
		std::vector<SimpleEngine::VertexData> mVertecis;
		std::vector<uint32_t> mIndecis;
	};
}

