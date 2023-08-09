#pragma once

struct aiScene;
struct aiNode;

namespace SimpleEngine {
	struct Mesh;

	class AssetManager
	{
	public:
		static std::vector<std::shared_ptr<Mesh>> importMeshes(std::string modelFileName);

	private:
		static void searchNode(const aiScene* scene, aiNode* node, std::vector<std::shared_ptr<Mesh>>& meshes);
	};
}
