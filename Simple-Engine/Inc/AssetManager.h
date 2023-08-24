#pragma once

struct aiScene;
struct aiNode;

namespace SimpleEngine {
	struct Mesh;
	class Material;
	enum class MaterialType;

	class AssetManager
	{
	public:
		AssetManager();

		AssetManager(const AssetManager&) = delete;
		AssetManager operator=(const AssetManager&) = delete;

		void Init();

		std::vector<std::shared_ptr<Mesh>> ImportMeshes(std::string modelFileName);
		static std::shared_ptr<AssetManager> GetInstance();

		std::shared_ptr<Material> CreateMaterial(MaterialType materialType);
		
		std::shared_ptr<Material> GetDefaultMaterial();
		std::shared_ptr<Material> GetDefaultLightMaterial();
		std::shared_ptr<Material> GetDefaultColorPassMaterial();
		std::shared_ptr<Material> GetDefaultDebugMaterial();

	private:
		void SearchNode(const aiScene* scene, aiNode* node, std::vector<std::shared_ptr<Mesh>>& meshes);

	private:

		static inline std::shared_ptr<AssetManager> sInstance = nullptr;
		std::shared_ptr<Material> mDefaultMaterial;
		std::shared_ptr<Material> mDefaultDirectLightMaterial;
		std::shared_ptr<Material> mDefaultColorPassMaterial;
		std::shared_ptr<Material> mDefaultDebugMaterial;

		std::vector<std::shared_ptr<Material>> mMaterials;
	};
}
