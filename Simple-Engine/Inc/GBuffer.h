#pragma once

namespace SimpleEngine {
	class GBuffer
	{
	public:
		GBuffer(DirectX::SimpleMath::Viewport viewport);

		void Init(Microsoft::WRL::ComPtr<ID3D11Device> device);
		void Clear(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void SetAsTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

	private:
		void InitDepthResouces(Microsoft::WRL::ComPtr<ID3D11Device> device);
		void InitResource(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11Texture2D>& texture, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourseView, Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& renderTargetView);
	private:
		//TODO move to one place
		DirectX::SimpleMath::Viewport mViewport;

		Microsoft::WRL::ComPtr <ID3D11Texture2D> mDepthBuffer;
		Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> mDepthSRV;
		Microsoft::WRL::ComPtr <ID3D11DepthStencilView> mDepthView;

		Microsoft::WRL::ComPtr <ID3D11Texture2D> mPositionTexture;
		Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> mPositionSRV;
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> mPositionRTV;

		Microsoft::WRL::ComPtr <ID3D11Texture2D> mNormalTexture;
		Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> mNormalSRV;
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> mNormalRTV;

		Microsoft::WRL::ComPtr <ID3D11Texture2D> mAlbedoTexture;
		Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> mAlbedoSRV;
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> mAlbedoRTV;

		Microsoft::WRL::ComPtr <ID3D11Texture2D> mMetallicRoughtnessAOTexture;
		Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> mMetallicRoughtnessAOSRV;
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> mMetallicRoughtnessAORTV;
	};
}

