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

		void init(Microsoft::WRL::ComPtr<ID3D11Device> device);

		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

	public:
		static const std::string kDefaultPSName;
		static const std::string kDefaultVSName;

		void setPSFileName(const std::string& PSFileName);
		std::string PSFileName() const;

		void setVSFileName(const std::string& VSFileName);
		std::string VSFileName() const;

	private:
		std::string mPSFileName;
		std::string mVSFileName;

		// Fong lighting
		std::string mColorTextureName;
		DirectX::SimpleMath::Color mColor;

		DirectX::SimpleMath::Vector4 mAmbient;

		std::string mSpecularAlphaTextureName;
		DirectX::SimpleMath::Vector4 mSpecularAlpha;

		//DirectX
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;

		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> mPixelShaderByteCode;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> mVertexShaderByteCode;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRastState;
	};
}

