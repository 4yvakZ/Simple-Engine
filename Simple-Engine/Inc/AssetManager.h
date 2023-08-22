#pragma once

struct aiScene;
struct aiNode;

namespace SimpleEngine {
	struct Mesh;
	class Material;

	class AssetManager
	{
	public:
		AssetManager();

		AssetManager(const AssetManager&) = delete;
		AssetManager operator=(const AssetManager&) = delete;

		void Init();

		std::vector<std::shared_ptr<Mesh>> ImportMeshes(std::string modelFileName);
		static std::shared_ptr<AssetManager> GetInstance();

		//uint32_t AddMaterial(std::shared_ptr<Material> material);
		//std::shared_ptr<Material> GetMaterial(uint32_t materialIndex);
		
		std::shared_ptr<Material> GetDefaultMaterial();
		std::shared_ptr<Material> GetDefaultLightMaterial();
		std::shared_ptr<Material> GetDefaultColorPassMaterial();

	private:
		void SearchNode(const aiScene* scene, aiNode* node, std::vector<std::shared_ptr<Mesh>>& meshes);

	private:

		static std::shared_ptr<AssetManager> sInstance;
		std::shared_ptr<Material> mDefaultMaterial;
		std::shared_ptr<Material> mDefaultDirectLightMaterial;
		std::shared_ptr<Material> mDefaultColorPassMaterial;

		//std::vector<std::shared_ptr<Material>> mMaterials;
	};
}
