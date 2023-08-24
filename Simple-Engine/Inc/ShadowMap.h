#pragma once
#include "ShaderStructs.h"

namespace SimpleEngine
{
	class Material;
	class DirectionalLightComponent;

	class ShadowMap
	{
	public:
		ShadowMap(std::shared_ptr<DirectionalLightComponent> dirLight, int width);

		void Init(Microsoft::WRL::ComPtr<ID3D11Device> device);

		void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void SetAsTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void Clear(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	private:
		void CompileShaders(Microsoft::WRL::ComPtr<ID3D11Device> device);
		void CreateShaderResources(Microsoft::WRL::ComPtr<ID3D11Device> device);

		void InitCascadeBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device);
		void UpdateCascadeBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		std::vector<DirectX::SimpleMath::Vector4> GetFrustrumCornersWorldSpace(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	public:
		static constexpr int kNumberOfCascades = 4;
	private:
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> mGeometryShader;
		Microsoft::WRL::ComPtr<ID3DBlob> mGeometryShaderByteCode;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> mVertexShaderByteCode;

		Microsoft::WRL::ComPtr <ID3D11Texture2D> mShadowMap;
		Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> mShadowMapSRV;
		Microsoft::WRL::ComPtr <ID3D11DepthStencilView> mDepthView;

		Microsoft::WRL::ComPtr <ID3D11SamplerState> mSamplerState;
		Microsoft::WRL::ComPtr <ID3D11RasterizerState> mRastState;

		DirectX::SimpleMath::Viewport mViewport;

		CascadeData mCascadeData;
		Microsoft::WRL::ComPtr <ID3D11Buffer> mCascadeBuffer;

		std::weak_ptr<DirectionalLightComponent> mDirectionalLight;

		int mWidth;
	};
}

