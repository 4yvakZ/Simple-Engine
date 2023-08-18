#pragma once
#include "ShaderStructs.h"

namespace SimpleEngine {

	class DirectionalLightComponent;
	class RenderComponent;
	class MeshRenderer;
	
	class RenderSystem
	{
	public:
		explicit RenderSystem(HWND hWnd, int ClientWidth, int ClientHeight);
		virtual ~RenderSystem();

		virtual void Init(HWND hWnd, int ClientWidth, int ClientHeight);

		virtual void PrepareFrame();

		virtual void Draw();

		virtual void EndFrame();

		virtual void Update(const FrameConstBufferData& frameBufferData);

		Microsoft::WRL::ComPtr<ID3D11Device> getDevice();
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> getContext();

		void AddRenderComponent(std::shared_ptr<RenderComponent> renderComponent);

		void AddDirectionalLightComponent(std::shared_ptr<DirectionalLightComponent> lightComponent);

	private:

		void InitFrameConstBuffer();

		void UpdateFrameConstBuffer();

	private:
		
		FrameConstBufferData mFrameConstBufferData;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mFrameConstBuffer;

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> mBackBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTarget;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
		DirectX::SimpleMath::Viewport mViewport;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRastState;

		//NOTE possible optimization point (CPU cahce)
		std::vector<std::weak_ptr<RenderComponent>> mRenderComponents;

		std::vector<std::weak_ptr<DirectionalLightComponent>> mDirectionalLightComponents;
	};
}

