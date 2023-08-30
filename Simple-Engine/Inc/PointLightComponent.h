#pragma once
#include "SceneComponent.h"
#include "ShaderStructs.h"

namespace SimpleEngine
{
	class Material;
	class PointLightComponent : public SceneComponent
	{
	public:
		PointLightComponent();

		// Inherited via SceneComponent
		void Update() override;
		void Init() override;
		void OnConstructed() override;

		void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

		DirectX::SimpleMath::Vector4 GetLightIntensity() const;
		void SetLightIntensity(DirectX::SimpleMath::Vector4 lightIntensity);

	private:
		void InitLightConstBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device);
		void UpdateLightConstBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

		void InitDepthStencilStates(Microsoft::WRL::ComPtr<ID3D11Device> device);

		void InitObjectConstBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device);
		void UpdateObjectConstBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

		void InitVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::vector<Vertex>& vertices);
		void InitIndexBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::vector<uint32_t>& indices);

		void CreateGeospherevertices(std::vector<SimpleEngine::Vertex>& vertices, std::vector<uint32_t>& indices);

	private:
		uint32_t mNumberOfIndices = 0;

		LightConstBufferData mLightConstBufferData;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mLightConstBuffer;

		std::shared_ptr<Material> mMeshMaterial;
		std::shared_ptr<Material> mQuadMaterial;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilStateLess;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilStateGreater;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilStateOff;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;

		ObjectConstBufferData mObjectConstBufferData;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mObjectConstBuffer;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> mFrontCullingRastState;

		float mLightRadius = 4;
	};
}

