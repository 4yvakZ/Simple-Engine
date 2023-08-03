#include "pch.h"
#include "AssetManager.h"
#include "Mesh.h"

using namespace DirectX::SimpleMath;

std::vector<SimpleEngine::Mesh> SimpleEngine::AssetManager::importMeshes(std::string modelFileName)
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
		return std::vector<Mesh>();
	}

	auto meshes = std::vector<Mesh>();

	searchNode(scene, scene->mRootNode, meshes);

	return meshes;
}

void SimpleEngine::AssetManager::searchNode(const aiScene* scene, aiNode* node, std::vector<Mesh>& meshes)
{
	if (node->mNumMeshes > 0)
	{
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			Mesh myMesh;
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
				Vector3 normal;
				//if (isZUp)
				//{
				normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].z, -mesh->mNormals[i].y);
				//}
				//else
				//{
				//	normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				//}

				myMesh.mVertecis.emplace_back(point, UV, normal);
			}


			size_t nFaces = mesh->mNumFaces;
			aiFace* meshFaces = mesh->mFaces;
			for (size_t i = 0; i < nFaces; i++)
			{
				myMesh.mIndecis.push_back(meshFaces[i].mIndices[0]);
				myMesh.mIndecis.push_back(meshFaces[i].mIndices[1]);
				myMesh.mIndecis.push_back(meshFaces[i].mIndices[2]);
			}

			meshes.push_back(myMesh);
		}
	}

	// continue for all child nodes
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		searchNode(scene, node->mChildren[i], meshes);
	}
}
