#pragma once
#include "Transform.h"
#include "ShaderStructs.h"

namespace SimpleEngine
{
	class Material;

	class DebugRenderer
	{
	public:

		DebugRenderer(const DebugRenderer&) = delete;
		DebugRenderer& operator=(const DebugRenderer&) = delete;

		void Init(Microsoft::WRL::ComPtr<ID3D11Device> device);
		void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void Update(float deltaTime);

		void DrawDebugSphere(const Transform& centerTransform,
			float radius = 1,
			DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1.0, 0.0, 0.0, 1.0),
			float lifeTime = -1);

		void DrawDebugBox(const Transform& centerTransform,
			DirectX::SimpleMath::Vector3 size = DirectX::SimpleMath::Vector3(1.0, 1.0, 1.0),
			DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1.0, 0.0, 0.0, 1.0),
			float lifeTime = -1);

		void DrawDebugVector(DirectX::SimpleMath::Vector3 start,
			DirectX::SimpleMath::Vector3 end, 
			DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1.0, 0.0, 0.0, 1.0),
			float lifeTime = -1);

		void DrawDebugGrid(float cellSize = 1);

		static std::shared_ptr<DebugRenderer> GetInstance();
		
	private:
		DebugRenderer();

		void InitObjectConstBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device);
		void UpdateObjectConstBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, const Transform& transform);

		void InitVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::vector<VertexData>& vertecis);
		void InitIndexBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::vector<uint32_t>& indecis);

		void InitGridVertecis(std::vector<SimpleEngine::VertexData>& vertecis, std::vector<uint32_t>& indecis);
		void InitVectorVertecis(std::vector<SimpleEngine::VertexData>& vertecis, std::vector<uint32_t>& indecis);
		void InitBoxVerticis(std::vector<SimpleEngine::VertexData>& vertecis, std::vector<uint32_t>& indecis);
		void InitSphereVerticis(std::vector<SimpleEngine::VertexData>& vertecis, std::vector<uint32_t>& indecis);

	private:

		static inline std::shared_ptr<DebugRenderer> sInstance = nullptr;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;

		ObjectConstBufferData mObjectConstBufferData;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mObjectConstBuffer;

		enum class DebugShapeType
		{
			Grid,
			Vector,
			Sphere,
			Box
		};

		struct DebugShape {
			Transform mTransform;
			DebugShapeType mType;
			std::shared_ptr<Material> mMaterial;
			bool mIsImmortal;
			float mLifeTime;
		};

		std::map<DebugShapeType, uint32_t> mIndexCounts;
		std::map<DebugShapeType, uint32_t> mStartIndexLocations;
		std::vector<DebugShape> mDebugShapes;
	};
}

