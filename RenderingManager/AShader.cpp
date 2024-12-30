#include "AShader.h"
#include "DirectXUtilities.h"

using namespace std;

void AShader::CreateShader(
	const wstring& shaderPath, 
	const string& entryPoint, 
	const string& targetVersion,
	ID3D11Device* device
)
{
	HRESULT hResult = D3DCompileFromFile(
		shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint.c_str(), targetVersion.c_str(), ShaderCompileFlag, 0,
		shaderByteCode.GetAddressOf(), errorByteCode.GetAddressOf()
	);
	AssertIfFailed(hResult);

	CreateShaderImpl(shaderPath, entryPoint, targetVersion, device);

	ResetByteCode();
}

void AShader::ResetByteCode()
{
	shaderByteCode.Reset();
	errorByteCode.Reset();
}

void AShader::SetSamplerState(
	ID3D11DeviceContext* deviceContext, 
	const vector<ID3D11SamplerState*>& samplerStates
)
{
	m_resetSamplerState.resize(samplerStates.size(), nullptr);
}
