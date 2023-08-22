#pragma once

namespace SimpleEngine {

	enum class MaterialType {
		Opaque,
		Translucent,
		Masked
	};

	class Material
	{
	public:
		Material();

		void Init(Microsoft::WRL::ComPtr<ID3D11Device> device);

		void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

		void SetPSFileName(const std::string& PSFileName);
		std::string PSFileName() const;

		void SetVSFileName(const std::string& VSFileName);
		std::string VSFileName() const;

		bool IsDeferred() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> InitTextureSRV(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::string& textureFileName);
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> InitTextureSRV(Microsoft::WRL::ComPtr<ID3D11Device> device, const DirectX::SimpleMath::Color& color);
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> InitTextureSRV(Microsoft::WRL::ComPtr<ID3D11Device> device, const float value);

	private:
		std::string mPSFileName;
		std::string mVSFileName;

		bool mIsDeferred = true;

		// Textures
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mAlbedoMap;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mNormalMap;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mMetallicRoughnessAOMap;


		//DirectX
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;

		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> mPixelShaderByteCode;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> mVertexShaderByteCode;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRastState;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
	};
}

