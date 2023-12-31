#include "pch.h"
#include "AssetManager.h"
#include "Mesh.h"
#include "Material.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags  

using namespace DirectX::SimpleMath;

const std::string kDefaultPointLightMeshPSName = "../shaders/DefaultDeferredPointLightPS.hlsl";
const std::string kDefaultPointLightMeshVSName = "../shaders/DefaultDeferredPointLightVS.hlsl";

const std::string kDefaultPointLightQuadPSName = "../shaders/DefaultDeferredPointLightPS.hlsl";
const std::string kDefaultPointLightQuadVSName = "../shaders/DefaultAlignedQuadVS.hlsl";

const std::string kDefaultDirLightPSName = "../shaders/DefaultDeferredDirLightPS.hlsl";
const std::string kDefaultDirLightVSName = "../shaders/DefaultAlignedQuadVS.hlsl";

const std::string kDefaultColorPassPSName = "../shaders/DefaultDeferredColorPassPS.hlsl";
const std::string kDefaultColorPassVSName = "../shaders/DefaultAlignedQuadVS.hlsl";

const std::string kDefaultDebugPSName = "../shaders/DefaultDebugPS.hlsl";
const std::string kDefaultDebugVSName = "../shaders/DefaultDebugVS.hlsl";

SimpleEngine::AssetManager::AssetManager()
{
	mDefaultMaterial = std::make_shared<Material>();

	mDefaultDirectionalLightMaterial = std::make_shared<Material>();
	mDefaultDirectionalLightMaterial->SetPSFileName(kDefaultDirLightPSName);
	mDefaultDirectionalLightMaterial->SetVSFileName(kDefaultDirLightVSName);
	mDefaultDirectionalLightMaterial->SetType(MaterialType::Light);

	mDefaultPointLightMeshMaterial = std::make_shared<Material>();
	mDefaultPointLightMeshMaterial->SetPSFileName(kDefaultPointLightMeshPSName);
	mDefaultPointLightMeshMaterial->SetVSFileName(kDefaultPointLightMeshVSName);
	mDefaultPointLightMeshMaterial->SetType(MaterialType::Light);

	mDefaultPointLightQuadMaterial = std::make_shared<Material>();
	mDefaultPointLightQuadMaterial->SetPSFileName(kDefaultPointLightQuadPSName);
	mDefaultPointLightQuadMaterial->SetVSFileName(kDefaultPointLightQuadVSName);
	mDefaultPointLightQuadMaterial->SetType(MaterialType::Light);

	mDefaultColorPassMaterial = std::make_shared<Material>();
	mDefaultColorPassMaterial->SetPSFileName(kDefaultColorPassPSName);
	mDefaultColorPassMaterial->SetVSFileName(kDefaultColorPassVSName);
	mDefaultColorPassMaterial->SetType(MaterialType::ColorPass);

	mDefaultDebugMaterial = std::make_shared<Material>();
	mDefaultDebugMaterial->SetPSFileName(kDefaultDebugPSName);
	mDefaultDebugMaterial->SetVSFileName(kDefaultDebugVSName);
	mDefaultDebugMaterial->SetType(MaterialType::Debug);
}

void SimpleEngine::AssetManager::Init()
{
	mDefaultDebugMaterial->Init();
	mDefaultColorPassMaterial->Init();
	mDefaultDirectionalLightMaterial->Init();
	mDefaultMaterial->Init();
	mDefaultPointLightQuadMaterial->Init();
	mDefaultPointLightMeshMaterial->Init();

	for (auto& material : mMaterials) {
		material->Init();
	}
}

std::vector<std::shared_ptr<SimpleEngine::Mesh>> SimpleEngine::AssetManager::ImportMeshes(std::string modelFileName)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(modelFileName,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_FlipWindingOrder
	);

	// If the import failed, report it
	if (nullptr == scene)
	{
		std::cout << "Missing file " << modelFileName << "\n";
		return std::vector<std::shared_ptr<Mesh>>();
	}

	auto meshes = std::vector<std::shared_ptr<Mesh>>();

	SearchNode(scene, scene->mRootNode, meshes);

	return meshes;
}

std::shared_ptr<SimpleEngine::AssetManager> SimpleEngine::AssetManager::GetInstance()
{
	if (!sInstance) {
		sInstance = std::make_shared<AssetManager>();
	}
	return sInstance;
}

std::shared_ptr<SimpleEngine::Material> SimpleEngine::AssetManager::CreateMaterial(MaterialType materialType)
{
	std::shared_ptr<Material> material;
	switch (materialType)
	{
	case SimpleEngine::MaterialType::Opacue:
		material = std::make_shared<Material>(*(mDefaultMaterial.get()));
		break;
	case SimpleEngine::MaterialType::Debug:
		material = std::make_shared<Material>(*(mDefaultDebugMaterial.get()));
		break;

	default:
		material = std::make_shared<Material>(*(mDefaultMaterial.get()));
		break;
	}
	mMaterials.push_back(material);
	return material;
}

std::shared_ptr<SimpleEngine::Material> SimpleEngine::AssetManager::GetDefaultMaterial()
{
	return mDefaultMaterial;
}

std::shared_ptr<SimpleEngine::Material> SimpleEngine::AssetManager::GetDefaultDirectionalLightMaterial()
{
	return mDefaultDirectionalLightMaterial;
}

std::shared_ptr<SimpleEngine::Material> SimpleEngine::AssetManager::GetDefaultPointLightMeshMaterial()
{
	return mDefaultPointLightMeshMaterial;
}

std::shared_ptr<SimpleEngine::Material> SimpleEngine::AssetManager::GetDefaultPointLightQuadMaterial()
{
	return mDefaultPointLightQuadMaterial;
}

std::shared_ptr<SimpleEngine::Material> SimpleEngine::AssetManager::GetDefaultColorPassMaterial()
{
	return mDefaultColorPassMaterial;
}

std::shared_ptr<SimpleEngine::Material> SimpleEngine::AssetManager::GetDefaultDebugMaterial()
{
	return mDefaultDebugMaterial;
}

void SimpleEngine::AssetManager::SearchNode(const aiScene* scene, aiNode* node, std::vector<std::shared_ptr<Mesh>>& meshes)
{
	if (node->mNumMeshes > 0)
	{
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			std::shared_ptr<Mesh> myMesh = std::make_shared<Mesh>();
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			size_t nvertices = mesh->mNumVertices;
			aiVector3D* vertices = mesh->mVertices;

			for (size_t i = 0; i < nvertices; i++)
			{
				aiVector3D vertex = vertices[i];

				Vector3 point = Vector3(vertex.x,
					vertex.y,
					vertex.z);

				Vector2 UV = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				Vector3 normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				Vector3 tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

				myMesh->mVertices.emplace_back(point, UV, normal, tangent);
			}


			size_t nFaces = mesh->mNumFaces;
			aiFace* meshFaces = mesh->mFaces;
			for (size_t i = 0; i < nFaces; i++)
			{
				myMesh->mIndices.push_back(meshFaces[i].mIndices[0]);
				myMesh->mIndices.push_back(meshFaces[i].mIndices[1]);
				myMesh->mIndices.push_back(meshFaces[i].mIndices[2]);
			}

			meshes.push_back(myMesh);
		}
	}

	// continue for all child nodes
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		SearchNode(scene, node->mChildren[i], meshes);
	}
}
