#include "pch.h"
#include "Material.h"

#include <DirectXTex.h>
#include "Utils.h"
#include "Game.h"
#include "RenderSystem.h"

const std::string kDefaultPSName = "../shaders/DefaultDeferredPS.hlsl";
const std::string kDefaultVSName = "../shaders/DefaultDeferredVS.hlsl";
const std::string kDefaultAlbedoName = "../assets/debug.png";

constexpr DirectX::SimpleMath::Vector3 kDefaultNormal = DirectX::SimpleMath::Vector3(0.5f, 0.5f, 1.0f);
constexpr float kDefaultMetallic = 0.0f; 
constexpr float kDefaultRoughtness = 0.5f;
constexpr float kDefaultAO = 1.0f;

SimpleEngine::Material::Material():
	mPSFileName(kDefaultPSName),
	mVSFileName(kDefaultVSName)
{
}

SimpleEngine::Material::Material(const Material& other):
	mPSFileName(other.mPSFileName),
	mVSFileName(other.mVSFileName),
	mType(other.mType)
{

}

SimpleEngine::Material& SimpleEngine::Material::operator=(const Material& other)
{
	if (&other != this) {
		mPSFileName = other.mPSFileName;
		mVSFileName = other.mVSFileName;
		mType = other.mType;
	}
	return *this;
}


void SimpleEngine::Material::Init()
{
	using namespace DirectX::SimpleMath;
	auto device = Game::GetRenderSystem()->getDevice();

	D3D_SHADER_MACRO shaderMacros[] = {nullptr, nullptr };

	CompileShaders(device, shaderMacros);

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
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"TANGENT",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};

	auto res = device->CreateInputLayout(
		inputElements,
		std::size(inputElements),
		mVertexShaderByteCode->GetBufferPointer(),
		mVertexShaderByteCode->GetBufferSize(),
		mInputLayout.GetAddressOf());

	///rastState initialization
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	//rastDesc.FillMode = D3D11_FILL_WIREFRAME;

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

	switch (mType)
	{
	case SimpleEngine::MaterialType::Opacue:
		if (!mAlbedoMapTexture)
		{
			mAlbedoMapTexture = CreateTexture(device, kDefaultAlbedoName);
		}
		mAlbedoMap = CreateTextureSRV(device, mAlbedoMapTexture);
		mNormalMapTexture = CreateTexture(device, ToVector4(kDefaultNormal, 0.0f));
		mNormalMap = CreateTextureSRV(device, mNormalMapTexture);
		mMetallicRoughnessAOMapTexture = CreateTexture(device, Vector4(kDefaultMetallic, kDefaultRoughtness, kDefaultAO, 0));
		mMetallicRoughnessAOMap = CreateTextureSRV(device, mMetallicRoughnessAOMapTexture);
		break;
	case SimpleEngine::MaterialType::Light:
		break;
	case SimpleEngine::MaterialType::ColorPass:
		break;
	case SimpleEngine::MaterialType::Debug:
		if (!mAlbedoMapTexture)
		{
			mAlbedoMapTexture = CreateTexture(device, kDefaultAlbedoName);
		}
		mAlbedoMap = CreateTextureSRV(device, mAlbedoMapTexture);
		break;
	default:
		break;
	}
}

void SimpleEngine::Material::CompileShaders(Microsoft::WRL::ComPtr<ID3D11Device>& device, D3D_SHADER_MACRO* shaderMacros)
{
	ID3DBlob* errorVertexCode = nullptr;
	std::wstring VSFileName(mVSFileName.begin(), mVSFileName.end());
	auto res = D3DCompileFromFile(VSFileName.c_str(),
	shaderMacros,
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
	shaderMacros,
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

	///pixelShader initialization*/
	device->CreatePixelShader(
		mPixelShaderByteCode->GetBufferPointer(),
		mPixelShaderByteCode->GetBufferSize(),
		nullptr, mPixelShader.GetAddressOf());
}

void SimpleEngine::Material::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	context->RSSetState(mRastState.Get());
	context->IASetInputLayout(mInputLayout.Get());

	///Set Vertex and Pixel Resources
	context->VSSetShader(mVertexShader.Get(), nullptr, 0);
	context->PSSetShader(mPixelShader.Get(), nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	
	context->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());

	ID3D11ShaderResourceView* SRVs[] = { mAlbedoMap.Get(), mNormalMap.Get(), mMetallicRoughnessAOMap.Get() };
	switch (mType)
	{
	case SimpleEngine::MaterialType::Opacue:
		context->PSSetShaderResources(1, 3, SRVs);
		break;
	case SimpleEngine::MaterialType::Debug:
		context->PSSetShaderResources(1, 1, SRVs);
		break;
	default:
		break;
	}
	
}

void SimpleEngine::Material::UnbindResources(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	ID3D11ShaderResourceView* nullSRVs[] = { nullptr, nullptr, nullptr };
	switch (mType)
	{
	case SimpleEngine::MaterialType::Opacue:
		context->PSSetShaderResources(1, 3, nullSRVs);
		break;
	case SimpleEngine::MaterialType::Debug:
		context->PSSetShaderResources(1, 1, nullSRVs);
		break;
	default:
		break;
	}
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

void SimpleEngine::Material::SetType(MaterialType type)
{
	mType = type;
}

SimpleEngine::MaterialType SimpleEngine::Material::GetType() const
{
	return mType;
}

bool SimpleEngine::Material::IsDeferred() const
{
	return mType == MaterialType::Opacue;
}

void SimpleEngine::Material::SetAlbedo(const std::string& albedoFileName)
{
	auto device = Game::GetRenderSystem()->getDevice();
	mAlbedoMapTexture = CreateTexture(device, albedoFileName);
}

void SimpleEngine::Material::SetAlbedo(DirectX::SimpleMath::Color color)
{
	auto device = Game::GetRenderSystem()->getDevice();
	mAlbedoMapTexture = CreateTexture(device, color);
}

Microsoft::WRL::ComPtr<ID3D11Texture2D> SimpleEngine::Material::CreateTexture(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::string& textureFileName)
{
	std::wstring fileName(textureFileName.begin(), textureFileName.end());
	DirectX::ScratchImage scratchImage;
	HRESULT res;
	if (_wcsicmp(fileName.c_str(), L".dds") == 0)
	{
		res = LoadFromDDSFile(fileName.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, scratchImage);
	}
	else if (_wcsicmp(fileName.c_str(), L".tga") == 0)
	{
		res = LoadFromTGAFile(fileName.c_str(), nullptr, scratchImage);
	}
	else if (_wcsicmp(fileName.c_str(), L".hdr") == 0)
	{
		res = LoadFromHDRFile(fileName.c_str(), nullptr, scratchImage);
	}
	else
	{
		res = LoadFromWICFile(fileName.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, scratchImage);
	}

	if (SUCCEEDED(res))
	{
		auto image = scratchImage.GetImage(0, 0, 0);

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.ArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.Format = image->format;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.Height = image->height;
		textureDesc.Width = image->width;
		textureDesc.SampleDesc = { 1,0 };

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = image->pixels;
		initData.SysMemPitch = image->rowPitch;

		auto res = device->CreateTexture2D(&textureDesc, &initData, texture.GetAddressOf());

		return texture;
	}
	else
	{
		std::cout << "Missing texture " << textureFileName << "\n";
		return nullptr;
	}
}

Microsoft::WRL::ComPtr<ID3D11Texture2D> SimpleEngine::Material::CreateTexture(Microsoft::WRL::ComPtr<ID3D11Device> device, const DirectX::SimpleMath::Color& color)
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

	return texture;
}

Microsoft::WRL::ComPtr<ID3D11Texture2D> SimpleEngine::Material::CreateTexture(Microsoft::WRL::ComPtr<ID3D11Device> device, const float value)
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

	return texture;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SimpleEngine::Material::CreateTextureSRV(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11Texture2D> texture)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texture->GetDesc(&texDesc);
	D3D11_SHADER_RESOURCE_VIEW_DESC positionSRVDesc = {};
	positionSRVDesc.Format = texDesc.Format;
	positionSRVDesc.Texture2D.MipLevels = 1;
	positionSRVDesc.Texture2D.MostDetailedMip = 0;
	positionSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV;

	HRESULT res = device->CreateShaderResourceView(texture.Get(), &positionSRVDesc, SRV.GetAddressOf());

	return SRV;
}