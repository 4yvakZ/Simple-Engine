#include "pch.h"
#include "Material.h"

#include <DirectXTex.h>

const std::string kDefaultPSName = "../shaders/DefaultPS.hlsl";
const std::string kDefaultVSName = "../shaders/DefaultVS.hlsl";
const std::string kDefaultAlbedoName = "../assets/debug.jpg";

constexpr DirectX::SimpleMath::Vector3 kDefaultNormal = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f);
constexpr float kDefaultMetalic = 0.0f; 
constexpr float kDefaultRoughtness = 0.5f;
constexpr float kDefaultAO = 1.0f;

SimpleEngine::Material::Material():
	mPSFileName(kDefaultPSName),
	mVSFileName(kDefaultVSName)
{
}

void SimpleEngine::Material::Init(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	using namespace DirectX::SimpleMath;
	D3D_SHADER_MACRO shaderMacros[] = {nullptr, nullptr };

	ID3DBlob* errorVertexCode = nullptr;
	std::wstring VSFileName(mVSFileName.begin(), mVSFileName.end());
	auto res = D3DCompileFromFile(VSFileName.c_str(),
		shaderMacros /*macros*/,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		mVertexShaderByteCode.GetAddressOf(),
		&errorVertexCode);
	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			std::cout << "Missing Shader File: " << mVSFileName << std::endl;
		}

		return;
	}

	///pixelShaderByteCode initialization
	std::wstring PSFileName(mPSFileName.begin(), mPSFileName.end());
	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(PSFileName.c_str(),
		shaderMacros /*macros*/,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		mPixelShaderByteCode.GetAddressOf(),
		&errorPixelCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorPixelCode) {
			char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			std::cout << "Missing Shader File: " << mPSFileName << std::endl;
		}

		return;
	}

	///vertexShader initialization
	device->CreateVertexShader(
		mVertexShaderByteCode->GetBufferPointer(),
		mVertexShaderByteCode->GetBufferSize(),
		nullptr, mVertexShader.GetAddressOf());

	///pixelShader initialization
	device->CreatePixelShader(
		mPixelShaderByteCode->GetBufferPointer(),
		mPixelShaderByteCode->GetBufferSize(),
		nullptr, mPixelShader.GetAddressOf());

	///layout initialization
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};

	res = device->CreateInputLayout(
		inputElements,
		std::size(inputElements),
		mVertexShaderByteCode->GetBufferPointer(),
		mVertexShaderByteCode->GetBufferSize(),
		mInputLayout.GetAddressOf());

	///rastState initialization
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	device->CreateRasterizerState(&rastDesc, mRastState.GetAddressOf());

	///samplerState initialization

	D3D11_SAMPLER_DESC sampDesc;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.MaxLOD = INT_MAX;
	sampDesc.MipLODBias = 0;
	sampDesc.MaxAnisotropy = 16;

	res = device->CreateSamplerState(&sampDesc, mSamplerState.GetAddressOf());

	mAlbedoMap = InitTextureSRV(device, kDefaultAlbedoName);
	mNormalMap = InitTextureSRV(device, Color(kDefaultNormal));
	mMetalicMap = InitTextureSRV(device, kDefaultMetalic);
	mRoughnessMap = InitTextureSRV(device, kDefaultRoughtness);
	mAOMap = InitTextureSRV(device, kDefaultAO);

}

void SimpleEngine::Material::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	context->RSSetState(mRastState.Get());
	context->IASetInputLayout(mInputLayout.Get());

	///Set Vertex and Pixel Resources
	context->VSSetShader(mVertexShader.Get(), nullptr, 0);
	context->PSSetShader(mPixelShader.Get(), nullptr, 0);
	
	context->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());
	context->PSSetShaderResources(0, 1, mAlbedoMap.GetAddressOf());
	context->PSSetShaderResources(1, 1, mNormalMap.GetAddressOf());
	context->PSSetShaderResources(2, 1, mMetalicMap.GetAddressOf());
	context->PSSetShaderResources(3, 1, mRoughnessMap.GetAddressOf());
	context->PSSetShaderResources(4, 1, mAOMap.GetAddressOf());
}

void SimpleEngine::Material::SetPSFileName(const std::string& PSFileName)
{
	mPSFileName = PSFileName;
}

std::string SimpleEngine::Material::PSFileName() const
{
	return mPSFileName;
}

void SimpleEngine::Material::SetVSFileName(const std::string& VSFileName)
{
	mVSFileName = VSFileName;
}

std::string SimpleEngine::Material::VSFileName() const
{
	return mVSFileName;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SimpleEngine::Material::InitTextureSRV(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::string& textureFileName)
{
	std::wstring fileName(textureFileName.begin(), textureFileName.end());
	DirectX::ScratchImage image;
	HRESULT res;
	if (_wcsicmp(fileName.c_str(), L".dds") == 0)
	{
		res = LoadFromDDSFile(fileName.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else if (_wcsicmp(fileName.c_str(), L".tga") == 0)
	{
		res = LoadFromTGAFile(fileName.c_str(), nullptr, image);
	}
	else if (_wcsicmp(fileName.c_str(), L".hdr") == 0)
	{
		res = LoadFromHDRFile(fileName.c_str(), nullptr, image);
	}
	else
	{
		res = LoadFromWICFile(fileName.c_str(), DirectX::WIC_FLAGS_DEFAULT_SRGB, nullptr, image);
	}

	if (SUCCEEDED(res))
	{
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV;
		res = CreateShaderResourceView(device.Get(),
			image.GetImages(), image.GetImageCount(),
			image.GetMetadata(), SRV.GetAddressOf());
		return SRV;
	}
	else
	{
		std::cout << "Missing texture " << textureFileName << "\n";
		return nullptr;
	}
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SimpleEngine::Material::InitTextureSRV(Microsoft::WRL::ComPtr<ID3D11Device> device, const DirectX::SimpleMath::Color& color)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.Height = 1;
	textureDesc.Width = 1;
	textureDesc.SampleDesc = { 1,0 };

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = &color;
	initData.SysMemPitch = 1;

	auto res = device->CreateTexture2D(&textureDesc, &initData, texture.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC positionSRVDesc = {};
	positionSRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	positionSRVDesc.Texture2D.MipLevels = 1;
	positionSRVDesc.Texture2D.MostDetailedMip = 0;
	positionSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV;

	res = device->CreateShaderResourceView(texture.Get(), &positionSRVDesc, SRV.GetAddressOf());

	return SRV;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SimpleEngine::Material::InitTextureSRV(Microsoft::WRL::ComPtr<ID3D11Device> device, const float value)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.Height = 1;
	textureDesc.Width = 1;
	textureDesc.SampleDesc = { 1,0 };

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = &value;
	initData.SysMemPitch = 1;

	auto res = device->CreateTexture2D(&textureDesc, &initData, texture.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC positionSRVDesc = {};
	positionSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	positionSRVDesc.Texture2D.MipLevels = 1;
	positionSRVDesc.Texture2D.MostDetailedMip = 0;
	positionSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV;

	res = device->CreateShaderResourceView(texture.Get(), &positionSRVDesc, SRV.GetAddressOf());

	return SRV;
}