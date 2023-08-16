#pragma once
#include "ShaderStructs.h"

namespace SimpleEngine {

	class RenderComponent;
	class MeshRenderer;
	
	class RenderSystem
	{
	public:
		explicit RenderSystem(HWND hWnd, int clientWidth, int clientHeight);
		virtual ~RenderSystem();

		virtual void init(HWND hWnd, int clientWidth, int clientHeight);

		virtual void prepareFrame();

		virtual void draw();

		virtual void endFrame();

		virtual void update(const FrameConstBufferData& frameBufferData);

		Microsoft::WRL::ComPtr<ID3D11Device> getDevice();
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> getContext();

		void addRenderComponent(std::shared_ptr<RenderComponent> renderComponent);

	private:

		void initFrameConstBuffer();

		void updateFrameConstBuffer();

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
	};
}

