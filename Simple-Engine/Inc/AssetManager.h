#pragma once

struct aiScene;
struct aiNode;

namespace SimpleEngine {
	struct Mesh;

	class AssetManager
	{
	public:
		static std::vector<std::shared_ptr<Mesh>> ImportMeshes(std::string modelFileName);

	private:
		static void SearchNode(const aiScene* scene, aiNode* node, std::vector<std::shared_ptr<Mesh>>& meshes);
	};
}
