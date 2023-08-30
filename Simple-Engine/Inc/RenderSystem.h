#pragma once
#include "ShaderStructs.h"

namespace SimpleEngine {

	class DirectionalLightComponent;
	class PointLightComponent;
	class RenderComponent;
	class MeshRenderer;
	class GBuffer;
	class DebugRenderer;
	
	class RenderSystem
	{
	public:
		enum class DebugFlag {
			None = -1,
			WorldPosition = 0,
			Normals = 1,
			Albedo = 2,
			MetallicRoughnessAO = 3,
			Lighting = 4
		};

		explicit RenderSystem(HWND hWnd, int ClientWidth, int ClientHeight);
		virtual ~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem operator=(const RenderSystem&) = delete;


		virtual void PrepareFrame();

		virtual void Draw();

		virtual void EndFrame();

		virtual void Update(float deltaTime, const FrameConstBufferData& frameBufferData);

		Microsoft::WRL::ComPtr<ID3D11Device> getDevice();
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> getContext();

		void AddRenderComponent(std::shared_ptr<RenderComponent> renderComponent);

		void AddDirectionalLightComponent(std::shared_ptr<DirectionalLightComponent> lightComponent);
		void AddPointLightComponent(std::shared_ptr<PointLightComponent> lightComponent);

		void SetDebugFlag(DebugFlag flag);

	protected:
		virtual void Init(HWND hWnd, int ClientWidth, int ClientHeight);

	private:
		void InitDepthStencilStateOff();

		void InitBlendState();

		void InitFrameConstBuffer();

		void UpdateFrameConstBuffer();

	private:
		
		FrameConstBufferData mFrameConstBufferData;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mFrameConstBuffer;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilStateOff;
		Microsoft::WRL::ComPtr<ID3D11BlendState> mBlendState;

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> mBackBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTarget;

		DirectX::SimpleMath::Viewport mViewport;

		std::unique_ptr<GBuffer> mGBuffer;
		std::shared_ptr<DebugRenderer> mDebugRenderer;

		//NOTE possible optimization point (CPU cahce)
		std::vector<std::weak_ptr<RenderComponent>> mRenderComponents;

		std::vector<std::weak_ptr<DirectionalLightComponent>> mDirectionalLightComponents;
		std::vector<std::weak_ptr<PointLightComponent>> mPointLightComponents;

		DebugFlag mDebugFlag = DebugFlag::None;
	};
}

