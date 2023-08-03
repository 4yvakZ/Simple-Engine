#pragma once
#include "ShaderStructs.h"

namespace SimpleEngine {
	class Mesh
	{
	public:
		Mesh() = default;

		std::vector<SimpleEngine::VertexData> mVertecis;
		std::vector<uint32_t> mIndecis;
	};
}

