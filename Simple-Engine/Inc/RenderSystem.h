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

		auto getDevice() {
			return mDevice;
		}

		void addRenderComponent(std::shared_ptr<RenderComponent> renderComponent);

	private:

		void initFrameConstBuffer();

		void updateFrameConstBuffer(const FrameConstBufferData& frameBufferData);

	private:
		
		FrameConstBufferData mFrameConstBufferData;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mFrameConstBuffer;

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> mBackBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTarget;

		DirectX::SimpleMath::Viewport mViewport;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRastState;

		//NOTE possible optimization (CPU cahce) create separate vectors without pointers for each Renderer type
		std::vector<std::weak_ptr<RenderComponent>> mRenderComponents;
	};
}

