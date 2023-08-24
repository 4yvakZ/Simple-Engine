#pragma once

namespace SimpleEngine 
{

	enum class MaterialType
	{
		Opacue,
		Light,
		ColorPass,
		Debug
	};

	class Material
	{
	public:
		

		Material();

		Material(const Material& other);
		Material& operator=(const Material& other);

		//Material(Material&& other);
		//Material& operator=(Material&& other);

		void Init();

		void CompileShaders(Microsoft::WRL::ComPtr<ID3D11Device>& device, D3D_SHADER_MACRO* shaderMacros);

		void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void UnbindResources(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

		void SetPSFileName(const std::string& PSFileName);
		std::string PSFileName() const;

		void SetVSFileName(const std::string& VSFileName);
		std::string VSFileName() const;

		void SetType(MaterialType type);
		MaterialType GetType() const;
		bool IsDeferred() const;

		void SetAlbedo(const std::string& albedoFileName);
		void SetAlbedo(DirectX::SimpleMath::Color color);

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateTexture(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::string& textureFileName);
		Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateTexture(Microsoft::WRL::ComPtr<ID3D11Device> device, const DirectX::SimpleMath::Color& color);
		Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateTexture(Microsoft::WRL::ComPtr<ID3D11Device> device, const float value);

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateTextureSRV(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11Texture2D> texture);
	private:
		std::string mPSFileName;
		std::string mVSFileName;

		MaterialType mType = MaterialType::Opacue;

		// Textures
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mAlbedoMapTexture;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mNormalMapTexture;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mMetallicRoughnessAOMapTexture;
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

