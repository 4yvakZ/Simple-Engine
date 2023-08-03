#pragma once

namespace SimpleEngine {
	class Mesh;

	class AssetManager
	{
		static std::vector<SimpleEngine::Mesh> importMeshes(std::string modelFileName);

	private:
		static void searchNode(const aiScene* scene, aiNode* node, std::vector<SimpleEngine::Mesh>& meshes);
	};
}
