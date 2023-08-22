#include "pch.h"
#include "AssetManager.h"
#include "Mesh.h"
#include "Material.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags  

using namespace DirectX::SimpleMath;

std::shared_ptr<SimpleEngine::AssetManager> SimpleEngine::AssetManager::sInstance = nullptr;

const std::string kDefaultLightPSName = "../shaders/DefaultDeferredLightPS.hlsl";
const std::string kDefaultLightVSName = "../shaders/DefaultAlignedQuadVS.hlsl";

const std::string kDefaultColorPassPSName = "../shaders/DefaultDeferredColorPassPS.hlsl";
const std::string kDefaultColorPassVSName = "../shaders/DefaultAlignedQuadVS.hlsl";

SimpleEngine::AssetManager::AssetManager()
{
	mDefaultMaterial = std::make_shared<Material>();

	mDefaultDirectLightMaterial = std::make_shared<Material>();
	mDefaultDirectLightMaterial->SetPSFileName(kDefaultLightPSName);
	mDefaultDirectLightMaterial->SetVSFileName(kDefaultLightVSName);
	mDefaultDirectLightMaterial->SetType(Material::Type::Light);

	mDefaultColorPassMaterial = std::make_shared<Material>();
	mDefaultColorPassMaterial->SetPSFileName(kDefaultColorPassPSName);
	mDefaultColorPassMaterial->SetVSFileName(kDefaultColorPassVSName);
	mDefaultColorPassMaterial->SetType(Material::Type::ColorPass);
}

void SimpleEngine::AssetManager::Init()
{
	mDefaultColorPassMaterial->Init();
	mDefaultDirectLightMaterial->Init();
	mDefaultMaterial->Init();
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

std::shared_ptr<SimpleEngine::Material> SimpleEngine::AssetManager::GetDefaultMaterial()
{
	return mDefaultMaterial;
}

std::shared_ptr<SimpleEngine::Material> SimpleEngine::AssetManager::GetDefaultLightMaterial()
{
	return mDefaultDirectLightMaterial;
}

std::shared_ptr<SimpleEngine::Material> SimpleEngine::AssetManager::GetDefaultColorPassMaterial()
{
	return mDefaultColorPassMaterial;
}

void SimpleEngine::AssetManager::SearchNode(const aiScene* scene, aiNode* node, std::vector<std::shared_ptr<Mesh>>& meshes)
{
	if (node->mNumMeshes > 0)
	{
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			std::shared_ptr<Mesh> myMesh = std::make_shared<Mesh>();
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			size_t nVertecis = mesh->mNumVertices;
			aiVector3D* vertecis = mesh->mVertices;

			for (size_t i = 0; i < nVertecis; i++)
			{
				aiVector3D vertex = vertecis[i];

				Vector3 point = Vector3(vertex.x,
					vertex.y,
					vertex.z);

				Vector2 UV = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				Vector3 normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				Vector3 tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

				myMesh->mVertecis.emplace_back(point, UV, normal, tangent);
			}


			size_t nFaces = mesh->mNumFaces;
			aiFace* meshFaces = mesh->mFaces;
			for (size_t i = 0; i < nFaces; i++)
			{
				myMesh->mIndecis.push_back(meshFaces[i].mIndices[0]);
				myMesh->mIndecis.push_back(meshFaces[i].mIndices[1]);
				myMesh->mIndecis.push_back(meshFaces[i].mIndices[2]);
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
